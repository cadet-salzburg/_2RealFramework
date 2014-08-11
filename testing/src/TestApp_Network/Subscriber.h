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

#include "common/_2RealSignals.h"
#include "engine/_2RealHumanReadableNameVisitor.h"

#include "ZmqSubscriberWrapper.h"
#include "BsonDeserializer.h"

namespace _2Real
{
	namespace network
	{
		/*
		*	there is no 'generic' subscriber, that is, one that just extracts any kind of data & calls a callback
		*	this is so, because to do deserialize ( or at least attempt to deserialize ) 'any' type, I'd need to access
		*	the engine to get the typemetainfo - in an asynchronous callback. this could lead to read write conflicts
		*	( engine install bundle with custim types while typemetainfo is queried ), as the engine was not designed
		*	to be accessed in multiple threads.
		*/

		/*
		*	basic async subscribers: expect callbacks, will call these.
		*/

		/*
		*	async subscriber, subscribes to address & topic; also expects a callback which accepts the type template parameter
		*	on receiving data, attempts to deserialize the template parameter's type -> data which does not fit will be ignored
		*	if the received data fits the template parameter's type, deserialization should succeed, and the callback will be invoked
		*	make sure the template parameter type actually is a framework supported type! ( creation will fail otherwise )
		*/

		template< typename TType >
		class Deserializer_T
		{
		public:

			Deserializer_T( std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) : mTypeMetainfo( info ), mExpectedType( info->getName() )
			{
			}

			TType extract( std::shared_ptr< const _2Real::DataItem > dataItem )
			{
				auto deserializer = _2Real::io::bson::Deserializer::create( dataItem, _2Real::network::Constants::MaxTopicNameLength );
				if ( deserializer->getTypename() != mExpectedType )
				{
					std::cout << "type mismatch " << mExpectedType << " " << deserializer->getTypename() << std::endl;
					throw TypeMismatch( "..." );
				}

				auto deserialized = deserializer->getDataItem( mTypeMetainfo );
				TType const& extracted = boost::get< TType const& >( *deserialized.get() );
				return extracted;
			}

		private:

			std::shared_ptr< const _2Real::app::TypeMetainfo_I >	mTypeMetainfo;
			std::string												mExpectedType;

		};

		template< >
		class Deserializer_T< _2Real::CustomDataItem >
		{
		public:

			Deserializer_T( std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) : mTypeMetainfo( info ), mExpectedType( info->getName() )
			{
			}

			_2Real::CustomDataItem extract( std::shared_ptr< const _2Real::DataItem > dataItem )
			{
				auto deserializer = _2Real::io::bson::Deserializer::create( dataItem, _2Real::network::Constants::MaxTopicNameLength );
				if ( deserializer->getTypename() != mExpectedType )
				{
					std::cout << "type mismatch " << mExpectedType << " " << deserializer->getTypename() << std::endl;
					throw TypeMismatch( "..." );
				}

				auto deserialized = deserializer->getDataItem( mTypeMetainfo );
				_2Real::CustomDataItem const& extracted = boost::get< _2Real::CustomDataItem const& >( *deserialized.get() );
				return extracted;
			}

		private:

			std::shared_ptr< const _2Real::app::TypeMetainfo_I >	mTypeMetainfo;
			std::string												mExpectedType;

		};

		template< typename TType >
		class AsyncSubscriber_T
		{

		public:

			typedef typename boost::signals2::signal< void( const TType ) >::slot_type ReceivedCallback;
		
			AsyncSubscriber_T( AsyncSubscriber_T< TType > const& ) = delete;
			AsyncSubscriber_T( AsyncSubscriber_T< TType > && ) = delete;

			static std::pair< std::shared_ptr< AsyncSubscriber_T< TType > >, _2Real::Connection > create( std::string const& address, std::string const& topic, ReceivedCallback callback, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
			{
				_2Real::HumanReadableNameVisitor name;

				// TODO: get name for a certain type, not just an object ( also, this causes a shitty warning and does not compile in debug mode )
				TType *dummy = new TType;
				std::string expectedType = name( *dummy );
				auto info = engine.getTypeMetainfo( expectedType );		// may throw NotFound

				// create basic subscriber ( not running ) & wrap it
				auto basicSubscriber = SubscriberBlockWrapper::create( address, topic, engine, threadpool, false );
				std::shared_ptr< AsyncSubscriber_T< TType > > result( new AsyncSubscriber_T< TType >( basicSubscriber, info ), AsyncSubscriber_T< TType >::Deleter() );
				// connect & start basic subscriber
				auto connection = result->mReceivedSignal.connect( callback );
				basicSubscriber->startListening();

				return std::make_pair( result, connection );
			}

		private:

			AsyncSubscriber_T( std::shared_ptr< SubscriberBlockWrapper > subscriber, std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) : mUnderlyingBasicSubscriber( subscriber ), mDeserializer( info )
			{
				auto callback = std::bind( &_2Real::network::AsyncSubscriber_T< TType >::received, this, std::placeholders::_1 );
				mConnectionToBasicSubscriber = subscriber->registerToData( callback );
			}

			~AsyncSubscriber_T() = default;

			void received( std::shared_ptr< const _2Real::DataItem > data )
			{
				assert( data.get() );

				try
				{
					mReceivedSignal( mDeserializer.extract( data ) );
				}
				catch ( TypeMismatch &e )
				{
					std::cout << e.what() << std::endl;
				}
			}

			std::shared_ptr< _2Real::network::SubscriberBlockWrapper >			mUnderlyingBasicSubscriber;
			_2Real::Connection													mConnectionToBasicSubscriber;
			boost::signals2::signal< void( const TType ) >						mReceivedSignal;
			Deserializer_T< TType >												mDeserializer;

			struct Deleter
			{
				void operator()( AsyncSubscriber_T< TType > *& sub )
				{
					if ( sub->mConnectionToBasicSubscriber.connected() )
						sub->mConnectionToBasicSubscriber.disconnect();

					sub->mReceivedSignal.disconnect_all_slots();

					sub->mUnderlyingBasicSubscriber.reset();

					delete sub;
					sub = nullptr;
				}
			};
		};

