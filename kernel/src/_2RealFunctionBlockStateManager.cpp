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

#include "_2RealException.h"
#include "_2RealFunctionBlock.h"
#include "_2RealFunctionBlockState.h"
#include "_2RealAbstractIOManager.h"
#include "_2RealEngineImpl.h"
#include "_2RealThreadPool.h"
#include "bundle/_2RealBlockHandle.h"
#include "_2RealTimer.h"
#include "_2RealInlet.h"
#include "_2RealLogger.h"
#include "_2RealSystemBlock.h"
#include "bundle/_2RealBlock.h"
#include "_2RealHelpersInternal.h"
#include "_2RealUberBlockBasedTrigger.h"

using std::string;

namespace _2Real
{

	FunctionBlockStateManager::FunctionBlockStateManager( AbstractUberBlock &owner ) :
		AbstractStateManager( owner ),
		m_CurrentState( new FunctionBlockStateCreated() ),
		m_FlaggedForSetUp( false ),
		m_FlaggedForStop( false ),
		m_TriggersEnabled( false ),
		m_UberBlockTriggersEnabled( false ),
		m_IOManager( nullptr ),
		m_System( nullptr ),
		m_UpdatePolicy( nullptr ),
		m_Threads( EngineImpl::instance().getThreadPool() ),
		m_Logger( EngineImpl::instance().getLogger() )
	{
	}

	FunctionBlockStateManager::~FunctionBlockStateManager()
	{
		disableAllTriggers();

		delete m_CurrentState;

		//for ( UberBlockTriggerList::iterator it = m_UberBlockTriggers.begin(); it != m_UberBlockTriggers.end(); ++it )
		//{
		//	delete *it;
		//}
	}

