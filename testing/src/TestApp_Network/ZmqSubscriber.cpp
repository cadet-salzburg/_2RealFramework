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

#include "ZmqSubscriber.h"

namespace _2Real
{
	namespace network
	{
		std::shared_ptr< Subscriber > Subscriber::create( std::string const& address, std::string const& topic, Subscriber::DataCallback dataCallback, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
		{
		// returns bundle & metainfo if already loaded
			std::pair< _2Real::app::BundleHandle, _2Real::app::BundleMetainfo > zmqBundle = engine.loadBundle( "TestBundle_Network" );

			if ( threadpool.isValid() )
			{
				_2Real::app::BlockHandle subscriberBlock = zmqBundle.first.createBlock( "zmq_subscriber", threadpool, std::vector< _2Real::app::BlockHandle >() );

		// setup & update called automatically

				// TODO: refactor....
				{
					_2Real::app::ParameterHandle param = subscriberBlock.getParameter( "topic" );
					std::shared_ptr< _2Real::DataItem > data; data.reset( new _2Real::DataItem( topic ) );
					param.setValue( data );
				}

				{
					_2Real::app::ParameterHandle param = subscriberBlock.getParameter( "address" );
					std::shared_ptr< _2Real::DataItem > data; data.reset( new _2Real::DataItem( address ) );
					param.setValue( data );
				}

				{
					auto result = subscriberBlock.setup();
					if ( result.get() != _2Real::BlockResult::CARRIED_OUT )
						throw _2Real::Exception( "network block setup failed" );
				}

				// we prolly can't match this timer, anyway
				// QTS: sth that allows re-triggering of updates?
				_2Real::app::TimerHandle updateTimer = engine.createTimer( 1000.0 );
			
				//updateTimer.registerToTimer( std::bind( [](){ std::cout << "timer test" << std::endl; } ) );

				{
					auto result = subscriberBlock.startUpdating( updateTimer );
					if ( result.get() != _2Real::BlockResult::CARRIED_OUT )
						throw _2Real::Exception( "network block start failed" );
				}

				updateTimer.start();

				Subscriber::Deleter deleter;
				std::shared_ptr< Subscriber > zmqSubscriber( new Subscriber( subscriberBlock, updateTimer, dataCallback ), deleter );
				return zmqSubscriber;
			}
			else
			{
				throw _2Real::CreationFailure( "no threadpool" );
			}
		}
	
		Subscriber::Subscriber( _2Real::app::BlockHandle block, _2Real::app::TimerHandle updateTimer, DataCallback dataCallback ) : mUnderlyingBlock( block ), mUpdateTimer( updateTimer ), mDataCallback( dataCallback )
		{
			auto outlet = block.getOutlet( "data" );
			std::function< void( std::shared_ptr< const _2Real::DataItem > ) > func =  std::bind( &Subscriber::outletListener, this, std::placeholders::_1 );
			outlet.registerToNewData( func );
		}

		void Subscriber::outletListener( std::shared_ptr< const _2Real::DataItem > dataItem )
		{
			std::vector< uint8_t > const& data = boost::get< std::vector< uint8_t > >( *dataItem.get() );

			if ( data.empty() )
				return;

			mDataCallback( dataItem );
		}

		void Subscriber::Deleter::operator()( Subscriber *& sub )
		{
			_2Real::app::BlockHandle &subscriber = sub->mUnderlyingBlock;
			_2Real::app::TimerHandle &timer = sub->mUpdateTimer;

			// engine clear happened before delete
			if ( !subscriber.isValid() || !timer.isValid() )
				return;

			// no more updates
			timer.stop();

			// stop block
			{
				auto result = subscriber.stopUpdating();
				if ( result.get() != _2Real::BlockResult::CARRIED_OUT )
					throw _2Real::Exception( "network block shutdown failed" );
			}

			// and kill ( calls shutdown on its own )
			subscriber.destroy( 1000 );

			// and we're done
			delete sub;
			sub = nullptr;
		}
	}
}