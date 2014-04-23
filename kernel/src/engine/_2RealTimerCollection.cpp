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

#include "engine/_2RealTimerCollection.h"
#include "engine/_2RealTimerImpl.h"

namespace _2Real
{
	class TimerByAddress
	{
	public:
		explicit TimerByAddress( std::shared_ptr< const TimerImpl > timer ) : mBaseline( timer ) { assert( timer.get() ); }
		bool operator()( std::pair< Connection, std::shared_ptr< TimerImpl > > const& val ) const
		{
			assert( val.second.get() );
			return mBaseline.get() == val.second.get();
		}
	private:
		std::shared_ptr< const TimerImpl > mBaseline;
	};

	TimerCollection::TimerCollection() :
		mIoService(),
		mWork( new boost::asio::io_service::work( mIoService ) ),
		mThread( &TimerCollection::run, this )
	{		
	}

	TimerCollection::~TimerCollection()
	{
		mWork.reset();
		mThread.join();
	}

	void TimerCollection::clear()
	{
		for ( auto it : mTimers )
		{
			it.first.disconnect();
			it.second->stop();
		}

		mTimers.clear();
	}

	void TimerCollection::run()
	{
		mIoService.run();
	}

	std::shared_ptr< TimerImpl > TimerCollection::createTimer( const double fps )
	{
		uint64_t period = static_cast< uint64_t >( 1000 / fps );
		std::shared_ptr< TimerImpl > timer( new TimerImpl( mIoService, period ) );
		Connection connection = timer->registerToDestroyed( std::bind( &TimerCollection::timerDestroyed, this, std::placeholders::_1 ) );
		mTimers.push_back( std::make_pair( connection, timer ) );
		return timer;
	}

	void TimerCollection::timerDestroyed( std::shared_ptr< const TimerImpl > timer )
	{
		auto timerIter = std::find_if( mTimers.begin(), mTimers.end(), TimerByAddress( timer ) );
		if ( timerIter != mTimers.end() )
		{
			timerIter->first.disconnect();
			mTimers.erase( timerIter );
		}
	}
}
