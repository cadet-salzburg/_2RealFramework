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

#include "engine/_2RealFunctionBlockUpdateManager.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealFunctionBlockStateManager.h"
#include "policies/_2RealUpdatePolicy.h"
#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealAbstractDataCondition.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealLogger.h"
#include "engine/_2RealTimer.h"
#include "engine/_2RealFunctionBlock.h"

namespace _2Real
{

	FunctionBlockUpdateManager::FunctionBlockUpdateManager( EngineImpl *engine, FunctionBlock *owner ) :
		AbstractUpdateManager(),
		mEngineImpl( engine ),
		mOwningBlock( owner ),
		mAreInletsOk( false ),
		mIsTimerOk( false ),
		mAreUpdatesEnabled( false )
	{
	}

	FunctionBlockUpdateManager::~FunctionBlockUpdateManager()
	{
		if ( mUpdateTimer.get() != nullptr )
		{
			AbstractCallback< long > *cb = new MemberCallback< FunctionBlockUpdateManager, long >( *this, &FunctionBlockUpdateManager::timerSignal );
			mUpdateTimer->unregisterFromTimerSignal( cb );
		}
	}

	std::string const& FunctionBlockUpdateManager::getFullHumanReadableName() const
	{
		return mOwningBlock->getFullHumanReadableName();
	}

	std::string const& FunctionBlockUpdateManager::getHumanReadableName() const
	{
		return mOwningBlock->getHumanReadableName();
	}

	//std::string const& FunctionBlockUpdateManager::getCode() const
	//{
	//	return mOwningBlock->getCode();
	//}

	void FunctionBlockUpdateManager::inletAdded( std::shared_ptr< BasicInletIO > inlet, UpdatePolicy const& policy )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );

		std::shared_ptr< ADataCondition > cond;

		if ( policy == UpdatePolicy::VALID_DATA )
			cond.reset( new DataCondition< ValidData >() );
		else if ( policy == UpdatePolicy::NEWER_DATA )
			cond.reset( new DataCondition< NewerTimestamp >() );
		else return;

		mInlets[ inlet ] = cond;
	}

	void FunctionBlockUpdateManager::inletRemoved( std::shared_ptr< BasicInletIO > inlet )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );

		std::map< std::shared_ptr< BasicInletIO >, std::shared_ptr< ADataCondition > >::iterator it = mInlets.find( inlet );
		if ( it != mInlets.end() )	mInlets.erase( it );
	}

	void FunctionBlockUpdateManager::policyChanged( std::shared_ptr< BasicInletIO > inlet, UpdatePolicy const& policy )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );

		std::map< std::shared_ptr< BasicInletIO >, std::shared_ptr< ADataCondition > >::iterator it = mInlets.find( inlet );
		if ( it != mInlets.end() )
		{
			std::shared_ptr< ADataCondition > cond;

			if ( policy == UpdatePolicy::VALID_DATA )
				cond.reset( new DataCondition< ValidData >() );
			else if ( policy == UpdatePolicy::NEWER_DATA )
				cond.reset( new DataCondition< NewerTimestamp >() );
			else return;

			it->second = cond;
		}
	}

	void FunctionBlockUpdateManager::setUpdateTimer( std::shared_ptr< Timer > timer )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mAccess );

		if ( mUpdateTimer.get() != nullptr )
		{
			AbstractCallback< long > *cb = new MemberCallback< FunctionBlockUpdateManager, long >( *this, &FunctionBlockUpdateManager::timerSignal );
			mUpdateTimer->unregisterFromTimerSignal( cb );
		}

		AbstractCallback< long > *cb = new MemberCallback< FunctionBlockUpdateManager, long >( *this, &FunctionBlockUpdateManager::timerSignal );
		timer->registerToTimerSignal( cb );
		mUpdateTimer = timer;
	}

	void FunctionBlockUpdateManager::enableUpdates( const bool enabled )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mUpdateAccess );
		mAreUpdatesEnabled = enabled;
	}

	bool FunctionBlockUpdateManager::areUpdatesEnabled() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mUpdateAccess );
		return mAreUpdatesEnabled;
	}

	void FunctionBlockUpdateManager::timerSignal( long )
	{
		mAccess.lock();
		mEngineImpl->getLogger()->addLine( getFullHumanReadableName() + " received timer signal" );
		if ( areUpdatesEnabled() )
		{
			if ( mAreInletsOk || areInletsOk() )
			{
				enableUpdates( false );
				resetInlets();
				mIsTimerOk = false;
				mAreInletsOk = false;
				mAccess.unlock();

				mEngineImpl->getLogger()->addLine( getFullHumanReadableName() + " scheduling update" );
				mStateManager.lock()->scheduleUpdate();
			}
			else
			{
				mIsTimerOk = true;
				mAccess.unlock();
			}
		}
		else
			mAccess.unlock();
	}

	void FunctionBlockUpdateManager::inletChanged()
	{
		mEngineImpl->getLogger()->addLine( getFullHumanReadableName() + " received inlet signal" );

		mAccess.lock();
		if ( areUpdatesEnabled() )
		{
			if ( areInletsOk() && ( mIsTimerOk || mUpdateTimer == mEngineImpl->getTimerManager()->getNullTimer() ) )
			{
				enableUpdates( false );
				resetInlets();
				mIsTimerOk = false;
				mAreInletsOk = false;
				mAccess.unlock();

				mEngineImpl->getLogger()->addLine( getFullHumanReadableName() + " scheduling update" );
				mStateManager.lock()->scheduleUpdate();
			}
			else
			{
				mAreInletsOk = true;
				mAccess.unlock();
			}
		}
		else
			mAccess.unlock();
	}

	bool FunctionBlockUpdateManager::areInletsOk()
	{
		bool ok = false;
		bool canFulfill = true;

		for ( std::map< std::shared_ptr< BasicInletIO >, std::shared_ptr< ADataCondition > >::iterator it = mInlets.begin(); it != mInlets.end(); ++it )
			canFulfill &= ( it->first->canFulfillCondition( it->second ) );

		if ( canFulfill )
		{
			ok = true;
			for ( std::map< std::shared_ptr< BasicInletIO >, std::shared_ptr< ADataCondition > >::iterator it = mInlets.begin(); it != mInlets.end(); ++it )
				ok &= ( it->first->tryFulfillCondition( it->second ) );
		}

		return ok;
	}

	void FunctionBlockUpdateManager::resetInlets()
	{
		for ( std::map< std::shared_ptr< BasicInletIO >, std::shared_ptr< ADataCondition > >::iterator it = mInlets.begin(); it != mInlets.end(); ++it )
			it->second->setFulfilled( false );
	}
}
