 /*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2012 Fachhochschule Salzburg GmbH

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

#pragma once

#include "ZmqNetworking.h"
#include "_2RealApplication.h"

namespace _2Real
{
	namespace network
	{
		/*
		*	simple asynchronous subscriber; accepts multiple listeners
		*/
		class SubscriberBlockWrapper
		{

		public:

			typedef boost::signals2::signal< void( std::shared_ptr< const DataItem > ) >::slot_type DataCallback;
		
			SubscriberBlockWrapper( SubscriberBlockWrapper const& ) = delete;
			SubscriberBlockWrapper( SubscriberBlockWrapper && ) = delete;

			static std::pair< std::shared_ptr< SubscriberBlockWrapper >, _2Real::Connection > create( std::string const& address, std::string const& topic, DataCallback, _2Real::app::Engine &, _2Real::app::ThreadpoolHandle, bool start = true );
			static std::shared_ptr< SubscriberBlockWrapper > create( std::string const& address, std::string const& topic, _2Real::app::Engine &, _2Real::app::ThreadpoolHandle, bool start = true );

			_2Real::Connection registerToData( DataCallback );

			// these actually start & stop the underlying block
			void startListening();
			void stopListening();

		private:

			static std::shared_ptr< SubscriberBlockWrapper > init( std::string const& address, std::string const& topic, _2Real::app::Engine &, _2Real::app::ThreadpoolHandle );

			SubscriberBlockWrapper( _2Real::app::BlockHandle, _2Real::app::TimerHandle );
			~SubscriberBlockWrapper() = default;

			_2Real::app::BlockHandle			mUnderlyingBlock;
			_2Real::app::TimerHandle			mUpdateTimer;

			struct Deleter
			{
				void operator()( SubscriberBlockWrapper *& sub );
			};

		};

		//class SubscriberWithFuture
		//{

		//public:

		//	SubscriberWithFuture( SubscriberWithFuture const& ) = delete;
		//	SubscriberWithFuture( SubscriberWithFuture && ) = delete;

		//	static std::shared_ptr< SubscriberWithFuture > create( std::string const& address, std::string const& topic, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
		//	{
		//		SubscriberWithFuture::Deleter deleter;
		//		std::shared_ptr< SubscriberWithFuture > result( new SubscriberWithFuture, deleter );
		//		auto callback = std::bind( &_2Real::network::SubscriberWithFuture::received, result.get(), std::placeholders::_1 );
		//		auto res = SubscriberBlockWrapper::create( address, topic, callback, engine, threadpool );
		//		result->mSubscriber = res.first;
		//		result->mConnection = res.second;
		//		return result;
		//	}

		//	std::future< std::shared_ptr< const _2Real::DataItem > > nextItem()
		//	{
		//		std::lock_guard< std::mutex > lock( mLock );
		//		mQueue.push_front( std::promise< std::shared_ptr< const _2Real::DataItem > >() );
		//		return mQueue.front().get_future();
		//	}

		//private:

		//	SubscriberWithFuture() = default;
		//	~SubscriberWithFuture() = default;

		//	std::shared_ptr< SubscriberBlockWrapper >												mSubscriber;
		//	_2Real::Connection															mConnection;
		//	std::deque< std::promise< std::shared_ptr< const _2Real::DataItem > > >		mQueue;
		//	mutable std::mutex															mLock;

		//	void received( std::shared_ptr< const _2Real::DataItem > data )
		//	{
		//		assert( data.get() );

		//		std::lock_guard< std::mutex > lock( mLock );

		//		if ( mQueue.empty() )		// no-one cares!
		//			return;

		//		mQueue.back().set_value( data );
		//		mQueue.pop_back();
		//	}

		//	struct Deleter
		//	{
		//		void operator()( SubscriberWithFuture *& sub )
		//		{
		//			if ( !sub ) return;

		//			if ( sub->mConnection.connected() )
		//				sub->mConnection.disconnect();

		//			sub->mSubscriber.reset();	// shut down the underlying async subscriber

		//			{
		//				std::lock_guard< std::mutex > lock( sub->mLock );
		//				//for ( auto &promise : sub->mQueue )
		//				//{
		//				//	promise.set_exception( std::exception( "subscriber was shut down" ) );
		//				//}
		//				sub->mQueue.clear();
		//			}

		//			delete sub;
		//			sub = nullptr;
		//		}
		//	};
		//};
	}
}
