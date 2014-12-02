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

#include "Publisher.h"
#include "Subscriber.h"

int main( int argc, char *argv[] )
{
	try
	{
		_2Real::app::Engine engine;

		auto threadpool = engine.createThreadpool( _2Real::ThreadpoolPolicy::FIFO );

		// additional bundle loaded b/c of custom type
		auto customTypeBundle = engine.loadBundle( "TestBundle_CustomTypes" );
		auto simpleInfo = customTypeBundle.second.getExportedType( "simpleType" );

		std::shared_ptr< _2Real::network::Publisher > publisher = _2Real::network::Publisher::create( "tcp://*:5556", engine, threadpool );

		// each topic may only publish a single type of data.
		// this restriction makes things a lot easier
		_2Real::network::Publisher::Topic_T< int > topic_int = publisher->addTopic< int >( "int" );
		_2Real::network::Publisher::Topic_T< float > topic_float = publisher->addTopic< float >( "float" );
		_2Real::network::Publisher::Topic_T< _2Real::CustomDataItem > topic_custom = publisher->addTopic( "simpleType", "custom" );

		_2Real::CustomDataItem in_custom = simpleInfo.makeCustomData();
		int32_t in_custom_int_field = 0;
		std::string in_custom_str_field = "";
		std::vector< uint16_t > in_custom_vec_field;

		int in_int = 0;
		float in_float = 0.f;

		//auto callback_int = std::bind( []( const int i ){ std::cout << i << std::endl; }, std::placeholders::_1 );
		//auto callback_float = std::bind( []( const float f ){ std::cout << f << std::endl; }, std::placeholders::_1 );
		//auto callback_custom = std::bind( []( _2Real::CustomDataItem const& c ){ std::cout << c << std::endl; }, std::placeholders::_1 );

		//auto testAsyncSubscriber_int = _2Real::network::AsyncSubscriber_T< int >::create( "tcp://localhost:5556", "int", callback_int, engine, threadpool );
		//auto testAsyncSubscriber_float = _2Real::network::AsyncSubscriber_T< float >::create( "tcp://localhost:5556", "float", callback_float, engine, threadpool );
		//auto testAsyncSubscriber_custom = _2Real::network::AsyncSubscriber_T< _2Real::CustomDataItem >::create( "tcp://localhost:5556", "custom", "simpleType", callback_custom, engine, threadpool );

		auto testQueuedSubscriber_int = _2Real::network::QueuedSubscriber_T< int >::create( "tcp://localhost:5556", "int", engine, threadpool );
		auto testQueuedSubscriber_float = _2Real::network::QueuedSubscriber_T< float >::create( "tcp://localhost:5556", "float", engine, threadpool );
		auto testQueuedSubscriber_custom = _2Real::network::QueuedSubscriber_T< _2Real::CustomDataItem >::create( "tcp://localhost:5556", "custom", "simpleType", engine, threadpool );

		while( 1 )
		{
			std::this_thread::yield();

			std::string line;
			char lineEnd = '\n';
			std::getline( std::cin, line, lineEnd );

			if ( line == "q" )
				break;

			else if ( line == "pub" )
			{
				in_custom_int_field += 1;
				in_custom_str_field.append( "-yay-" );
				in_custom_vec_field.push_back( 10 );
				in_custom.set( "int_field", in_custom_int_field );
				in_custom.set( "string_field", in_custom_str_field );
				in_custom.set( "ushort_field", (uint16_t)100 );
				in_custom.set( "vector_field", in_custom_vec_field );

				in_int += 5;
				in_float += 0.1f;

				topic_int.publish( in_int );
				topic_float.publish( in_float );
				topic_custom.publish( in_custom );

				while ( testQueuedSubscriber_int->empty() || testQueuedSubscriber_float->empty() || testQueuedSubscriber_custom->empty() )
					std::this_thread::yield();

				//std::cout << "the int data is    " << testQueuedSubscriber_int->getNewest() << std::endl;
				//std::cout << "the float data is  " << testQueuedSubscriber_float->getNewest() << std::endl;
				std::cout << "the custom data is " << testQueuedSubscriber_custom->getNewest() << std::endl;
			}
		}

		publisher.reset();		// <---- absolutely vital! the 'high level' publisher attempts to manipulate ( singlestep ) a framework block
								// in a separate thread; clearing the engine in this thread while a block is still in use is a very bad idea
		engine.clear();
		std::cout << "enter \'q\' to exit the application" << std::endl;
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

		std::string line;
		char lineEnd = '\n';
		std::getline( std::cin, line, lineEnd );
		if ( line == "q" )
			break;
	}

	return 0;
}