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

#ifdef _WIN32
	#ifdef _DEBUG
		//#include "vld.h"
	#endif
	//#include <Windows.h>
#elif _WIN64
	#ifdef _DEBUG
		#include "vld.h"
	#endif
	#include <Windows.h>
#endif

#include "ZmqPublisher.h"
#include "ZmqSubscriber.h"
#include "BsonIo.h"
#include "Common.h"
#include "BsonDeserializer.h"

class TestReceiver
{
public:

	std::thread							mThread;
	std::atomic< bool >					mRun;
	std::string							mTopic;
	std::unique_ptr< zmq::context_t >	mContext;
	std::unique_ptr< zmq::socket_t >	mSocket;

	TestReceiver( std::string const& topic ) : mTopic( topic )
	{
		int linger = 100;
		int timeout = 100;

		mContext.reset( new zmq::context_t( 1 ) );
		mSocket.reset( new zmq::socket_t( *mContext.get(), ZMQ_SUB ) );
		mSocket->connect( "tcp://localhost:5556" );
		mSocket->setsockopt( ZMQ_SUBSCRIBE, mTopic.c_str(), mTopic.size() );
		mSocket->setsockopt( ZMQ_LINGER, &linger, sizeof( linger ) );
		mSocket->setsockopt( ZMQ_RCVTIMEO, &timeout, sizeof( timeout ) );

		mRun = true;

		mThread.swap( std::thread( std::bind( &TestReceiver::handleMessages, this ) ) );
	}

