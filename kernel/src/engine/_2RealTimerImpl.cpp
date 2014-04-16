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

#include "engine/_2RealTimerImpl.h"

#include <boost/bind.hpp>

namespace _2Real
{
	TimerImpl::TimerImpl( boost::asio::io_service &service, const uint64_t period ) :
		UpdateTrigger_I(),
		mPeriod( period ),
		mTimer( service ),
		mShouldUpdate( false )
	{
	}

	TimerImpl::~TimerImpl()
	{
	}

	void TimerImpl::start()
	{
		mMutex.lock();
		if ( !mShouldUpdate )
		{
			mShouldUpdate = true;
			mMutex.unlock();

			mTimer.expires_from_now( boost::posix_time::milliseconds( mPeriod ) );
			mTimer.async_wait( boost::bind( &TimerImpl::update, this ) );
		}
		else
			mMutex.unlock();
	}

	void TimerImpl::stop()
	{
		std::lock_guard< std::mutex > lock( mMutex );
		mShouldUpdate = false;
		mTimer.cancel();
	}

	void TimerImpl::update()
	{
		mMutex.lock();
		if ( mShouldUpdate )
		{
			mEvent.notify();
			mMutex.unlock();

			mTimer.expires_at( mTimer.expires_at() + boost::posix_time::milliseconds( mPeriod ) );
			mTimer.async_wait( boost::bind( &TimerImpl::update, this ) );
		}
		else
			mMutex.unlock();
	}

	void TimerImpl::registerToUpdate( std::shared_ptr< AbstractCallback_T< void > > cb )
	{
		mEvent.addListener( cb );
	}

	void TimerImpl::unregisterFromUpdate( std::shared_ptr< AbstractCallback_T< void > > cb )
	{
		mEvent.removeListener( cb );
	}

}