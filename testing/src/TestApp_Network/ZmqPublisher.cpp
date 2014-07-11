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

#include "ZmqPublisher.h"

namespace _2Real
{
	namespace network
	{
		std::shared_ptr< Publisher > Publisher::create( std::string const& address, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
		{
		// returns bundle & metainfo if already loaded
			std::pair< _2Real::app::BundleHandle, _2Real::app::BundleMetainfo > zmqBundle = engine.loadBundle( "TestBundle_Network" );

			if ( threadpool.isValid() )
			{
				_2Real::app::BlockHandle publisherBlock = zmqBundle.first.createBlock( "zmq_publisher", threadpool, std::vector< _2Real::app::BlockHandle >() );

				{
					_2Real::app::ParameterHandle param = publisherBlock.getParameter( "address" );
					std::shared_ptr< _2Real::DataItem > data; data.reset( new _2Real::DataItem( address ) );
					param.setValue( data );
				}

		// setup & update called automatically

				{
					auto result = publisherBlock.setup();
					if ( result.get() != _2Real::BlockResult::CARRIED_OUT )
						throw _2Real::Exception( "network block setup failed" );
				}

				// set policy so that
				// any new sub-inlet value will cause an update
				//_2Real::app::UpdatePolicyHandle updatePolicy = publisherBlock.getUpdatePolicy();
				//updatePolicy.set( _2Real::DefaultUpdatePolicy::ANY );
				// TODO: obvious bug in there

				{
					auto result = publisherBlock.startUpdating();
					if ( result.get() != _2Real::BlockResult::CARRIED_OUT )
						throw _2Real::Exception( "network block start failed" );
				}

				Publisher::Deleter deleter;

				std::shared_ptr< Publisher > zmqPublisher( new Publisher( publisherBlock), deleter );
				return zmqPublisher;

			}
			else
			{
				throw _2Real::CreationFailure( "no threadpool" );
			}
		}
	
		Publisher::Publisher( _2Real::app::BlockHandle sender ) : mUnderlyingBlock( sender )
		{
		}

		void Publisher::publish( std::string const& topic, std::shared_ptr< _2Real::DataItem > message )
		{
			if ( topic.size() > Constants::MaxTopicNameLength )
				throw _2Real::Exception( "name too long, should be max 20 characters" );

			try
			{
				std::vector< uint8_t > &data = boost::get< std::vector< uint8_t > >( *message.get() );
				for ( uint32_t i=0; i<topic.size(); ++i ) data[ i ] = topic[ i ];
			}
			catch ( boost::bad_get &e )
			{
				throw _2Real::TypeMismatch( "expected type: vector of bytes" );
			}

			auto topicIter = mTopics.find( topic );
			if ( topicIter != mTopics.end() )
				topicIter->second.setValue( message );
			else
			{
				_2Real::app::MultiInletHandle dataInlet = mUnderlyingBlock.getMultiInlet( "topic_data" );
				_2Real::app::InletHandle inlet = dataInlet.add();
				mTopics.insert( std::make_pair( topic, inlet ) );
				inlet.setValue( message );
			}
		}

		void Publisher::Deleter::operator()( Publisher *& pub )
		{
			//pub->mTopics.clear();
			_2Real::app::BlockHandle &sender = pub->mUnderlyingBlock;

			// engine clear happened before delete
			if ( !sender.isValid() )
				return;

			// stop block
			{
				auto result = sender.stopUpdating();
				if ( result.get() != _2Real::BlockResult::CARRIED_OUT )
					throw _2Real::Exception( "network block shutdown failed" );
			}

			// and kill ( calls shutdown on its own )
			sender.destroy( 2000 );

			// and we're done
			delete pub;
			pub = nullptr;
		}
	}
}