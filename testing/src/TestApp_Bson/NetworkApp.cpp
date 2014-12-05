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

#define BOOST_ALL_DYN_LINK

#define _WINSOCKAPI_
#include <Windows.h>
#include <winsock.h>
#include "mongo/bson/bson.h"
#include <thread>

std::vector< char > serialize( const std::string message )
{
	mongo::BSONObjBuilder objBuilder;
	objBuilder.append( std::string( "testField" ), message );
	mongo::BSONObj obj = objBuilder.obj();

	return std::vector< char >( obj.objdata(), obj.objdata() + obj.objsize() );
}

std::string deserialize( std::vector< char > const& data )
{
	mongo::BSONObj obj( &data.front() );

	if ( obj.hasField( std::string( "testField" ) ) )
	{
		mongo::BSONElement element = obj.getField( std::string( "testField" ) );
		return element.String();
		//auto stringData = element.valueStringData();
		//std::string result( stringData.begin(), stringData.begin() + stringData.size() );
	}
	else
		return std::string( "NOT FOUND" );
}

int main( int argc, char *argv[] )
{
	try
	{
		std::vector< char > serialized = serialize( std::string( "ach fick dich doch" ) );
		std::string deserialized = deserialize( serialized );
		std::cout << deserialized << std::endl;
	}
	catch ( std::exception &e )
	{
		std::cout << e.what() << std::endl;
	}
	catch ( ... )
	{
		std::cout << "unknown exception" << std::endl;
	}

	while( 1 )
	{
		std::this_thread::yield();

		std::string line;
		char lineEnd = '\n';
		std::getline( std::cin, line, lineEnd );
		if ( line == "quit" )
			break;
	}

	return 0;
}