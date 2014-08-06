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
#include "ZmqSubscriber.h"
#include "BsonSerializer.h"
#include "BsonDeserializer.h"

#include "Subscriber.h"

//int main( int argc, char *argv[] )
//{
//	try
//	{
//		const unsigned int numSubscribers = 20;
//
//		_2Real::app::Engine engine;
//
//		auto threadpool = engine.createThreadpool( _2Real::ThreadpoolPolicy::FIFO );
//
//		// additional bundle loaded b/c of custom type
//		auto customTypeBundle = engine.loadBundle( "TestBundle_CustomTypes" );
//		auto simpleInfo = customTypeBundle.second.getExportedType( "simpleType" );
//
//		std::shared_ptr< _2Real::network::Publisher > publisher = _2Real::network::Publisher::create( "tcp://*:5556", engine, threadpool );
//
//		std::vector< std::shared_ptr< _2Real::network::SubscriberWithQueue > > subscribers( numSubscribers, nullptr );
//		for ( unsigned int i=0; i<numSubscribers; ++i )
//		{
//			std::string label = std::to_string( i );
//			subscribers[ i ] = _2Real::network::SubscriberWithQueue::create( "tcp://localhost:5556", label, engine, threadpool );
//		}
//
//		//std::vector< std::shared_ptr< _2Real::network::SubscriberWithFuture > > subscribers( numSubscribers, nullptr );
//		//for ( unsigned int i=0; i<numSubscribers; ++i )
//		//{
//		//	std::string label = std::to_string( i );
//		//	subscribers[ i ] = _2Real::network::SubscriberWithFuture::create( "tcp://localhost:5556", label, engine, threadpool );
//		//}
//
//		int8_t in1 = 0; int8_t inc1 = -5;
//		uint8_t in2 = 0; uint8_t inc2 = 4;
//		uint32_t in3 = 0; uint32_t inc3 = 8;
//		int32_t in4 = 0; int32_t inc4 = 6;
//		uint64_t in5 = ( std::numeric_limits< int64_t >::max )(); uint64_t inc5 = 10;
//		int64_t in6 = 0; int64_t inc6 = -10;
//		double in7 = 0.0; double inc7 = 0.5;
//		float in8 = 0.0; float inc8 = 2.5f;
//		std::string in9 = ""; std::string app9 = "a";
//		bool in10 = false; // just flip
//		std::vector< int32_t > in11; int32_t app11 = 77;
//		std::vector< double > in12; double app12 = 0.1;
//
//		_2Real::CustomDataItem in13 = simpleInfo.makeCustomData();
//		int32_t in13_int = 19;
//		std::string in13_str = "";
//
//		std::vector< float > in14; float app14 = -0.2f;
//		std::vector< bool > in15; bool app15 = true;
//		std::vector< std::string > in16; std::string app16 = "hello";
//		std::vector< uint32_t > in17; uint32_t app17 = 25;
//		std::vector< uint8_t > in18; uint8_t app18 = 1;
//		std::vector< uint64_t > in19; int64_t app19 = ( std::numeric_limits< int64_t >::max )() + 100000;
//
//		// 5 ( ! )
//
//		while( 1 )
//		{
//			std::this_thread::yield();
//
//			std::string line;
//			char lineEnd = '\n';
//			std::getline( std::cin, line, lineEnd );
//
//			if ( line == "q" )
//				break;
//
//			std::shared_ptr< _2Real::DataItem > serialized;
//			bool publish = false;
//			std::string label;
//			if ( line ==  "1" )
//			{
//				publish = true;
//				label = line;
//				in1 += inc1;
//				_2Real::io::bson::Serializer serializer;
//				serializer( in1 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "2" )
//			{
//				publish = true;
//				label = line;
//				in2 += inc2;
//				_2Real::io::bson::Serializer serializer;
//				serializer( in2 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "3" )
//			{
//				publish = true;
//				label = line;
//				in3 += inc3;
//				_2Real::io::bson::Serializer serializer;
//				serializer( in3 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "4" )
//			{
//				publish = true;
//				label = line;
//				in4 += inc4;
//				_2Real::io::bson::Serializer serializer;
//				serializer( in4 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "5" )
//			{
//				publish = true;
//				label = line;
//				in5 += inc5;
//				_2Real::io::bson::Serializer serializer;
//				serializer( in5 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "6" )
//			{
//				publish = true;
//				label = line;
//				in6 += inc6;
//				_2Real::io::bson::Serializer serializer;
//				serializer( in6 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "7" )
//			{
//				publish = true;
//				label = line;
//				in7 += inc7;
//				_2Real::io::bson::Serializer serializer;
//				serializer( in7 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "8" )
//			{
//				publish = true;
//				label = line;
//				in8 += inc8;
//				_2Real::io::bson::Serializer serializer;
//				serializer( in8 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "9" )
//			{
//				publish = true;
//				label = line;
//				in9.append( app9 );
//				_2Real::io::bson::Serializer serializer;
//				serializer( in9 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "10" )
//			{
//				publish = true;
//				label = line;
//				in10 = !in10;
//				_2Real::io::bson::Serializer serializer;
//				serializer( in10 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "11" )
//			{
//				publish = true;
//				label = line;
//				in11.push_back( app11 );
//				_2Real::io::bson::Serializer serializer;
//				serializer( in11 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "12" )
//			{
//				publish = true;
//				label = line;
//				in12.push_back( app12 );
//				_2Real::io::bson::Serializer serializer;
//				serializer( in12 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "13" )
//			{
//				publish = true;
//				label = line;
//				in13_int += 1;
//				in13_str.append( "-yay-" );
//				in13.set( "int_field", in13_int );
//				in13.set( "string_field", in13_str );
//				_2Real::io::bson::Serializer serializer;
//				serializer( in13 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "14" )
//			{
//				publish = true;
//				label = line;
//				in14.push_back( app14 );
//				_2Real::io::bson::Serializer serializer;
//				serializer( in14 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "15" )
//			{
//				publish = true;
//				label = line;
//				in15.push_back( app15 );
//				_2Real::io::bson::Serializer serializer;
//				serializer( in15 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "16" )
//			{
//				publish = true;
//				label = line;
//				in16.push_back( app16 );
//				_2Real::io::bson::Serializer serializer;
//				serializer( in16 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "17" )
//			{
//				publish = true;
//				label = line;
//				in17.push_back( app17 );
//				_2Real::io::bson::Serializer serializer;
//				serializer( in17 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "18" )
//			{
//				publish = true;
//				label = line;
//				in18.push_back( app18 );
//				_2Real::io::bson::Serializer serializer;
//				serializer( in18 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//			else if ( line == "19" )
//			{
//				publish = true;
//				label = line;
//				in19.push_back( app19 );
//				_2Real::io::bson::Serializer serializer;
//				serializer( in19 );
//				serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
//			}
//
//			if ( publish )
//				publisher->publish( label, serialized );
//
//			// let's wait a bit, so that the data arrives
//			std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
//
//			for ( unsigned int i=0; i<numSubscribers; ++i )
//			{
//				if ( !subscribers[ i ]->empty() )
//				{
//					std::cout << "data available for subscriber " << i << " ( is consumed )" << std::endl;
//					std::shared_ptr< const _2Real::DataItem > d = subscribers[ i ]->getNewest();
//					auto deserializer = _2Real::io::bson::Deserializer::create( d, _2Real::network::Constants::MaxTopicNameLength );
//
//					auto dataItem = deserializer->getDataItem( engine.getTypeMetainfo( deserializer->getTypename() ) );
//
//					_2Real::PrintOutVisitor printy( std::cout );
//					std::cout << "data: ";
//					boost::apply_visitor( printy, *dataItem.get() );
//					std::cout << std::endl;
//				}
//			}
//		}
//
//		engine.clear();
//		std::cout << "enter \'q\' to exit the application" << std::endl;
//	}
//	catch ( _2Real::Exception &e )
//	{
//		std::cout << "-------------exception caught in main------------" << std::endl;
//		std::cout << e.what() << " " << e.message() << std::endl;
//		std::cout << "-------------exception caught in main------------" << std::endl;
//	}
//	catch ( std::exception const& e )
//	{
//		std::cout << "-------------exception caught in main------------" << std::endl;
//		std::cout << e.what() << std::endl;
//		std::cout << "-------------exception caught in main------------" << std::endl;
//	}
//
//	while( 1 )
//	{
//		Sleep( 100 );
//
//		std::string line;
//		char lineEnd = '\n';
//		std::getline( std::cin, line, lineEnd );
//		if ( line == "q" )
//			break;
//	}
//
//	return 0;
//}

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
		std::shared_ptr< _2Real::network::SubscriberWithFuture > subscriber = _2Real::network::SubscriberWithFuture::create( "tcp://localhost:5556", "test", engine, threadpool );

		auto promised = subscriber->nextItem();

		_2Real::CustomDataItem in = simpleInfo.makeCustomData();
		int32_t in_int = 19;
		std::string in_str = "";

		//auto callback = std::bind( []( const int i ){ std::cout << "received int " << i << std::endl; }, std::placeholders::_1 );
		//auto testSubscriber = _2Real::network::Subscriber_T< int >::create( "tcp://localhost:5556", "test", callback, engine, threadpool );

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
				in_int += 1;
				in_str.append( "-yay-" );
				in.set( "int_field", in_int );
				in.set( "string_field", in_str );
				_2Real::io::bson::Serializer serializer;
				serializer( in );
				auto serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );

				publisher->publish( "test", serialized );

				promised.wait();
				auto dataItem = promised.get();
				promised = subscriber->nextItem();

				_2Real::PrintOutVisitor printy( std::cout );
				std::cout << "data: ";
				boost::apply_visitor( printy, *dataItem.get() );
				std::cout << std::endl;
			}
		}

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
		Sleep( 100 );

		std::string line;
		char lineEnd = '\n';
		std::getline( std::cin, line, lineEnd );
		if ( line == "q" )
			break;
	}

	return 0;
}