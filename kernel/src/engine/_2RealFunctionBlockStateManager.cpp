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
		m_IsTriggeringEnabled( false ),
		m_IsFlaggedForHalting( false ),
		m_IsFlaggedForShutdown( false ),
		m_IOManager( nullptr ),
		m_UpdatePolicy( nullptr ),
		m_Threads( EngineImpl::instance().getThreadPool() ),
		m_Logger( EngineImpl::instance().getLogger() ),
		m_TimeTrigger( nullptr ),
		m_Thread( nullptr ),
		m_HasException( false ),
		m_Exception( "" )
	{
		m_Thread = m_Threads.requestUniqueThread();
	}

	FunctionBlockStateManager::~FunctionBlockStateManager()
	{
		delete m_CurrentState;
	}

	void FunctionBlockStateManager::handleStateChangeException( Exception &e )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		std::cout << "-------------------------------------------------------------------" << std::endl;
		std::cout << getName() << " EXCEPTION: " << e.message() << std::endl;
		std::cout << "-------------------------------------------------------------------" << std::endl;

		m_Logger.addLine( std::string( getName() + " state: error\n\t" + e.message() ) );

		m_HasException = true;
		m_Exception = e;
	}

	bool FunctionBlockStateManager::isRunning() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_StateAccess );
		return ( *m_CurrentState == AbstractFunctionBlockState::UPDATING );
	}

	void FunctionBlockStateManager::setUp()
	{
		m_StateAccess.lock();

		// created: ok
		// initialized: ok
		// excpetion: ok
		// else: throw exception
		m_CurrentState->setUp( *this );

		m_UpdatePolicy->syncChanges();

		// blocking request /////////////////////////////////////
		ThreadExecRequest *req = new ThreadExecRequest( *this, &FunctionBlockStateManager::setupFunctionBlock );
		Poco::Event *ev = new Poco::Event();
		req->event = ev;
		m_Threads.scheduleRequest( *req, m_Thread );
		ev->wait();
		delete ev;
		// blocking request /////////////////////////////////////

		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		if ( m_HasException )
		{
			// state change /////////////////////////////////////////
			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateError();
			m_StateAccess.unlock();
			// state change /////////////////////////////////////////

			m_Owner.handleException( m_Exception );
		}
		else
		{
			// state change /////////////////////////////////////////
			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateInitialized();
			m_Logger.addLine( string( getName() + " state: initialized" ) );
			m_StateAccess.unlock();
			// state change /////////////////////////////////////////
		}
	}

	void FunctionBlockStateManager::setupFunctionBlock()
	{
		// called by pooled thread

		try
		{
			m_IOManager->clearInletBuffers();
			m_IOManager->updateInletData();

			bundle::BlockHandle &h = m_IOManager->getHandle();
			m_FunctionBlock->setup( h );
			m_Logger.addLine( std::string( getName() + " carried out setup" ) );
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
		}
	}

	void FunctionBlockStateManager::start()
	{
		m_StateAccess.lock();

		// initialized: ok
		// else: throw exception
		m_CurrentState->start( *this );

		// state change /////////////////////////////////////////
		delete m_CurrentState;
		m_Logger.addLine( string( getName() + " new state: updating" ) );
		m_CurrentState = new FunctionBlockStateUpdating();
		// state change /////////////////////////////////////////

		m_StateAccess.unlock();

		m_StopEvent.reset();
		m_UpdatePolicy->syncChanges();

		// enable triggers /////////////////////////////////////////
		m_EnabledAccess.lock();
		m_IsTriggeringEnabled = true;
		m_EnabledAccess.unlock();
		// enable triggers /////////////////////////////////////////

		setTriggers( false );
		m_IOManager->updateInletBuffers( true );
	}

	void FunctionBlockStateManager::updateFunctionBlock()
	{
		try
		{
			m_IOManager->updateInletData();
			m_FunctionBlock->update();
			m_IOManager->updateOutletData();

			m_Logger.addLine( std::string( getName() + " carried out update" ) );
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
		}

		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		if ( m_IsFlaggedForShutdown.isSet() )
		{
			m_StateAccess.lock();

			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateStopped();
			m_Logger.addLine( std::string( getName() + " new state: stopped ( finished update cycle )" ) );

			m_StopEvent.set();
			m_ShutdownEvent.set();
			m_IsFlaggedForShutdown.unset();
			m_IsFlaggedForHalting.unset();

			m_StateAccess.unlock();
		}
		else if ( m_HasException )
		{
			m_StateAccess.lock();

			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateError();
			m_StateAccess.unlock();

			m_Owner.handleException( m_Exception );
		}
		else if ( m_IsFlaggedForHalting.isSet() )
		{
			m_StateAccess.lock();

			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateInitialized();
			m_Logger.addLine( std::string( getName() + " new state: initialized ( finished update cycle )" ) );

			m_StopEvent.set();
			m_IsFlaggedForHalting.unset();

			m_StateAccess.unlock();
		}
		else
		{
			m_UpdatePolicy->syncChanges();

			m_EnabledAccess.lock();
			m_IsTriggeringEnabled = true;
			m_EnabledAccess.unlock();

			setTriggers( false );

			m_IOManager->updateInletBuffers( true );
		}
	}

	void FunctionBlockStateManager::singleStep()
	{
		m_StateAccess.lock();

		// initialized: ok
		// else: throw exception
		m_CurrentState->start( *this );

		// without enabling triggers update inlet data //////////
		setTriggers( false );
		m_IOManager->updateInletBuffers( false );
		/////////////////////////////////////////////////////////

		// blocking request /////////////////////////////////////
		ThreadExecRequest *req = new ThreadExecRequest( *this, &FunctionBlockStateManager::singleStepFunctionBlock );
		Poco::Event *ev = new Poco::Event();
		req->event = ev;
		m_Threads.scheduleRequest( *req, m_Thread );
		ev->wait();
		delete ev;
		// blocking request /////////////////////////////////////

		//

		if ( m_HasException )
		{
			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateError();
			m_StateAccess.unlock();

			m_Owner.handleException( m_Exception );
		}
		else
		{
			m_StateAccess.unlock();
		}
	}

	void FunctionBlockStateManager::singleStepFunctionBlock()
	{
		try
		{
			m_IOManager->updateInletData();
			m_FunctionBlock->update();
			m_IOManager->updateOutletData();

			m_Logger.addLine( std::string( getName() + " carried out singlestep" ) );
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
		}
	}

	void FunctionBlockStateManager::prepareForShutDown()
	{
		m_StateAccess.lock();
		if ( m_CurrentState->tryStop( *this ) )
		{
			// state change /////////////////////////////////////////
			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateStopped();
			m_Logger.addLine( std::string( getName() + " state: stopped ( framework stop )" ) );
			// state change /////////////////////////////////////////

			m_StopEvent.set();
			m_ShutdownEvent.set();
		}
		else
		{
			m_IsFlaggedForShutdown.set();
			m_Logger.addLine( std::string( getName() + " flagged for shutdown" ) );
		}

		m_StateAccess.unlock();
	}

	Poco::Event & FunctionBlockStateManager::stop()
	{
		m_StateAccess.lock();

		if ( m_CurrentState->tryHalt( *this ) )
		{
			// state change /////////////////////////////////////////
			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateInitialized();
			m_Logger.addLine( std::string( getName() + " state: initialized ( user stop )" ) );
			// state change /////////////////////////////////////////

			m_StopEvent.set();
		}
		else
		{
			m_IsFlaggedForHalting.set();
			m_Logger.addLine( std::string( getName() + " flagged for halting" ) );
		}

		m_StateAccess.unlock();
		return m_StopEvent;
	}

	bool FunctionBlockStateManager::shutDown( const long timeout )
	{
		if ( m_ShutdownEvent.tryWait( timeout ) )
		{
			m_StateAccess.lock();

			// blocking request /////////////////////////////////////
			ThreadExecRequest *req = new ThreadExecRequest( *this, &FunctionBlockStateManager::shutdownFunctionBlock );
			Poco::Event *ev = new Poco::Event();
			req->event = ev;
			m_Threads.scheduleRequest( *req, m_Thread );
			ev->wait();
			delete ev;
			// blocking request /////////////////////////////////////

			if ( m_HasException )
			{
				// state change /////////////////////////////////////////
				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateError();
				m_StateAccess.unlock();
				// state change /////////////////////////////////////////

				m_Owner.handleException( m_Exception );
			}
			else
			{
				// state change /////////////////////////////////////////
				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateShutDown();
				m_Logger.addLine( std::string( getName() + " state: shut down" ) );
				// state change /////////////////////////////////////////

				m_StateAccess.unlock();
			}
			return true;
		}
		else
		{
			// state change /////////////////////////////////////////
			_2Real::Exception e( "timeout on shutdown" );
			handleStateChangeException( e );
			// state change /////////////////////////////////////////

			return false;
		}
	}

	void FunctionBlockStateManager::shutdownFunctionBlock()
	{
		try
		{
			m_FunctionBlock->shutdown();
			m_Logger.addLine( std::string( getName() + " carried out shutdown" ) );
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
		}
	}

	void FunctionBlockStateManager::tryTriggerInlet( AbstractInletBasedTrigger &trigger )
	{
		m_EnabledAccess.lock();

		if ( m_IsTriggeringEnabled )
		{
			m_TriggerAccess.lock();

			// check inlet triggers /////////////////////////////////////////
			bool groupWeightResult = true;
			bool singleWeightResult = false;

			for ( InletTriggerIterator it = m_SingleInletTriggers.begin(); it != m_SingleInletTriggers.end(); ++it )	singleWeightResult |= ( *it )->isFulfilled();
			for ( InletTriggerIterator it = m_GroupInletTriggers.begin(); it != m_GroupInletTriggers.end(); ++it )		groupWeightResult &= ( *it )->isFulfilled();
			groupWeightResult &= !m_GroupInletTriggers.empty();

			bool triggersOk = ( groupWeightResult || singleWeightResult );
			// check inlet triggers /////////////////////////////////////////

			if ( m_TimeTrigger != nullptr )
			{
				triggersOk &= m_TimeTrigger->isFulfilled();
			}

			m_TriggerAccess.unlock();

			// schedule request /////////////////////////////////////////
			if ( triggersOk )
			{
				m_IsTriggeringEnabled = false;
				m_Logger.addLine( std::string( getName() + " inlet fulfilled trigger conditions" ) );

				ThreadExecRequest *req = new ThreadExecRequest( *this, &FunctionBlockStateManager::updateFunctionBlock );
				m_Threads.scheduleRequest( *req, m_Thread );

				m_Logger.addLine( std::string( getName() + " scheduled update request" ) );
			}
			// schedule request /////////////////////////////////////////
			else
			{
				//m_Logger.addLine( std::string( getName() + " recvd inlet trigger but no update" ) );
			}
		}
		else
		{
			//m_Logger.addLine( std::string( getName() + " recvd inlet trigger while disabled" ) );
		}

		m_EnabledAccess.unlock();
	}

	void FunctionBlockStateManager::tryTriggerTime( TimeBasedTrigger &trigger )
	{
		m_EnabledAccess.lock();

		if ( m_IsTriggeringEnabled )
		{
			m_TriggerAccess.lock();

			bool triggersOk = true;
			if ( !( m_SingleInletTriggers.empty() && m_GroupInletTriggers.empty() ) )
			{
				// check inlet triggers /////////////////////////////////////////
				bool groupWeightResult = true;
				bool singleWeightResult = false;

				for ( InletTriggerIterator it = m_SingleInletTriggers.begin(); it != m_SingleInletTriggers.end(); ++it )	singleWeightResult |= ( *it )->isFulfilled();
				for ( InletTriggerIterator it = m_GroupInletTriggers.begin(); it != m_GroupInletTriggers.end(); ++it )		groupWeightResult &= ( *it )->isFulfilled();
				groupWeightResult &= !m_GroupInletTriggers.empty();

				triggersOk = ( groupWeightResult || singleWeightResult );
				// check inlet triggers /////////////////////////////////////////
			}

			// time need not be checked, as this is the time trigger anyway

			m_TriggerAccess.unlock();

			// schedule request /////////////////////////////////////////
			if ( triggersOk )
			{
				m_IsTriggeringEnabled = false;
				m_Logger.addLine( std::string( getName() + " time fulfilled trigger conditions" ) );

				ThreadExecRequest *req = new ThreadExecRequest( *this, &FunctionBlockStateManager::updateFunctionBlock );
				m_Threads.scheduleRequest( *req, m_Thread );

				m_Logger.addLine( std::string( getName() + " scheduled update request" ) );
			}
			// schedule request /////////////////////////////////////////
			else
			{
				//m_Logger.addLine( std::string( getName() + " recvd time trigger but no update" ) );
			}
		}
		else
		{
			//m_Logger.addLine( std::string( getName() + " recvd time trigger while disabled" ) );
		}

		m_EnabledAccess.unlock();
	}

	void FunctionBlockStateManager::addTrigger( AbstractInletBasedTrigger &trigger, const bool isOr )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		if ( isOr )		m_SingleInletTriggers.insert( &trigger );
		else			m_GroupInletTriggers.insert( &trigger );
	}

	void FunctionBlockStateManager::removeTrigger( AbstractInletBasedTrigger &trigger, const bool isOr )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		if ( isOr )
		{
			InletTriggerIterator it = m_SingleInletTriggers.find( &trigger );
			if ( it != m_SingleInletTriggers.end() )	m_SingleInletTriggers.erase( it );
		}
		else
		{
			InletTriggerIterator it = m_GroupInletTriggers.find( &trigger );
			if ( it != m_GroupInletTriggers.end() )		m_GroupInletTriggers.erase( it );
		}
	}

	void FunctionBlockStateManager::addTrigger( TimeBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		m_TimeTrigger = &trigger;
	}

	void FunctionBlockStateManager::removeTrigger( TimeBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		m_TimeTrigger = nullptr;
	}

	void FunctionBlockStateManager::setTriggers( const bool fulfilled )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		for ( InletTriggerIterator it = m_SingleInletTriggers.begin(); it != m_SingleInletTriggers.end(); ++it )	( *it )->set( fulfilled );
		for ( InletTriggerIterator it = m_GroupInletTriggers.begin(); it != m_GroupInletTriggers.end(); ++it )		( *it )->set( fulfilled );
		if ( m_TimeTrigger != nullptr )																				m_TimeTrigger->set( fulfilled );
	}
}
