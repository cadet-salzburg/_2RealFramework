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

#include "Subscriber.h"
#include "../zmq.hpp"

void Subscriber::getBlockMetainfo( _2Real::bundle::BlockMetainfo &info, _2Real::bundle::TypeMetainfoCollection const& types )
{
	_2Real::bundle::BlockMetainfo &subscriberInfo = dynamic_cast< _2Real::bundle::BlockMetainfo & >( info );

	subscriberInfo.setBlockClass< Subscriber >();
	subscriberInfo.setDescription( "receiver based on publish - subscribe pattern" );

	_2Real::bundle::UpdatePolicyMetainfo policy = subscriberInfo.getUpdatePolicyMetainfo();
	policy.set( _2Real::DefaultUpdatePolicy::DISABLED );

	_2Real::bundle::OutletMetainfo out = subscriberInfo.getOutletMetainfo( "data" );
	out.setDescription( "holds the topic's data" );
	out.setDatatypeAndInitialValue( std::vector< uint8_t >() );

	_2Real::bundle::ParameterMetainfo topic = subscriberInfo.getParameterMetainfo( "topic" );
	topic.setDescription( "the topic this subscriber should listen to" );
	topic.setDatatypeAndInitialValue( std::string( "" ) );

	_2Real::bundle::ParameterMetainfo address = subscriberInfo.getParameterMetainfo( "address" );
	address.setDescription( "the address this subscriber should listen to" );
	address.setDatatypeAndInitialValue( std::string( "tcp://localhost:5556" ) );

	_2Real::bundle::ParameterMetainfo rcvtimeo = subscriberInfo.getParameterMetainfo( "rcvtimeo" );
	rcvtimeo.setDescription( "receive timeout for socket - so that no infinite wait occurs" );
	rcvtimeo.setDatatypeAndInitialValue( ( uint32_t )100 );
}

Subscriber::Subscriber( _2Real::bundle::BlockIo const& io, std::vector< std::shared_ptr< _2Real::bundle::Block > > const& dependencies ) 
	try : _2Real::bundle::Block( io, dependencies ), mContext( new zmq::context_t( 1 ) ), mSocket( new zmq::socket_t( *mContext.get(), ZMQ_SUB ) ) 
{
}
catch( zmq::error_t &e )
{
	throw _2Real::CreationFailure( e.what() );
}
catch( std::bad_alloc &e )
{
	throw _2Real::CreationFailure( e.what() );
}

Subscriber::~Subscriber()
{
	mSocket.reset();
	mContext.reset();
	//std::cout << "goodbye subscriber" << std::endl;
}

void Subscriber::setup()
{
	try
	{
		std::string const& topic = boost::get< std::string >( mIo.mParameters[ 0 ]->getValue() );
		std::string actualTopic = std::string( "*" ) + topic + std::string( "*" );
		std::string const& address = boost::get< std::string >( mIo.mParameters[ 1 ]->getValue() );
		uint32_t rcvtimeo = boost::get< uint32_t >( mIo.mParameters[ 2 ]->getValue() );
		mSocket->connect( address.c_str() );
		mSocket->setsockopt( ZMQ_SUBSCRIBE, actualTopic.c_str(), actualTopic.size() );
		//mSocket->setsockopt( ZMQ_SUBSCRIBE, "", 0 );
		mSocket->setsockopt( ZMQ_RCVTIMEO, &rcvtimeo, sizeof( rcvtimeo ) );
	}
	catch( zmq::error_t &e )
	{
		std::cout << e.what() << std::endl;
	}
	catch( ... )
	{
		std::cout << "unknown exception" << std::endl;
	}
}

void Subscriber::update()
{
	try
	{
		zmq::message_t message;
		bool success = mSocket->recv( &message );
		if ( !success )
		{
			// set outlet value to sth?
			auto dataOutlet = mIo.mOutlets[ 0 ];
			auto &outletData = dataOutlet->getValue();
			std::vector< uint8_t > data = boost::get< std::vector< uint8_t > >( outletData );
			data.clear();
			mIo.mOutlets[ 0 ]->discard();
		}
		else
		{
			auto messageData = reinterpret_cast< char * >( message.data() );
			uint32_t messageLength = message.size(); //- _2Real::network::Constants::MaxTopicNameLength;
			auto messageStart = messageData;//&( messageData )[ _2Real::network::Constants::MaxTopicNameLength ];

			auto dataOutlet = mIo.mOutlets[ 0 ];
			auto &outletData = dataOutlet->getValue();
			std::vector< uint8_t > &data = boost::get< std::vector< uint8_t > >( outletData );

			data.resize( messageLength );
			memcpy( &data[ 0 ], messageStart, messageLength );
		}
	}
	catch( zmq::error_t &e )
	{
		std::cout << e.what() << std::endl;
	}
	catch( ... )
	{
		std::cout << "unknown exception" << std::endl;
	}
}

void Subscriber::shutdown()
{
	mSocket->close();
	mContext->close();
}