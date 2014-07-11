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

#include "Publisher.h"
#include "zmq.hpp"

void Publisher::getBlockMetainfo( _2Real::bundle::BlockMetainfo &info, _2Real::bundle::TypeMetainfoCollection const& types )
{
	_2Real::bundle::BlockMetainfo &publisherInfo = dynamic_cast< _2Real::bundle::BlockMetainfo & >( info );

	publisherInfo.setBlockClass< Publisher >();
	publisherInfo.setDescription( "sender based on publish - subscribe pattern" );

	_2Real::bundle::UpdatePolicyMetainfo policy = publisherInfo.getUpdatePolicyMetainfo();
	policy.set( _2Real::DefaultUpdatePolicy::ANY );

	_2Real::bundle::ParameterMetainfo address = publisherInfo.getParameterMetainfo( "address" );
	address.setDescription( "the address this subscriber should listen to" );
	address.setDatatypeAndInitialValue( std::string( "tcp://*:5556" ) );

	_2Real::bundle::InletMetainfo in = publisherInfo.getInletMetainfo( "topic_data" );
	in.setDescription( "every subinlet represents one kind of topic" );
	in.setDatatypeAndInitialValue( std::vector< uint8_t >() );
}

Publisher::Publisher( _2Real::bundle::BlockIo const& io, std::vector< std::shared_ptr< _2Real::bundle::Block > > const& dependencies ) 
	try : _2Real::bundle::Block( io, dependencies ), mContext( new zmq::context_t( 1 ) ), mSocket( new zmq::socket_t( *mContext.get(), ZMQ_PUB ) ) 
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

Publisher::~Publisher()
{
	mSocket.reset();
	mContext.reset();
}

void Publisher::setup()
{
	std::string const& address = boost::get< std::string >( mIo.mParameters[ 0 ]->getValue() );
	mSocket->bind( address.c_str() );
}


void Publisher::update()
{
	try
	{
		auto dataInlet = std::dynamic_pointer_cast< _2Real::bundle::MultiInletHandle >( mIo.mInlets[ 0 ] );
		for ( unsigned int i = 0; i< dataInlet->getSize(); ++i )
		{
			auto subinlet = dataInlet->operator[]( i );
			auto const& data = subinlet.getValue();

			// allright, now I really just want to resend the stuff that was updated.....
			// TODO: -> inlet / subinlet wasUpdated; reset right after update; set when a value is set.

			std::vector< uint8_t > const& buffer = boost::get< std::vector< uint8_t > >( data );
			if ( buffer.empty() )
				continue;

			//std::cout << "block -> sending " << buffer.size() << " bytes " << std::endl;

			zmq::message_t message( buffer.size() );
			memcpy( reinterpret_cast< void * >( message.data() ), reinterpret_cast< void const* >( &buffer[0] ), buffer.size() );

			bool success = mSocket->send( message );

			if ( !success )
				std::cout << "failed to send " << message.size() << std::endl;
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

void Publisher::shutdown()
{
	mSocket->close();
}