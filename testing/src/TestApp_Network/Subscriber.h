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

#include "common/_2RealBoostDefines.h"
#include "boost/signals2.hpp"
#include "ZmqSubscriber.h"
#include "BsonDeserializer.h"

namespace _2Real
{
	namespace network
	{
		// underlying subscriber
		// deserializes & calls
		template< typename TType >
		class Subscriber_T
		{

		public:

			typedef typename boost::signals2::signal< void( const TType ) >::slot_type ReceivedCallback;
		
			Subscriber_T( Subscriber_T< TType > const& ) = delete;
			Subscriber_T( Subscriber_T< TType > && ) = delete;

			static std::pair< std::shared_ptr< Subscriber_T< TType > >, _2Real::Connection > create( std::string const& address, std::string const& topic, ReceivedCallback callback, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
			{
				// create basic subscriber ( not running )
				auto basicSubscriber = Subscriber::create( address, topic, engine, threadpool, false );

				std::shared_ptr< Subscriber_T< TType > > result( new Subscriber_T< TType >( basicSubscriber ), Subscriber_T< TType >::Deleter() );

				// connect & start basic subscriber
				auto connection = result->mReceivedSignal.connect( callback );

				return std::make_pair( result, connection );
			}

		private:

			Subscriber_T( std::shared_ptr< Subscriber > subscriber ) : mUnderlyingBasicSubscriber( subscriber )
			{
				//auto callback = std::bind( &_2Real::network::Subscriber_T< TType >::received, this, std::placeholders::_1 );
				//subscriber->registerToData( callback );
			}

			~Subscriber_T() = default;

			void received( std::shared_ptr< const _2Real::DataItem > data )
			{
				//assert( data.get() );

				//auto deserializer = _2Real::io::bson::Deserializer::create( data, _2Real::network::Constants::MaxTopicNameLength );
				//if ( deserializer->getTypename() != mExpectedType )
				//{
				//	std::cout << "did not receive expected type" << std::endl;
				//}
				//else
				//{
				//	auto deserialized = deserializer->getDataItem( mExpectedType );
				//	TType const& extracted = boost::get< TType const& >( *deserialized.get() );
				//	mReceivedSignal( extracted );
				//}
			}

			std::shared_ptr< _2Real::network::Subscriber >		mUnderlyingBasicSubscriber;
			_2Real::Connection									mConnection;
			boost::signals2::signal< void( const TType ) >		mReceivedSignal;
			std::string											mExpectedType;

			struct Deleter
			{
				void operator()( Subscriber_T< TType > *& sub )
				{
					//sub->mUnderlyingBasicSubscriber->stopListening();
					//if ( sub->mConnection.connected() )
					//	sub->mConnection.disconnect();

					//sub->mReceivedSignal.clear();

					//sub->mUnderlyingBasicSubscriber.reset();

					delete sub;
					sub = nullptr;
				}
			};
		};
	}
}
