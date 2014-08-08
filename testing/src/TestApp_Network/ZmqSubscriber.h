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
#include <deque>

namespace _2Real
{
	namespace network
	{
		/*
		*	simple asynchronous subscriber; accepts multiple listeners
		*/
		class Subscriber
		{

		public:

			typedef boost::signals2::signal< void( std::shared_ptr< const DataItem > ) >::slot_type DataCallback;
		
			Subscriber( Subscriber const& ) = delete;
			Subscriber( Subscriber && ) = delete;

			static std::pair< std::shared_ptr< Subscriber >, _2Real::Connection > create( std::string const& address, std::string const& topic, DataCallback, _2Real::app::Engine &, _2Real::app::ThreadpoolHandle, bool start = true );
			static std::shared_ptr< Subscriber > create( std::string const& address, std::string const& topic, _2Real::app::Engine &, _2Real::app::ThreadpoolHandle, bool start = true );

			_2Real::Connection registerToData( DataCallback );

			// these actually start & stop the underlying block
			void startListening();
			void stopListening();

		private:

			static std::shared_ptr< Subscriber > init( std::string const& address, std::string const& topic, _2Real::app::Engine &, _2Real::app::ThreadpoolHandle );

			Subscriber( _2Real::app::BlockHandle, _2Real::app::TimerHandle );
			~Subscriber() = default;

			_2Real::app::BlockHandle			mUnderlyingBlock;
			_2Real::app::TimerHandle			mUpdateTimer;
			std::deque< _2Real::Connection >	mConnections;

			struct Deleter
			{
				void operator()( Subscriber *& sub );
			};

		};

		class SubscriberWithQueue
		{

		public:

			SubscriberWithQueue( SubscriberWithQueue const& ) = delete;
			SubscriberWithQueue( SubscriberWithQueue && ) = delete;

			static std::shared_ptr< SubscriberWithQueue > create( std::string const& address, std::string const& topic, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
			{
				SubscriberWithQueue::Deleter deleter;
				std::shared_ptr< SubscriberWithQueue > result( new SubscriberWithQueue, deleter );
				auto callback = std::bind( &_2Real::network::SubscriberWithQueue::received, result.get(), std::placeholders::_1 );
				auto res = Subscriber::create( address, topic, callback, engine, threadpool );
				result->mSubscriber = res.first;
				result->mConnection = res.second;
				result->mName = topic;
				return result;
			}

			std::shared_ptr< const DataItem > getNewestDataItem()
			{
				std::lock_guard< std::mutex > lock( mLock );
				auto result = mQueue.front(); mQueue.pop_front();
				return result;
			}

			std::shared_ptr< const DataItem > getOldestDataItem()
			{
				std::lock_guard< std::mutex > lock( mLock );
				auto result = mQueue.back(); mQueue.pop_back();
				return result;
			}

			bool empty() const
			{
				std::lock_guard< std::mutex > lock( mLock );
				return mQueue.empty();
			}

			uint32_t size() const
			{
				std::lock_guard< std::mutex > lock( mLock );
				return mQueue.size();
			}

		private:

			SubscriberWithQueue() = default;
			~SubscriberWithQueue() = default;

			std::shared_ptr< Subscriber >										mSubscriber;
			_2Real::Connection													mConnection;
			std::deque< std::shared_ptr< const _2Real::DataItem > >				mQueue;
			mutable std::mutex													mLock;
			std::string															mName;

			void received( std::shared_ptr< const _2Real::DataItem > data )
			{
				assert( data.get() );

				//std::cout << mName << " received data" << std::endl;

				std::lock_guard< std::mutex > lock( mLock );
				mQueue.push_front( data );
			}

			struct Deleter
			{
				void operator()( SubscriberWithQueue *& sub )
				{
					if ( !sub ) return;

					if ( sub->mConnection.connected() )
						sub->mConnection.disconnect();

					sub->mSubscriber.reset();	// shut down the underlying async subscriber

					{
						std::lock_guard< std::mutex > lock( sub->mLock );
						sub->mQueue.clear();
					}

					delete sub;
					sub = nullptr;
				}
			};

		};

		class SubscriberWithFuture
		{

		public:

			SubscriberWithFuture( SubscriberWithFuture const& ) = delete;
			SubscriberWithFuture( SubscriberWithFuture && ) = delete;

			static std::shared_ptr< SubscriberWithFuture > create( std::string const& address, std::string const& topic, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
			{
				SubscriberWithFuture::Deleter deleter;
				std::shared_ptr< SubscriberWithFuture > result( new SubscriberWithFuture, deleter );
				auto callback = std::bind( &_2Real::network::SubscriberWithFuture::received, result.get(), std::placeholders::_1 );
				auto res = Subscriber::create( address, topic, callback, engine, threadpool );
				result->mSubscriber = res.first;
				result->mConnection = res.second;
				return result;
			}

			std::future< std::shared_ptr< const _2Real::DataItem > > nextItem()
			{
				std::lock_guard< std::mutex > lock( mLock );
				mQueue.push_front( std::promise< std::shared_ptr< const _2Real::DataItem > >() );
				return mQueue.front().get_future();
			}

		private:

			SubscriberWithFuture() = default;
			~SubscriberWithFuture() = default;

			std::shared_ptr< Subscriber >												mSubscriber;
			_2Real::Connection															mConnection;
			std::deque< std::promise< std::shared_ptr< const _2Real::DataItem > > >		mQueue;
			mutable std::mutex															mLock;

			void received( std::shared_ptr< const _2Real::DataItem > data )
			{
				assert( data.get() );

				std::lock_guard< std::mutex > lock( mLock );

				if ( mQueue.empty() )		// no-one cares!
					return;

				mQueue.back().set_value( data );
				mQueue.pop_back();
			}

			struct Deleter
			{
				void operator()( SubscriberWithFuture *& sub )
				{
					if ( !sub ) return;

					if ( sub->mConnection.connected() )
						sub->mConnection.disconnect();

					sub->mSubscriber.reset();	// shut down the underlying async subscriber

					{
						std::lock_guard< std::mutex > lock( sub->mLock );
						//for ( auto &promise : sub->mQueue )
						//{
						//	promise.set_exception( std::exception( "subscriber was shut down" ) );
						//}
						sub->mQueue.clear();
					}

					delete sub;
					sub = nullptr;
				}
			};
		};
	}
}
