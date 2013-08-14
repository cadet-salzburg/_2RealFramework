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
#include "engine/_2RealFunctionBlockUpdateManager.h"
#include "engine/_2RealFunctionBlockIOManager.h"
#include "engine/_2RealFunctionBlock.h"
#include "engine/_2RealFunctionBlockState.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealThreadPool.h"
#include "engine/_2RealLogger.h"
#include "engine/_2RealFunctionBlock.h"
#include "../_2RealBlock.h"

using std::string;

namespace _2Real
{

	FunctionBlockStateManager::FunctionBlockStateManager( EngineImpl *engine, FunctionBlock *owner, const bool thread ) :
		AbstractStateManager(),
		mEngineImpl( engine ),
		mOwningBlock( owner ),
		m_CurrentState( new FunctionBlockStateCreated() ),
		m_IsFlaggedForHalting( false ),
		m_IsFlaggedForShutdown( false ),
		m_Thread( nullptr ),
		m_HasException( false ),
		m_Exception( "" )
	{
		if ( thread ) m_Thread = mEngineImpl->getThreadpool()->requestUniqueThread();
	}

	FunctionBlockStateManager::~FunctionBlockStateManager()
	{
		delete m_CurrentState;
	}

	std::string const& FunctionBlockStateManager::getFullHumanReadableName() const
	{
		return mOwningBlock->getFullHumanReadableName();
	}

	std::string const& FunctionBlockStateManager::getHumanReadableName() const
	{
		return mOwningBlock->getHumanReadableName();
	}

	//std::string const& FunctionBlockStateManager::getCode() const
	//{
	//	return mOwningBlock->getCode();
	//}

	void FunctionBlockStateManager::handleStateChangeException( Exception &e )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		std::cout << "-------------------------------------------------------------------" << std::endl;
		std::cout << getFullHumanReadableName() << " EXCEPTION: " << e.message() << std::endl;
		std::cout << "-------------------------------------------------------------------" << std::endl;

		mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " state: error\n\t" + e.message() ) );

		m_HasException = true;
		m_Exception = e;
	}

	//bool FunctionBlockStateManager::isRunning() const
	//{
	//	Poco::ScopedLock< Poco::FastMutex > lock( m_StateAccess );
	//	return ( *m_CurrentState == AbstractFunctionBlockState::UPDATING );
	//}

	void FunctionBlockStateManager::setUp()
	{
		m_StateAccess.lock();

		// created: ok
		// initialized: ok
		// excpetion: ok
		// else: throw exception
		m_CurrentState->setUp( *this );

		// blocking request /////////////////////////////////////
		ThreadExecRequest *req = new ThreadExecRequest( *this, &FunctionBlockStateManager::setupFunctionBlock );
		Poco::Event *ev = new Poco::Event();
		req->event = ev;
		mEngineImpl->getThreadpool()->scheduleRequest( *req, m_Thread );
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

			mOwningBlock->handleException( m_Exception );
		}
		else
		{
			// state change /////////////////////////////////////////
			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateInitialized();
			mEngineImpl->getLogger()->addLine( string( getFullHumanReadableName() + " state: initialized" ) );
			m_StateAccess.unlock();
			// state change /////////////////////////////////////////
		}
	}

	void FunctionBlockStateManager::setupFunctionBlock()
	{
		// called by pooled thread

		try
		{
			//mIOManager->updateParameterData();
			bundle::BlockHandle handle( mIOManager.lock() );
			mFunctionBlock.lock()->setup( handle );
			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " carried out setup" ) );
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
		mEngineImpl->getLogger()->addLine( string( getFullHumanReadableName() + " new state: waiting" ) );
		m_CurrentState = new FunctionBlockStateWaiting();
		// state change /////////////////////////////////////////

		m_StateAccess.unlock();

		m_StopEvent.reset();
		mUpdateManager.lock()->enableUpdates( true );
	}

	void FunctionBlockStateManager::scheduleUpdate()
	{
		m_StateAccess.lock();

		// waiting: ok
		// else: throw exception
		if ( m_CurrentState->scheduleUpdate( *this ) )
		{

			// state change /////////////////////////////////////////
			delete m_CurrentState;
			mEngineImpl->getLogger()->addLine( string( getFullHumanReadableName() + " new state: updating" ) );
			m_CurrentState = new FunctionBlockStateUpdating();
			// state change /////////////////////////////////////////

			ThreadExecRequest *req = new ThreadExecRequest( *this, &FunctionBlockStateManager::updateFunctionBlock );
			mEngineImpl->getThreadpool()->scheduleRequest( *req, m_Thread );
			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " scheduled update request" ) );

			m_StateAccess.unlock();
		}
		else
		{
			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + "rec update request in invalid state" ) );
			m_StateAccess.unlock();
		}
	}

	void FunctionBlockStateManager::updateFunctionBlock()
	{
		try
		{
			mIOManager.lock()->updateParameterData();
			mIOManager.lock()->updateInletData();
			mFunctionBlock.lock()->update();
			mIOManager.lock()->updateOutletData();

			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " carried out update" ) );
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
			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " new state: stopped ( finished update cycle )" ) );

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

			mOwningBlock->handleException( m_Exception );
		}
		else if ( m_IsFlaggedForHalting.isSet() )
		{
			m_StateAccess.lock();

			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateInitialized();
			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " new state: initialized ( finished update cycle )" ) );

			m_StopEvent.set();
			m_IsFlaggedForHalting.unset();

			m_StateAccess.unlock();
		}
		else
		{
			m_StateAccess.lock();

			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateWaiting();
			m_StateAccess.unlock();

			mUpdateManager.lock()->enableUpdates( true );
		}
	}

	//void FunctionBlockStateManager::singleStep()
	//{
	//	//m_StateAccess.lock();

	//	//// initialized: ok
	//	//// else: throw exception
	//	//m_CurrentState->start( *this );

	//	///////////////////////////////////////////////////////////

	//	//// blocking request /////////////////////////////////////
	//	//ThreadExecRequest *req = new ThreadExecRequest( *this, &FunctionBlockStateManager::singleStepFunctionBlock );
	//	//Poco::Event *ev = new Poco::Event();
	//	//req->event = ev;
	//	//mEngineImpl->getThreadpool()->scheduleRequest( *req, m_Thread );
	//	//ev->wait();
	//	//delete ev;
	//	//// blocking request /////////////////////////////////////

	//	////

	//	//if ( m_HasException )
	//	//{
	//	//	delete m_CurrentState;
	//	//	m_CurrentState = new FunctionBlockStateError();
	//	//	m_StateAccess.unlock();

	//	//	mOwningBlock->handleException( m_Exception );
	//	//}
	//	//else
	//	//{
	//	//	m_StateAccess.unlock();
	//	//}
	//}

	//void FunctionBlockStateManager::singleStepFunctionBlock()
	//{
	//	try
	//	{
	//		//mIOManager->updateInletData();
	//		//mIOManager->updateParameterData();
	//		mFunctionBlock.lock()->update();
	//		//mIOManager->updateOutletData();

	//		mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " carried out singlestep" ) );
	//	}
	//	catch ( Exception &e )
	//	{
	//		handleStateChangeException( e );
	//	}
	//}

	void FunctionBlockStateManager::prepareForShutDown()
	{
		m_StateAccess.lock();
		if ( m_CurrentState->tryStop( *this ) )
		{
			// state change /////////////////////////////////////////
			delete m_CurrentState;
			m_CurrentState = new FunctionBlockStateStopped();
			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " state: stopped ( framework stop )" ) );
			// state change /////////////////////////////////////////

			m_StopEvent.set();
			m_ShutdownEvent.set();
		}
		else
		{
			m_IsFlaggedForShutdown.set();
			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " flagged for shutdown" ) );
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
			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " state: initialized ( user stop )" ) );
			// state change /////////////////////////////////////////

			m_StopEvent.set();
		}
		else
		{
			m_IsFlaggedForHalting.set();
			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " flagged for halting" ) );
		}

		m_StateAccess.unlock();
		return m_StopEvent;
	}

	bool FunctionBlockStateManager::shutDown( const long timeout )
	{
		mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + "--------------------" ) );

		if ( m_ShutdownEvent.tryWait( timeout ) )
		{
			m_StateAccess.lock();

			// blocking request /////////////////////////////////////
			ThreadExecRequest *req = new ThreadExecRequest( *this, &FunctionBlockStateManager::shutdownFunctionBlock );
			Poco::Event *ev = new Poco::Event();
			req->event = ev;

			mEngineImpl->getThreadpool()->scheduleRequest( *req, m_Thread );

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

				mOwningBlock->handleException( m_Exception );
			}
			else
			{
				// state change /////////////////////////////////////////
				delete m_CurrentState;
				m_CurrentState = new FunctionBlockStateShutDown();
				mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " state: shut down" ) );
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
			mFunctionBlock.lock()->shutdown();
			mEngineImpl->getLogger()->addLine( std::string( getFullHumanReadableName() + " carried out shutdown" ) );
		}
		catch ( Exception &e )
		{
			handleStateChangeException( e );
		}
	}
}