		/*
		*	just like the above, only with CustomTypes
		*	since CustomTypes are in essence just one class, the expected typename must be given when creating the subscriber
		*/
		template< >
		class AsyncSubscriber_T< _2Real::CustomDataItem >
		{

		public:

			typedef boost::signals2::signal< void( _2Real::CustomDataItem const& ) >::slot_type ReceivedCallback;
		
			AsyncSubscriber_T( AsyncSubscriber_T< _2Real::CustomDataItem  > const& ) = delete;
			AsyncSubscriber_T( AsyncSubscriber_T< _2Real::CustomDataItem  > && ) = delete;

			static std::pair< std::shared_ptr< AsyncSubscriber_T< _2Real::CustomDataItem > >, _2Real::Connection > create( std::string const& address, std::string const& topic, std::string const& expectedType, ReceivedCallback callback, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
			{
				_2Real::HumanReadableNameVisitor name;

				auto info = engine.getTypeMetainfo( expectedType );		// may throw NotFound

				// create basic subscriber ( not running ) & wrap it
				auto basicSubscriber = SubscriberBlockWrapper::create( address, topic, engine, threadpool, false );
				std::shared_ptr< AsyncSubscriber_T< _2Real::CustomDataItem  > > result( new AsyncSubscriber_T< _2Real::CustomDataItem >( basicSubscriber, info ), AsyncSubscriber_T< _2Real::CustomDataItem  >::Deleter() );
				// connect & start basic subscriber
				auto connection = result->mReceivedSignal.connect( callback );
				basicSubscriber->startListening();

				return std::make_pair( result, connection );
			}

		private:

			AsyncSubscriber_T( std::shared_ptr< SubscriberBlockWrapper > subscriber, std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) : mUnderlyingBasicSubscriber( subscriber ), mDeserializer( info )
			{
				auto callback = std::bind( &_2Real::network::AsyncSubscriber_T< _2Real::CustomDataItem  >::received, this, std::placeholders::_1 );
				mConnectionToBasicSubscriber = subscriber->registerToData( callback );
			}

			~AsyncSubscriber_T() = default;

			void received( std::shared_ptr< const _2Real::DataItem > data )
			{
				assert( data.get() );

				try
				{
					mReceivedSignal( mDeserializer.extract( data ) );
				}
				catch ( TypeMismatch &e )
				{
					std::cout << e.what() << std::endl;
				}
			}

			std::shared_ptr< _2Real::network::SubscriberBlockWrapper >			mUnderlyingBasicSubscriber;
			_2Real::Connection													mConnectionToBasicSubscriber;
			boost::signals2::signal< void( _2Real::CustomDataItem const& ) >	mReceivedSignal;
			Deserializer_T< _2Real::CustomDataItem >							mDeserializer;

			struct Deleter
			{
				void operator()( AsyncSubscriber_T< _2Real::CustomDataItem > *& sub )
				{
					if ( sub->mConnectionToBasicSubscriber.connected() )
						sub->mConnectionToBasicSubscriber.disconnect();

					sub->mReceivedSignal.disconnect_all_slots();

					sub->mUnderlyingBasicSubscriber.reset();

					delete sub;
					sub = nullptr;
				}
			};
		};

		template< typename TType >
		class QueuedSubscriber_T
		{

		public:

			QueuedSubscriber_T( QueuedSubscriber_T< TType > const& ) = delete;
			QueuedSubscriber_T( QueuedSubscriber_T< TType > && ) = delete;

