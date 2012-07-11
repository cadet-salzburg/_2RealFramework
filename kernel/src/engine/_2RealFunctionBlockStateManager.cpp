/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "engine/_2RealFunctionBlockStateManager.h"
#include "engine/_2RealFunctionBlock.h"
#include "engine/_2RealFunctionBlockState.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealThreadPool.h"
#include "engine/_2RealLogger.h"
#include "../_2RealBlock.h"

#include <iostream>

using std::string;

namespace _2Real
{

	FunctionBlockStateManager::FunctionBlockStateManager( AbstractUberBlock &owner ) :
		AbstractStateManager( owner ),
		m_CurrentState( new FunctionBlockStateCreated() ),
		m_IsFlaggedForSetup( false ),
		m_IsFlaggedForHalting( false ),
		m_IsFlaggedForShutdown( false ),
		m_IsTriggeringEnabled( false ),
		m_IOManager( nullptr ),
		m_UpdatePolicy( nullptr ),
		m_Threads( EngineImpl::instance().getThreadPool() ),
		m_Logger( EngineImpl::instance().getLogger() ),
		m_TimeTrigger( nullptr )
	{
	}

	FunctionBlockStateManager::~FunctionBlockStateManager()
	{
		delete m_CurrentState;
	}

	void FunctionBlockStateManager::setUp()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_StateAccess );
		try
		{
			if ( m_CurrentState->trySetUp( *this ) )
			{
				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateSetUp();

				m_IOManager->updateInletData();

				m_FunctionBlock->setup( m_IOManager->getHandle() );
				m_Logger.addLine( string( getName() + " new state: set up" ) );
			}
			else
			{
				m_IsFlaggedForSetup.set();
				m_Logger.addLine( string( getName() + " flagged for delayed set up" ) );
			}
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
		}
	}

	void FunctionBlockStateManager::start()
	{
		try
		{
			m_StateAccess.lock();
			if ( m_CurrentState->tryStart( *this ) )
			{
				m_UpdatePolicy->changePolicy();
				m_StopEvent.reset();
				resetTriggers();

				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateTriggering();
				m_StateAccess.unlock();

				// enable triggers ( inlets, time )
				m_EnabledAccess.lock();
				m_IsTriggeringEnabled = true;
				m_EnabledAccess.unlock();

				m_IOManager->updateInletBuffers();		// give buffered data a chance of triggering
				m_Logger.addLine( string( getName() + " new state: triggering" ) );
			}
			else
			{
				m_StateAccess.unlock();
			}
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
			m_StateAccess.unlock();
		}
	}

	Poco::Event & FunctionBlockStateManager::stop()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_StateAccess );
		try
		{
			if ( m_CurrentState->prepareForShutDown(*this) )
			{
				m_EnabledAccess.lock();
				m_IsTriggeringEnabled = false;
				m_EnabledAccess.unlock();

				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateSetUp();

				m_Logger.addLine( std::string( getName() + " new state: setup ( stop requested by user )" ) );

				m_StopEvent.set();
				return m_StopEvent;
			}
			else
			{
				m_IsFlaggedForHalting.set();
				m_Logger.addLine( std::string( getName() + " flagged for delayed stop" ) );
				return m_StopEvent;
			}
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
			return m_StopEvent;
		}
	}

	void FunctionBlockStateManager::triggersAreOk()
	{
		try
		{
			m_StateAccess.lock();

			if ( !m_IsTriggeringEnabled )
			{
				m_StateAccess.unlock();
				return;
			}

			m_CurrentState->triggersAreOk( *this );

			// no more triggers accepted
			m_EnabledAccess.lock();
			m_IsTriggeringEnabled = false;
			m_EnabledAccess.unlock();

			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateWaiting();

			m_StateAccess.unlock();

			m_Logger.addLine( std::string( getName() + " new state: waiting ( triggered )" ) );
			uberBlocksAreOk();	// currently, does nothing
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
			m_StateAccess.unlock();
		}
	}

	void FunctionBlockStateManager::singleStep()
	{
		try
		{
			m_StateAccess.lock();

			if ( m_CurrentState->singleStep( *this ) )
			{
				m_StopEvent.reset();

				// a singlestep overrides all triggers
				m_EnabledAccess.lock();
				m_IsTriggeringEnabled = false;
				m_EnabledAccess.unlock();

				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateWaiting();

				m_IsFlaggedForHalting.set();
				m_StateAccess.unlock();

				m_Logger.addLine( std::string( getName() + " new state: waiting ( singlestepped )" ) );
				uberBlocksAreOk();
			}
			else
			{
				m_StateAccess.unlock();
			}
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
			m_StateAccess.unlock();
		}
	}

	void FunctionBlockStateManager::uberBlocksAreOk()
	{
		try
		{
			m_StateAccess.lock();
			m_CurrentState->uberBlocksAreOk( *this );

			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateScheduled();
			m_StateAccess.unlock();

			m_Logger.addLine( std::string( getName() + " new state: scheduled" ) );
			m_Threads.scheduleService( *this );
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
			m_StateAccess.unlock();
		}
	}

	void FunctionBlockStateManager::beginUpdate()
	{
		try
		{
			m_StateAccess.lock();
			m_CurrentState->beginUpdate( *this );
			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateUpdating();
			m_StateAccess.unlock();

			m_Logger.addLine( std::string( getName() + " new state: updating" ) );
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
			m_StateAccess.unlock();
		}
	}

	void FunctionBlockStateManager::updateFunctionBlock()
	{
		try
		{
			m_StateAccess.lock();
			m_CurrentState->update( *this );	// no state change in this case, although maybe i should add another? sigh
			m_StateAccess.unlock();

			m_IOManager->updateInletData();		// make sure the inlets have the current data
			m_FunctionBlock->update();			// carry out update
			m_IOManager->updateOutletData();	// make sure outlets send their data
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
			m_StateAccess.unlock();
		}
	}

	void FunctionBlockStateManager::finishUpdate()
	{
		try
		{
			m_StateAccess.lock();

			m_CurrentState->finishUpdate( *this );
			delete m_CurrentState;

			if ( m_IsFlaggedForShutdown.isSet() )
			{
				m_StopEvent.set();		// just in case someone is waiting for a stop
				m_ShutdownEvent.set();

				m_CurrentState = new FunctionBlockStateStopped();
				m_StateAccess.unlock();

				m_Logger.addLine( std::string( getName() + " new state: shut down ( finished update cycle )" ) );
				m_IsFlaggedForShutdown.unset();
			}
			else if ( m_IsFlaggedForHalting.isSet() )
			{
				m_IOManager->updateInletData();

				if ( m_IsFlaggedForSetup.isSet() )
				{
					bundle::BlockHandle context( *m_IOManager );
					m_FunctionBlock->setup( context );
					m_IsFlaggedForSetup.unset();
				}

				m_CurrentState = new FunctionBlockStateSetUp();
				m_StateAccess.unlock();

				m_StopEvent.set();

				m_Logger.addLine( std::string( getName() + " new state: set up ( finished update cycle )" ) );
				m_IsFlaggedForHalting.unset();
			}
			else
			{
				m_IOManager->updateInletData();

				if ( m_IsFlaggedForSetup.isSet() )
				{
					bundle::BlockHandle context( *m_IOManager );
					m_FunctionBlock->setup( context );
					m_IsFlaggedForSetup.unset();
				}

				m_UpdatePolicy->changePolicy();
				m_CurrentState = new FunctionBlockStateTriggering();

				// re-enable triggering
				m_EnabledAccess.lock();
				m_IsTriggeringEnabled = true;
				m_EnabledAccess.unlock();

				m_StateAccess.unlock();

				m_Logger.addLine( std::string( getName() + " new state: started ( finished update cycle )" ) );

				resetTriggers();
				m_IOManager->updateInletBuffers();
			}
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
			m_StateAccess.unlock();
		}
	}

	void FunctionBlockStateManager::prepareForShutDown()
	{
		try
		{
			m_StateAccess.lock();
			if ( m_CurrentState->prepareForShutDown( *this ) )
			{
				m_EnabledAccess.lock();
				m_IsTriggeringEnabled = false;
				m_EnabledAccess.unlock();

				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateStopped();

				m_StopEvent.set();
				m_ShutdownEvent.set();
	
				m_Logger.addLine( std::string( getName() + " new state: stopped ( requested by framework )" ) );
				m_StateAccess.unlock();
			}
			else
			{
				m_StateAccess.unlock();
				m_IsFlaggedForShutdown.set();
				m_Logger.addLine( std::string( getName() + " flagged for delayed stop" ) );
			}
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
			m_StateAccess.unlock();
		}
	}

	bool FunctionBlockStateManager::shutDown( const long timeout )
	{
		try
		{
			if ( m_ShutdownEvent.tryWait( timeout ) )
			{
				m_StateAccess.lock();
				m_CurrentState->shutDown( *this );
				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateShutDown();
				m_StateAccess.unlock();

				m_Logger.addLine( std::string( getName() + " new state: shut down" ) );

				m_FunctionBlock->shutdown();
				return true;
			}
			else
			{
				m_StateAccess.lock();
				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateError();
				m_StateAccess.unlock();

				m_Logger.addLine( std::string( getName() + " new state: error (aborted because of timeout on shutdown)" ) );

				m_Threads.abortService( *this );
				return false;
			}
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
			m_StateAccess.unlock();
			return false;
		}
	}

	void FunctionBlockStateManager::handleStateChangeException(Exception &e)
	{
		std::cout << "-------------------------------------------------------------------" << std::endl;
		std::cout << getName() << " EXCEPTION: " << e.message() << std::endl;
		std::cout << "-------------------------------------------------------------------" << std::endl;

		m_Logger.addLine( std::string( getName() + " new function block state: error\n\t" + e.message() ) );

		m_EnabledAccess.lock();
		m_IsTriggeringEnabled = false;
		m_EnabledAccess.unlock();

		delete m_CurrentState;
		m_CurrentState = new FunctionBlockStateError();
		m_Owner.handleException( e );
	}

	void FunctionBlockStateManager::addTrigger( AbstractInletBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		m_InletTriggers.insert( &trigger );
	}

	void FunctionBlockStateManager::removeTrigger( AbstractInletBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		InletTriggerIterator it = m_InletTriggers.find( &trigger );
		if ( it != m_InletTriggers.end() )
		{
			m_InletTriggers.erase( it );
		}
	}

	void FunctionBlockStateManager::addTrigger( AbstractTimeBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		m_TimeTrigger = &trigger;
	}

	void FunctionBlockStateManager::removeTrigger( AbstractTimeBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		m_TimeTrigger = nullptr;
	}

	void FunctionBlockStateManager::tryTriggerInlet( AbstractInletBasedTrigger &trigger )
	{
		m_EnabledAccess.lock();
		if ( m_IsTriggeringEnabled )
		{
			m_EnabledAccess.unlock();
			m_TriggerAccess.lock();

			bool andRes = true;
			unsigned int andCount = 0;
			bool orRes = false;
			for ( InletTriggerIterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); ++it )
			{
				if ( ( *it )->isOr() )
				{
					orRes |= ( *it )->isOk();
				}
				else
				{
					andCount++;
					andRes &= ( *it )->isOk();
				}
			}
			bool triggersOk = ( ( andRes && ( andCount > 0 ) ) || orRes );
			if ( m_TimeTrigger != nullptr )
			{
				triggersOk &= m_TimeTrigger->isOk();
			}

			m_TriggerAccess.unlock();
			if ( triggersOk )
			{
				triggersAreOk();
			}
		}
		else
		{
			m_EnabledAccess.unlock();
		}
	}

	void FunctionBlockStateManager::tryTriggerTime( AbstractTimeBasedTrigger &trigger )
	{
		m_EnabledAccess.lock();
		if ( m_IsTriggeringEnabled )
		{
			m_EnabledAccess.unlock();
			m_TriggerAccess.lock();

			if ( m_InletTriggers.empty() )
			{
				m_TriggerAccess.unlock();
				triggersAreOk();
			}
			else
			{
				bool andRes = true;
				unsigned int andCount = 0;
				bool orRes = false;
				for ( InletTriggerIterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); ++it )
				{
					if ( ( *it )->isOr() )
					{
						orRes |= ( *it )->isOk();
					}
					else
					{
						andCount++;
						andRes &= ( *it )->isOk();
					}
				}
				
				bool triggersOk = ( ( andRes && ( andCount > 0 ) ) || orRes );
				m_TriggerAccess.unlock();

				if ( triggersOk )
				{
					triggersAreOk();
				}
			}
		}
		else
		{
			m_EnabledAccess.unlock();
		}
	}

	void FunctionBlockStateManager::resetTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		for ( InletTriggerIterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); ++it )
		{
			( *it )->reset();
		}
		if ( m_TimeTrigger != nullptr )
		{
			m_TimeTrigger->reset();
		}
	}
}