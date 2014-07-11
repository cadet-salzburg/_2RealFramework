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

#include "_2RealBundle.h"
#include "Publisher.h"
#include "Subscriber.h"

void getBundleMetainfo( _2Real::bundle::BundleMetainfo &info )
{
	info.setAuthor( "fhs33223" );
	info.setDescription( "networking with zmq" );
	info.setCategory( "networking" );
	info.setContact( "fhs33223@fh-salzburg.ac.at" );
	info.setVersion( 0, 0, 0 );

	info.exportsType( "testType", { _2Real::declareField( "int_field", "int" ), _2Real::declareField( "string_", "string" ) } );

	info.exportsBlock( "zmq_publisher", { _2Real::declareMultiInlet( "topic_data" ) }, {}, { _2Real::declareParameter( "address" ) } );
	info.exportsBlock( "zmq_subscriber", {}, { _2Real::declareOutlet( "data" ) }, { _2Real::declareParameter( "topic" ), _2Real::declareParameter( "address" ), _2Real::declareParameter( "rcvtimeo" ) } );
}

void getTypeMetainfo( _2Real::bundle::CustomTypeMetainfo &info, _2Real::bundle::TypeMetainfoCollection const& existingTypes )
{
	( void )( existingTypes );
	if ( info.getName() == "testType" )
	{
		info.setDescription( "network custom type test" );
		info.setInitialFieldValue( "int_field", ( int32_t )15 );
		info.setInitialFieldValue( "string_field", std::string( "poit" ) );
	}
}

void getBlockMetainfo( _2Real::bundle::BlockMetainfo &info, _2Real::bundle::TypeMetainfoCollection const& existingTypes )
{
	if ( info.getName() == "zmq_publisher" )
		Publisher::getBlockMetainfo( info, existingTypes );
	else if ( info.getName() == "zmq_subscriber" )
		Subscriber::getBlockMetainfo( info, existingTypes );
}