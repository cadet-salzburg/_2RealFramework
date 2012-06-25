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
#include "_2RealUpdatePolicyImpl.h"
#include "_2RealFrameworkContext.h"
#include "_2RealTimer.h"
#include "_2RealInlet.h"
#include "_2RealLogger.h"
#include "_2RealSystemImpl.h"
#include "_2RealBlock.h"
#include "_2RealHelpersInternal.h"

using std::string;

namespace _2Real
{

	FunctionBlockStateManager::FunctionBlockStateManager( AbstractBlock &owner ) :
		AbstractStateManager( owner ),
		m_CurrentState( new FunctionBlockStateCreated() ),
		m_FlaggedForSetUp( false ),
		m_FlaggedForStop( false ),
		m_FlaggedForPolicyChange( true ),
		m_TriggersEnabled( false ),
		m_UberTriggersEnabled( false ),
		m_TimeTrigger( nullptr ),
		m_IO( nullptr ),
		m_System( nullptr ),
		m_CurrentPolicy( nullptr ),
		m_NewPolicy( nullptr ),
		m_Threads( EngineImpl::instance().getThreadPool() ),
		m_Logger( EngineImpl::instance().getLogger() )
	{
	}

	FunctionBlockStateManager::~FunctionBlockStateManager()
	{
		disableAllTriggers();

		safeDelete( m_CurrentState );
		safeDelete( m_CurrentPolicy );
		safeDelete( m_NewPolicy );

		if ( m_TimeTrigger != nullptr )
		{
			EngineImpl::instance().getTimer().unregisterFromTimerSignal( *this );
			safeDelete( m_TimeTrigger );
		}

		InletMap inlets = m_IO->getInlets();

		for ( InletBasedTriggerMap::iterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); /**/ )
		{
			InletMap::iterator ioIt = inlets.find( it->first );
			if ( ioIt != inlets.end() )
			{
				ioIt->second->unregisterFromDataReceived( *this );
			}
			
			safeDelete( it->second );
			it = m_InletTriggers.erase( it );
		}