	void handleMessages()
	{
		while( 1 )
		{
			if ( mRun )
			{
				zmq::message_t message;
				bool success = mSocket->recv( &message );
				if ( !success )
				{
					// std::cout << "failed to receive" << std::endl;
					// exception?
				}
				else
				{
					try
					{
						auto messageData = reinterpret_cast< char * >( message.data() );
						auto messageStart = &( messageData )[ _2Real::network::Constants::MaxTopicNameLength ];
						mongo::BSONObj object( messageStart );

						if ( object.hasField( "typename" ) )
						{
							auto typeElem = object.getField( "typename" );

							// weird. prolly sth to do with /0 termination
							std::string typeName;
							typeElem.Val( typeName );

							if ( typeName == "uint" )
							{
								if ( object.hasField( "value" ) )
								{
									auto valElem = object.getField( "value" );
									int32_t val;
									valElem.Val( val );
									std::ostringstream msg;
									msg << "received value " << val << " for topic \'" << mTopic << "\'" << std::endl;
									std::cout << msg.str();
								}
								else
								{
									std::cout << "no field \'value\' found" << std::endl;
								}
							}
							else
							{
								std::cout << "wrong type received: " << typeName << std::endl;
							}
						}
						else
						{
							std::cout << "no field \'typename\' found" << std::endl;
						}
					}
					catch ( zmq::error_t &e )
					{
						std::cout << "zmq error in TestReceiver::handleMessages " << e.what() << std::endl;
					}
					catch ( std::exception &e )
					{
						std::cout << "error in TestReceiver::handleMessages " << e.what() << std::endl;
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	~TestReceiver()
	{
		mRun = false;
		mThread.join();
		mSocket->close();
		mContext->close();
	}
};

int main( int argc, char *argv[] )
{
	try
	{
		_2Real::app::Engine engine;

		auto threadpool = engine.createThreadpool( _2Real::ThreadpoolPolicy::FIFO );
		// loaded b/c of custom types
		auto customTypeBundle = engine.loadBundle( "TestBundle_CustomTypes" );
		auto simpleInfo = customTypeBundle.second.getExportedType( "simpleType" );

		std::shared_ptr< _2Real::network::Publisher > publisher = _2Real::network::Publisher::create( "tcp://*:5556", engine, threadpool );
	
		_2Real::network::ReceiverQueue data_1st, data_2nd, data_3rd, data_4th;
		std::shared_ptr< _2Real::network::Subscriber > subscriber_1st = _2Real::network::Subscriber::create( "tcp://localhost:5556", "first", std::bind( &_2Real::network::ReceiverQueue::add, &data_1st, std::placeholders::_1 ), engine, threadpool );
		std::shared_ptr< _2Real::network::Subscriber > subscriber_2nd = _2Real::network::Subscriber::create( "tcp://localhost:5556", "second", std::bind( &_2Real::network::ReceiverQueue::add, &data_2nd, std::placeholders::_1 ), engine, threadpool );
		std::shared_ptr< _2Real::network::Subscriber > subscriber_3rd = _2Real::network::Subscriber::create( "tcp://localhost:5556", "third", std::bind( &_2Real::network::ReceiverQueue::add, &data_3rd, std::placeholders::_1 ), engine, threadpool );
		std::shared_ptr< _2Real::network::Subscriber > subscriber_4th = _2Real::network::Subscriber::create( "tcp://localhost:5556", "fourth", std::bind( &_2Real::network::ReceiverQueue::add, &data_4th, std::placeholders::_1 ), engine, threadpool );

// for testing purposes
// std::shared_ptr< TestReceiver > receiver_1st( new TestReceiver( "first" ) );
// std::shared_ptr< TestReceiver > receiver_2nd( new TestReceiver( "second" ) );

		uint32_t in_1st = 0;
		double in_2nd = 0.0;
		std::string in_3rd = "";

		// problem with vectors: don't send
		// std::vector< int > in_4th = { 0 };

		_2Real::CustomDataItem in_4th = simpleInfo.makeCustomData();
		in_4th.set( "int_field", 10 );
		in_4th.set( "string_field", std::string( "test" ) );
		while( 1 )
		{
			Sleep( 100 );

			std::string line;
			char lineEnd = '\n';
			std::getline( std::cin, line, lineEnd );
			if ( line == "p1" )
			{
				in_1st += 5;
				{
					_2Real::io::bson::Serializer serializer;
					serializer( in_1st );
					auto data = serializer.getDataItem(_2Real::network::Constants::MaxTopicNameLength );
					publisher->publish( "first", data );
				}
				
				if ( !data_1st.empty() )
				{
					std::shared_ptr< const _2Real::DataItem > data = data_1st.getNewest();
					auto deserializer = _2Real::io::bson::Deserializer::create( data, _2Real::network::Constants::MaxTopicNameLength );
					auto dataItem = deserializer->getDataItem( engine.getTypeMetainfo( deserializer->getTypename() ) );
				}
				else std::cout << "no data available" << std::endl;
			}
			else if ( line == "p2" )
			{
				in_2nd += 0.5;
				{
					_2Real::io::bson::Serializer serializer;
					serializer( in_2nd );
					auto data = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
					publisher->publish( "second", data );
				}
				
				if ( !data_2nd.empty() )
				{
					std::shared_ptr< const _2Real::DataItem > data = data_2nd.getNewest();
					auto deserializer = _2Real::io::bson::Deserializer::create( data, _2Real::network::Constants::MaxTopicNameLength );
					auto dataItem = deserializer->getDataItem( engine.getTypeMetainfo( deserializer->getTypename() ) );
				}
				else std::cout << "no data available" << std::endl;
			}
			else if ( line == "p3" )
			{
				in_3rd.append( "a" );
				{
					_2Real::io::bson::Serializer serializer;
					serializer( in_3rd );
					auto data = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
					publisher->publish( "third", data );
				}
				
				if ( !data_3rd.empty() )
				{
					std::shared_ptr< const _2Real::DataItem > data = data_3rd.getNewest();
					auto deserializer = _2Real::io::bson::Deserializer::create( data, _2Real::network::Constants::MaxTopicNameLength );
					auto dataItem = deserializer->getDataItem( engine.getTypeMetainfo( deserializer->getTypename() ) );
				}
				else std::cout << "no data available" << std::endl;
			}
			else if ( line == "p4" )
			{
				{
					_2Real::io::bson::Serializer serializer;
					serializer( in_4th );
					auto data = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );
					publisher->publish( "fourth", data );
				}
				
				if ( !data_4th.empty() )
				{
					std::shared_ptr< const _2Real::DataItem > data = data_4th.getNewest();
					auto deserializer = _2Real::io::bson::Deserializer::create( data, _2Real::network::Constants::MaxTopicNameLength );
					auto dataItem = deserializer->getDataItem( engine.getTypeMetainfo( deserializer->getTypename() ) );
				}
				else std::cout << "no data available" << std::endl;
			}
			else if ( line == "q" )
				break;
		}

// for testing purposes
// receiver_1st.reset();
// receiver_2nd.reset();

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