	void FunctionBlockStateManager::setUp()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_StateAccess );
		try
		{
			if ( m_CurrentState->trySetUp( *this ) )
			{
				disableAllTriggers();
				resetTriggers();

				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateSetUp();

				m_IOManager->updateParameterValues();

				bundle::BlockHandle context( dynamic_cast< FunctionBlock& >( m_Owner) );
				m_FunctionBlock->setup( context );

				m_Logger.addLine( string( getName() + " new state: set up" ) );
			}
			else
			{
				// there's currently an update cycle going on -> flag for later
				m_FlaggedForSetUp.set();
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
			m_UpdatePolicy->changePolicy();

			m_StateAccess.lock();
			if ( m_CurrentState->tryStart( *this ) )
			{
				disableAllTriggers();
				resetTriggers();

				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateStarted();
				m_StateAccess.unlock();

				m_StopEvent.reset();
				enableTriggers();

				m_IOManager->updateInletBuffers();		///???????????????????????
				m_IOManager->updateParameterValues();

				m_Logger.addLine( string( getName() + " new state: started" ) );
			}
			else
			{
				// nothing
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
				disableAllTriggers();

				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateStopped();
				
				m_Logger.addLine( std::string( getName() + " new state: stopped ( requested by user )" ) );

				m_StopEvent.set();	// this needs to be unset at some point ?
				return m_StopEvent;
			}
			else
			{
				// flag for a later stop
				m_FlaggedForStop.set();
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
		if ( !areTriggersEnabled() )
		{
			//	TODO: it might happen that a block is stopped while trigger eval is in process
			//	this is basically just a quick fix
			//	( if triggers were disabled, it menas that there was a state change in between )
			return;
		}

		try
		{
			m_StateAccess.lock();

			m_CurrentState->triggersAreOk( *this );

			disableTriggers();
			resetTriggers();
			resetUberBlockTriggers();

			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateWaiting();

			m_StateAccess.unlock();

			m_Logger.addLine( std::string( getName() + " new state: waiting" ) );

			enableUberBlockTriggers();

			Poco::ScopedLock< Poco::FastMutex > lock( m_UberBlockTriggerAccess );

			for ( UberBlockTriggerList::iterator it = m_UberBlockTriggers.begin(); it != m_UberBlockTriggers.end(); ++it )
			{
				( *it )->tryTriggerOther( BLOCK_READY );
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
		if ( !areUberBlockTriggersEnabled() )
		{
			//	TODO: it might happen that a block is stopped while trigger eval is in process
			//	this is basically just a quick fix
			//	( if triggers were disabled, it menas that there was a state change in between )
			return;
		}

		try
		{
			m_StateAccess.lock();
			m_CurrentState->uberBlocksAreOk( *this );

			disableUberBlockTriggers();

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

			m_IOManager->updateInletValues();	// make sure the inlets have the current data
			m_FunctionBlock->update();			// carry out update
			m_IOManager->updateOutletValues();	// make sure outlets send their data
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

			if ( m_FlaggedForSetUp.isSet() )
			{
				disableAllTriggers();

				m_IOManager->updateParameterValues();

				bundle::BlockHandle context( dynamic_cast< FunctionBlock& >( m_Owner) );
				m_FunctionBlock->setup( context );
				m_CurrentState = new FunctionBlockStateSetUp();
				m_StateAccess.unlock();

				m_Logger.addLine( std::string( getName() + " new state: set up ( finished update cycle )" ) );
			}
			
			if ( m_FlaggedForStop.isSet() )
			{
				disableAllTriggers();
				m_StopEvent.set();
				m_CurrentState = new FunctionBlockStateStopped();
				m_StateAccess.unlock();

				m_Logger.addLine( std::string( getName() + " new state: stopped ( finished update cycle )" ) );

				m_StopEvent.set();
			}
			else
			{
				disableTriggers();
				resetTriggers();

				m_UpdatePolicy->changePolicy();

				m_CurrentState = new FunctionBlockStateStarted();
				m_StateAccess.unlock();

				m_Logger.addLine( std::string( getName() + " new state: started ( finished update cycle )" ) );

				enableTriggers();

				m_IOManager->updateInletBuffers();
				m_IOManager->updateParameterValues();
			}

			m_FlaggedForStop.unset();
			m_FlaggedForSetUp.unset();
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
				disableAllTriggers();

				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateStopped();

				m_StopEvent.set();
	
				m_Logger.addLine( std::string( getName() + " new state: stopped ( requested by framework )" ) );
				m_StateAccess.unlock();
			}
			else
			{
				m_StateAccess.unlock();
				m_FlaggedForStop.set();
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
			if ( m_StopEvent.tryWait( timeout ) )
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
#ifdef _2REAL_DEBUG
		std::cout << "-------------------------------------------------------------------" << std::endl;
		std::cout << getName() << " EXCEPTION: " << e.message() << std::endl;
		std::cout << "-------------------------------------------------------------------" << std::endl;
#endif
		m_Logger.addLine( std::string( getName() + " new function block state: error\n\t" + e.message() ) );
		disableAllTriggers();
		delete m_CurrentState;
		m_CurrentState = new FunctionBlockStateError();
		m_System->handleException( m_Owner, e );
	}

	void FunctionBlockStateManager::addUberBlockTrigger( UberBlockBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_UberBlockTriggerAccess );
		m_UberBlockTriggers.push_back( &trigger );
	}

	void FunctionBlockStateManager::removeUberBlockTrigger( UberBlockBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_UberBlockTriggerAccess );
		for ( UberBlockTriggerList::iterator it = m_UberBlockTriggers.begin(); it != m_UberBlockTriggers.end(); /**/ )
		{
			if ( *it == &trigger )
			{
				m_UberBlockTriggers.erase( it );
				break;
			}
			++it;
		}
	}

	void FunctionBlockStateManager::addTrigger( AbstractUpdateTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		m_Triggers.push_back( &trigger );
	}

	void FunctionBlockStateManager::removeTrigger( AbstractUpdateTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		for ( TriggerList::iterator it = m_Triggers.begin(); it != m_Triggers.end(); /**/ )
		{
			if ( *it == &trigger )
			{
				m_Triggers.erase( it );
				break;
			}
			++it;
		}
	}

	void FunctionBlockStateManager::tryTrigger( AbstractUpdateTrigger &trigger )
	{
		if ( areTriggersEnabled() )
		{
			m_TriggerAccess.lock();

			bool triggersOk = true;
			for ( TriggerList::iterator it = m_Triggers.begin(); it != m_Triggers.end(); ++it )
			{
				triggersOk &= ( *it )->isOk();
			}

			m_TriggerAccess.unlock();

			if ( triggersOk )
			{
				triggersAreOk();	// request state change
			}
		}
	}

	void FunctionBlockStateManager::resetTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		for ( TriggerList::iterator it = m_Triggers.begin(); it != m_Triggers.end(); ++it )
		{
			( *it )->reset();
		}
	}

	void FunctionBlockStateManager::tryTriggerUberBlock( UberBlockBasedTrigger &trigger )
	{
		if ( areUberBlockTriggersEnabled() )
		{
			m_UberBlockTriggerAccess.lock();
			bool triggersOk = true;
			for ( UberBlockTriggerList::iterator it = m_UberBlockTriggers.begin(); it != m_UberBlockTriggers.end(); ++it )
			{
				triggersOk &= ( *it )->isOk();
			}
			m_UberBlockTriggerAccess.unlock();

			if (triggersOk)
			{
				uberBlocksAreOk();	//request state change
			}
		}
	}

	void FunctionBlockStateManager::resetUberBlockTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_UberBlockTriggerAccess );
		for ( UberBlockTriggerList::iterator it = m_UberBlockTriggers.begin(); it != m_UberBlockTriggers.end(); ++it )
		{
			( *it )->reset();
		}
	}

	bool FunctionBlockStateManager::areTriggersEnabled() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		return m_TriggersEnabled;
	}

	bool FunctionBlockStateManager::areUberBlockTriggersEnabled() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		return m_UberBlockTriggersEnabled;
	}

	void FunctionBlockStateManager::disableAllTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_TriggersEnabled = false;
		m_UberBlockTriggersEnabled = false;
	}

	void FunctionBlockStateManager::disableTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_TriggersEnabled = false;
	}

	void FunctionBlockStateManager::disableUberBlockTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_UberBlockTriggersEnabled = false;
	}

	void FunctionBlockStateManager::enableAllTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_TriggersEnabled = true;
		m_UberBlockTriggersEnabled = true;
	}

	void FunctionBlockStateManager::enableTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_TriggersEnabled = true;
	}

	void FunctionBlockStateManager::enableUberBlockTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_UberBlockTriggersEnabled = true;
	}

}