			static std::shared_ptr< QueuedSubscriber_T< TType > > create( std::string const& address, std::string const& topic, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
			{
				_2Real::HumanReadableNameVisitor name;

				// TODO: get name for a certain type, not just an object ( also, this causes a shitty warning and does not compile in debug mode )
				TType *dummy = new TType;
				std::string expectedType = name( *dummy );
				auto info = engine.getTypeMetainfo( expectedType );		// may throw NotFound

				// create basic subscriber ( not running ) & wrap it
				auto basicSubscriber = SubscriberBlockWrapper::create( address, topic, engine, threadpool, false );
				std::shared_ptr< QueuedSubscriber_T< TType > > result( new QueuedSubscriber_T< TType >( basicSubscriber, info ), QueuedSubscriber_T< TType >::Deleter() );
				basicSubscriber->startListening();

				return result;
			}

			TType getNewest()
			{
				std::lock_guard< std::mutex > lock( mLock );
				auto result = mQueue.front(); mQueue.pop_front();
				return result;
			}

			TType getOldest()
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

			QueuedSubscriber_T( std::shared_ptr< SubscriberBlockWrapper > subscriber, std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) : mUnderlyingBasicSubscriber( subscriber ), mDeserializer( info )
			{
				auto callback = std::bind( &_2Real::network::QueuedSubscriber_T< TType >::received, this, std::placeholders::_1 );
				mConnectionToBasicSubscriber = subscriber->registerToData( callback );
			}

			~QueuedSubscriber_T() = default;

			std::shared_ptr< _2Real::network::SubscriberBlockWrapper >			mUnderlyingBasicSubscriber;
			_2Real::Connection													mConnectionToBasicSubscriber;
			Deserializer_T< TType >												mDeserializer;
			std::deque< TType >													mQueue;
			mutable std::mutex													mLock;

			void received( std::shared_ptr< const _2Real::DataItem > data )
			{
				assert( data.get() );

				std::lock_guard< std::mutex > lock( mLock );

				try
				{
					mQueue.push_back( mDeserializer.extract( data ) );
				}
				catch ( TypeMismatch &e )
				{
					std::cout << e.what() << std::endl;
				}
			}

			struct Deleter
			{
				void operator()( QueuedSubscriber_T< TType > *& sub )
				{
					if ( sub->mConnectionToBasicSubscriber.connected() )
						sub->mConnectionToBasicSubscriber.disconnect();

					sub->mUnderlyingBasicSubscriber.reset();

					delete sub;
					sub = nullptr;
				}
			};

		};

		template< >
		class QueuedSubscriber_T< _2Real::CustomDataItem >
		{

		public:

			QueuedSubscriber_T( QueuedSubscriber_T< _2Real::CustomDataItem > const& ) = delete;
			QueuedSubscriber_T( QueuedSubscriber_T< _2Real::CustomDataItem > && ) = delete;

			static std::shared_ptr< QueuedSubscriber_T< _2Real::CustomDataItem > > create( std::string const& address, std::string const& topic, std::string const& expectedType, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
			{
				_2Real::HumanReadableNameVisitor name;

				auto info = engine.getTypeMetainfo( expectedType );		// may throw NotFound

				// create basic subscriber ( not running ) & wrap it
				auto basicSubscriber = SubscriberBlockWrapper::create( address, topic, engine, threadpool, false );
				std::shared_ptr< QueuedSubscriber_T< _2Real::CustomDataItem > > result( new QueuedSubscriber_T< _2Real::CustomDataItem >( basicSubscriber, info ), QueuedSubscriber_T< _2Real::CustomDataItem >::Deleter() );
				basicSubscriber->startListening();

				return result;
			}

			 _2Real::CustomDataItem getNewest()
			{
				std::lock_guard< std::mutex > lock( mLock );
				auto result = mQueue.front(); mQueue.pop_front();
				return result;
			}

			 _2Real::CustomDataItem getOldest()
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

			QueuedSubscriber_T( std::shared_ptr< SubscriberBlockWrapper > subscriber, std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) : mUnderlyingBasicSubscriber( subscriber ), mDeserializer( info )
			{
				auto callback = std::bind( &_2Real::network::QueuedSubscriber_T<  _2Real::CustomDataItem >::received, this, std::placeholders::_1 );
				mConnectionToBasicSubscriber = subscriber->registerToData( callback );
			}

			~QueuedSubscriber_T() = default;

			std::shared_ptr< _2Real::network::SubscriberBlockWrapper >			mUnderlyingBasicSubscriber;
			_2Real::Connection													mConnectionToBasicSubscriber;
			Deserializer_T< _2Real::CustomDataItem >							mDeserializer;
			// TODO std::reference_wrapper - maybe
			std::deque< _2Real::CustomDataItem >								mQueue;
			mutable std::mutex													mLock;

			void received( std::shared_ptr< const _2Real::DataItem > data )
			{
				assert( data.get() );

				std::lock_guard< std::mutex > lock( mLock );

				try
				{
					mQueue.push_back( mDeserializer.extract( data ) );
				}
				catch ( TypeMismatch &e )
				{
					std::cout << e.what() << std::endl;
				}
			}

			struct Deleter
			{
				void operator()( QueuedSubscriber_T< _2Real::CustomDataItem > *& sub )
				{
					if ( sub->mConnectionToBasicSubscriber.connected() )
						sub->mConnectionToBasicSubscriber.disconnect();

					sub->mUnderlyingBasicSubscriber.reset();

					delete sub;
					sub = nullptr;
				}
			};

		};
	}
}
