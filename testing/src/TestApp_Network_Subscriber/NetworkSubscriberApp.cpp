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

#ifndef BOOST_ALL_DYN_LINK
	#define BOOST_ALL_DYN_LINK
#endif

#include "Subscriber.h"

int main( int argc, char *argv[] )
{
	try
	{
		std::string address, topic;
		std::cout << "enter the tcp address ( e.g. \'tcp://localhost:5556\' )" << std::endl;
		std::getline( std::cin, address, '\n' );
		std::cout << "address: " << address << std::endl;
		std::cout << "enter the topic title" << std::endl;
		std::getline( std::cin, topic, '\n' );
		std::cout << "topic: " << topic << std::endl;

		_2Real::app::Engine engine;

		auto threadpool = engine.createThreadpool( _2Real::ThreadpoolPolicy::FIFO );

		auto customTypeBundle = engine.loadBundle( "TestBundle_CustomTypes" );
		auto simpleInfo = customTypeBundle.second.getExportedType( "simpleType" );

		auto subscriber = _2Real::network::QueuedSubscriber_T< _2Real::CustomDataItem >::create( address, topic, "simpleType", engine, threadpool );

		std::cout << "enter \'out\' to print all data to std::cout, \'quit\' to shutdown the receiver" << std::endl;
		while( 1 )
		{
			std::this_thread::yield();

			std::string input;
			std::getline( std::cin, input, '\n' );

			if ( input == "quit" )
				break;
			else if ( input == "out" )
			{
				while ( !subscriber->empty() )
					std::cout << subscriber->getNewest() << std::endl;
			}
		}

		engine.clear();
		std::cout << "enter \'quit\' to exit the application" << std::endl;
	}
	catch ( _2Real::Exception &e )
	{
		std::cout << "-------------exception caught in main------------" << std::endl;
		std::cout << e.what() << " " << e.message() << std::endl;
		std::cout << "-------------exception caught in main------------" << std::endl;
	}
	catch ( std::exception const& e )
	{
		std::cout << "-------------exception caught in main------------" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "-------------exception caught in main------------" << std::endl;
	}

	while( 1 )
	{
		std::this_thread::yield();
		std::string input;
		std::getline( std::cin, input, '\n' );
		if ( input == "quit" )
			break;
	}

	return 0;
}