		for ( UberBlockBasedTriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); /**/ )
		{
			safeDelete( it->second.first );
			it = m_UberTriggers.erase( it );
		}
	}

	void FunctionBlockStateManager::setUpdatePolicy( UpdatePolicyImpl const& policy )
	{
		m_FlaggedForPolicyChange.set();

		Poco::ScopedLock< Poco::FastMutex > lock( m_PolicyAccess );

		safeDelete( m_NewPolicy );
		m_NewPolicy = &policy;
	}

	void FunctionBlockStateManager::changePolicy()
	{
		m_PolicyAccess.lock();

		safeDelete( m_CurrentPolicy );
		m_CurrentPolicy = m_NewPolicy;
		m_NewPolicy = nullptr;

		m_PolicyAccess.unlock();

		if ( m_TimeTrigger != nullptr )
		{
			EngineImpl::instance().getTimer().unregisterFromTimerSignal( *this );
			safeDelete( m_TimeTrigger );
		}

		InletMap inlets = m_IO->getInlets();

		for ( InletBasedTriggerMap::iterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); /**/ )
		{
			InletMap::iterator ioIt = inlets.find( it->first );
			if ( ioIt != inlets.end() )
			{
				ioIt->second->unregisterFromDataReceived( *this );
			}
			
			safeDelete( it->second );
			it = m_InletTriggers.erase( it );
		}

		if ( m_CurrentPolicy->hasTimeBasedTrigger() )
		{
			m_TimeTrigger =  m_CurrentPolicy->getTimeBasedTrigger();
			EngineImpl::instance().getTimer().registerToTimerSignal( *this );
		}

		for ( InletMap::const_iterator it = inlets.begin(); it != inlets.end(); ++it )
		{
			std::string inletName = it->second->getName();
			if (  m_CurrentPolicy->hasTriggerForInlet( inletName ) )
			{
				AbstractInletBasedTrigger *trigger =  m_CurrentPolicy->getTriggerForInlet( inletName );
				it->second->registerToDataReceived( *this );
				m_InletTriggers.insert( std::make_pair( inletName, trigger ) );
			}
		}
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
				resetUberBlockTriggers( false );	//hm - arg should be based on previous state ( if it was waiting )

				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateSetUp();

				FrameworkContext context( dynamic_cast< FunctionBlock& >( m_Owner) );
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
			if ( m_FlaggedForPolicyChange.isSet() )
			{
				changePolicy();
				m_FlaggedForPolicyChange.unset();
			}

			m_StateAccess.lock();
			if ( m_CurrentState->tryStart( *this ) )
			{
				disableAllTriggers();
				resetTriggers();
				resetUberBlockTriggers( false );	//hm - arg should be based on previous state ( if it was waiting )

				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateStarted();
				m_StateAccess.unlock();

				m_StopEvent.reset();
				enableTriggers();

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
				resetTriggers();
				resetUberBlockTriggers( false );	//hm - arg should be based on previous state ( if it was waiting )

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

			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateWaiting();

			m_StateAccess.unlock();

			m_Logger.addLine( std::string( getName() + " new state: waiting" ) );

			resetUberBlockTriggers( false );
			enableUberBlockTriggers();
			Poco::ScopedLock< Poco::FastMutex > lock( m_UberTriggerAccess );
			for ( UberBlockBasedTriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); ++it )
			{
				it->second.second->tryTriggerSubBlock( *this, BLOCK_READY );
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
			resetUberBlockTriggers( false );

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

			m_IO->updateInletValues();			// make sure the inlets have the current data
			m_FunctionBlock->update();			// carry out update
			m_IO->updateOutletValues();			// make sure outlets send their data
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

				FrameworkContext context( dynamic_cast< FunctionBlock& >( m_Owner) );
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

				if ( m_FlaggedForPolicyChange.isSet() )
				{
					changePolicy();
					m_FlaggedForPolicyChange.unset();
				}

				m_CurrentState = new FunctionBlockStateStarted();
				m_StateAccess.unlock();

				m_Logger.addLine( std::string( getName() + " new state: started ( finished update cycle )" ) );

				enableTriggers();
			}

			m_IO->updateInletBuffers();		// ok, so the last thing to do to complete the update is handling data
											// that was received during the update

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
		m_Logger.addLine( std::string( getName() + " new service state: error\n\t" + e.message() ) );
		disableAllTriggers();
		delete m_CurrentState;
		m_CurrentState = new FunctionBlockStateError();
		m_System->handleException( m_Owner, e );
	}

	void FunctionBlockStateManager::uberBlockAdded( AbstractBlock &uberBlock, AbstractUberBlockBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_UberTriggerAccess );

		std::string name = uberBlock.getName();

		UberBlockBasedTriggerMap::iterator it = m_UberTriggers.find( name );
		if ( it != m_UberTriggers.end() )
		{
			delete it->second.first;
			it->second.first = &trigger;
		}
		else
		{
			m_UberTriggers.insert( std::make_pair( name, std::make_pair( &trigger, &uberBlock.getStateManager() ) ) );
		}
	}

	void FunctionBlockStateManager::uberBlockRemoved( AbstractBlock &uberBlock )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_UberTriggerAccess );

		UberBlockBasedTriggerMap::iterator it = m_UberTriggers.find( uberBlock.getName() );
		if ( it != m_UberTriggers.end() )
		{
			delete it->second.first;
			m_UberTriggers.erase( it );
		}
	}

	void FunctionBlockStateManager::tryTriggerInlet( const void *inlet, std::pair< long, long > &times )
	{
		if ( areTriggersEnabled() )
		{
			Inlet const* in = static_cast< Inlet const* >( inlet );

			m_TriggerAccess.lock();
			InletBasedTriggerMap::iterator it = m_InletTriggers.find( in->getName() );
			if ( it != m_InletTriggers.end() && it->second->tryTriggerUpdate( times.first, times.second ) )
			{
				evaluateTriggers();				// access unlocked inside
			}
			else
			{
				m_TriggerAccess.unlock();
			}
		}
	}

	void FunctionBlockStateManager::tryTriggerTime( long &time )
	{
		if ( areTriggersEnabled() )
		{
			m_TriggerAccess.lock();
			if ( m_TimeTrigger != nullptr && m_TimeTrigger->tryTrigger(time) )
			{
				evaluateTriggers();				// access unlocked inside
			}
			else
			{
				m_TriggerAccess.unlock();
			}
		}
	}

	void FunctionBlockStateManager::tryTriggerUberBlock( AbstractStateManager &uber, const BlockMessage msg )
	{
		if ( areUberBlockTriggersEnabled() )
		{
			m_UberTriggerAccess.lock();
			UberBlockBasedTriggerMap::iterator it = m_UberTriggers.find( uber.getName() );
			if ( it != m_UberTriggers.end() && it->second.first->tryTriggerUpdate( msg ) )
			{
				evaluateUberBlockTriggers();	// access unlocked inside
			}
			else
			{
				m_UberTriggerAccess.unlock();
			}
		}
	}

	void FunctionBlockStateManager::evaluateTriggers()
	{
		bool triggersOk = true;
		if ( m_TimeTrigger != nullptr )
		{
			triggersOk &= m_TimeTrigger->isFullfilled();
		}
		for ( InletBasedTriggerMap::iterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); ++it )
		{
			triggersOk &= it->second->isFullfilled();
		}
		//if ( triggersOk ) disableTriggers();
		m_TriggerAccess.unlock();	// was locked from within calling function

		if (triggersOk)
		{
			triggersAreOk();	// request state change
		}
	}

	void FunctionBlockStateManager::evaluateUberBlockTriggers()
	{
		bool triggersOk = true;
		for ( UberBlockBasedTriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); ++it )
		{
			triggersOk &= it->second.first->isFullfilled();
		}
		//if ( triggersOk ) disableUberBlockTriggers();
		m_UberTriggerAccess.unlock();	// was locked from within calling function

		if (triggersOk)
		{
			uberBlocksAreOk();	//request state change
		}
	}

	void FunctionBlockStateManager::resetTriggers()
	{
		m_TriggerAccess.lock();
		if ( m_TimeTrigger != nullptr )
		{
			m_TimeTrigger->reset();
		}
		for ( InletBasedTriggerMap::iterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); ++it )
		{
			it->second->reset();
		}
		m_TriggerAccess.unlock();
	}

	void FunctionBlockStateManager::resetUberBlockTriggers( const bool notify )
	{
		m_UberTriggerAccess.lock();
		for ( UberBlockBasedTriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); ++it )
		{
			it->second.first->reset();
		}
		m_UberTriggerAccess.unlock();
	}

	bool FunctionBlockStateManager::areTriggersEnabled() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		return m_TriggersEnabled;
	}

	bool FunctionBlockStateManager::areUberBlockTriggersEnabled() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		return m_UberTriggersEnabled;
	}

	void FunctionBlockStateManager::disableAllTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_TriggersEnabled = false;
		m_UberTriggersEnabled = false;
	}

	void FunctionBlockStateManager::disableTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_TriggersEnabled = false;
	}

	void FunctionBlockStateManager::disableUberBlockTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_UberTriggersEnabled = false;
	}

	void FunctionBlockStateManager::enableAllTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_TriggersEnabled = true;
		m_UberTriggersEnabled = true;
	}

	void FunctionBlockStateManager::enableTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_TriggersEnabled = true;
	}

	void FunctionBlockStateManager::enableUberBlockTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_EnabledAccess );
		m_UberTriggersEnabled = true;
	}

}