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

#pragma once

#include "common/_2RealSignals.h"
#include "engine/_2RealHumanReadableNameVisitor.h"
#include "ZmqPublisherWrapper.h"
#include "BsonSerializer.h"
#include <iostream>

namespace _2Real
{
	namespace network
	{
		/*
		*	'queued' publisher - data items in a specific topic are stored in a queue, and therefore always sent in the correct order
		*	of course, that means that an if you're generating values way too fast, the publisher will lag behind & you won't even notice this
		*/

		/*
		*	IMPORTANT
		*	the engine was not designed with multithreading in mind;
		*	yet here a separate thread is used to periodically call 'singlestep' on a framework block
		*	obviously, this is critical
		*
		*
		*	the following is safe:
		*	creation & deletion of the publisher object must happen from within the same thread that the engine was created in
		*	afterwards, no-one can access the underlying framework block except the publisher -> everyting is fine
		*	the only thing that must not happen, is clearing the engine ( i.e. deleting all blocks ) while the publisher object exists!
		*	-> call 'reset' on the shared_ptr pointing to the publisher before clearing ( or destroying ) the engine
		*/
		//class Publisher
		//{

		//private:

		//	class Topic
		//	{
		//	public:
		//		explicit Topic( std::string const& name ) : mName( name ) {}
		//		void add( std::shared_ptr< _2Real::DataItem > serialized )
		//		{
		//			assert( serialized.get() );

		//			std::lock_guard< std::mutex > lock( mLock );
		//			mData.push_front( serialized );
		//		}
		//		std::string getName()
		//		{
		//			return mName;
		//		}
		//		bool empty() const
		//		{
		//			std::lock_guard< std::mutex > lock( mLock );
		//			return mData.empty();
		//		}
		//		std::shared_ptr< _2Real::DataItem > getData()
		//		{
		//			assert( !mData.empty() );

		//			std::lock_guard< std::mutex > lock( mLock );
		//			std::shared_ptr< _2Real::DataItem > result = mData.back();
		//			mData.pop_back();

		//			return result;
		//		}
		//	private:
		//		mutable std::mutex										mLock;
		//		std::string												mName;
		//		std::deque< std::shared_ptr< _2Real::DataItem > >		mData;
		//	};

		//public:

		//	template< typename TType >
		//	class Topic_T
		//	{

		//	public:

		//		Topic_T( Topic_T< TType > const& ) = default;
		//		Topic_T& operator=( Topic_T< TType > const& ) = default;
		//		~Topic_T() = default;
		//		Topic_T() = default;

		//		explicit Topic_T( std::shared_ptr< Topic > topic ) : mTopic( topic ) {}

		//		void publish( TType const& value )
		//		{
		//			auto underlyingTopic = mTopic.lock();
		//			if ( !underlyingTopic.get() )
		//				return;

		//			_2Real::io::bson::Serializer serializer;
		//			serializer( value );
		//			std::shared_ptr< _2Real::DataItem > serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );

		//			underlyingTopic->add( serialized );
		//		}

		//	private:

		//		std::weak_ptr< Publisher::Topic >	mTopic;

		//	};

		//	template< >
		//	class Topic_T< _2Real::CustomDataItem >
		//	{

		//	public:

		//		Topic_T( Topic_T< _2Real::CustomDataItem > const& ) = default;
		//		Topic_T& operator=( Topic_T< _2Real::CustomDataItem > const& ) = default;
		//		~Topic_T() = default;
		//		Topic_T() = default;

		//		Topic_T( std::string const& typeName, std::shared_ptr< Topic > topic ) : mExpectedType( typeName ), mTopic( topic ) {}

		//		void publish( _2Real::CustomDataItem const& value )
		//		{
		//			//std::cout << value << std::endl;

		//			auto underlyingTopic = mTopic.lock();
		//			if ( !underlyingTopic.get() )
		//				return;

		//			if ( value.getName() != mExpectedType )
		//				throw TypeMismatch( std::string( "this topic expects" ).append( mExpectedType ) );

		//			_2Real::io::bson::Serializer serializer;
		//			serializer( value );
		//			std::shared_ptr< _2Real::DataItem > serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );

		//			underlyingTopic->add( serialized );
		//		}

		//	private:

		//		std::string							mExpectedType;
		//		std::weak_ptr< Publisher::Topic >	mTopic;

		//	};

		//	Publisher( Publisher const& ) = delete;
		//	Publisher& operator=( Publisher const& ) = delete;

		//	static std::shared_ptr< Publisher > create( std::string const& address, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
		//	{
		//		auto underlyingPublisher = _2Real::network::PublisherBlockWrapper::create( address, engine, threadpool );
		//		std::shared_ptr< Publisher > result( new Publisher( underlyingPublisher ), Publisher::Deleter() );
		//		return result;
		//	}

		//	template< typename TType >
		//	Topic_T< TType > addTopic( std::string const& topicName )
		//	{
		//		if ( !PublisherBlockWrapper::topicNameCheck( topicName ) )
		//			throw _2Real::Exception( "name too long, should be max 18 characters" );

		//		std::lock_guard< std::mutex > lock( mLock );
		//		mTopics.push_back( std::shared_ptr< Topic >( new Topic( topicName ) ) );
		//		Topic_T< TType> result( mTopics.back() );
		//		return result;
		//	}

		//	Topic_T< _2Real::CustomDataItem > addTopic( std::string const& typeName, std::string const& topicName )
		//	{
		//		if ( !PublisherBlockWrapper::topicNameCheck( topicName ) )
		//			throw _2Real::Exception( "name too long, should be max 18 characters" );
		//		
		//		std::lock_guard< std::mutex > lock( mLock );
		//		mTopics.push_back( std::shared_ptr< Topic >( new Topic( topicName ) ) );
		//		Topic_T< _2Real::CustomDataItem > result( typeName, mTopics.back() );
		//		return result;
		//	}

		//private:

		//	Publisher( std::shared_ptr< _2Real::network::PublisherBlockWrapper > publisher ) : mUnderlyingBasicPublisher( publisher ), mShouldRun( true )
		//	{
		//		mThread.swap( std::thread( &Publisher::publishStuff, this ) );
		//	}

		//	~Publisher() = default;

		//	std::shared_ptr< _2Real::network::PublisherBlockWrapper >		mUnderlyingBasicPublisher;	// the basic publisher
		//	std::thread														mThread;					// 
		//	std::atomic< bool >												mShouldRun;					// keeps thread running
		//	mutable std::mutex												mLock;						// used to lock down the topics
		//	std::vector< std::shared_ptr< Topic > >							mTopics;					// containing data for the topics ( one publisher = many topics )

		//	void publishStuff()
		//	{
		//		while ( mShouldRun )
		//		{
		//			try
		//			{
		//				auto start_time = std::chrono::high_resolution_clock::now();

		//				unsigned int numDataItems = 0;
		//				{
		//					std::lock_guard< std::mutex > lock( mLock );
		//					for ( auto topic : mTopics )
		//					{
		//						if ( !topic->empty() )
		//						{
		//							numDataItems += 1;
		//							mUnderlyingBasicPublisher->setTopicMessage( topic->getName(), topic->getData() );
		//						}
		//					}
		//				}

		//				// will throw an exception if it takes longer than 30 milliseconds to singlestep
		//				if ( numDataItems > 0 )
		//					mUnderlyingBasicPublisher->blockingSend( 30 );

		//				auto end_time = std::chrono::high_resolution_clock::now();

		//				auto sleep_dur = std::chrono::milliseconds( 33 ) - std::chrono::duration_cast< std::chrono::milliseconds >( end_time - start_time );
		//				std::this_thread::sleep_for( sleep_dur );
		//			}
		//			catch ( std::exception &e )
		//			{
		//				std::cout << e.what() << std::endl;
		//			}
		//		}
		//	}

		//	struct Deleter
		//	{
		//		void operator()( Publisher *& pub )
		//		{
		//			pub->mShouldRun = false;
		//			pub->mThread.join();

		//			pub->mLock.lock();
		//			pub->mTopics.clear();
		//			pub->mLock.unlock();

		//			pub->mUnderlyingBasicPublisher.reset();

		//			delete pub;
		//			pub = nullptr;
		//		}
		//	};
		//};

		/*
		*	IMPORTANT
		*	the engine was not designed with multithreading in mind;
		*	yet here a separate thread is used to periodically call 'singlestep' on a framework block
		*	obviously, this is critical
		*
		*
		*	the following is safe:
		*	creation & deletion of the publisher object must happen from within the same thread that the engine was created in
		*	afterwards, no-one can access the underlying framework block except the publisher -> everyting is fine
		*	the only thing that must not happen, is clearing the engine ( i.e. deleting all blocks ) while the publisher object exists!
		*	-> call 'reset' on the shared_ptr pointing to the publisher before clearing ( or destroying ) the engine
		*/
		class Publisher
		{

		private:

			class Topic
			{
			public:
				explicit Topic( std::string const& name ) : mName( name ) {}
				void set( std::shared_ptr< _2Real::DataItem > serialized )
				{
					assert( serialized.get() );

					std::lock_guard< std::mutex > lock( mLock );
					mData = serialized;
				}
				std::string getName()
				{
					return mName;
				}
				bool empty() const
				{
					std::lock_guard< std::mutex > lock( mLock );
					return !mData.get();
				}
				std::shared_ptr< _2Real::DataItem > getData()
				{
					assert( mData.get() );

					std::lock_guard< std::mutex > lock( mLock );
					std::shared_ptr< _2Real::DataItem > result = mData;
					mData = nullptr;

					return result;
				}
			private:
				mutable std::mutex							mLock;
				std::string									mName;
				std::shared_ptr< _2Real::DataItem >			mData;
			};

		public:

			template< typename TType >
			class Topic_T
			{

			public:

				Topic_T( Topic_T< TType > const& ) = default;
				Topic_T& operator=( Topic_T< TType > const& ) = default;
				~Topic_T() = default;
				Topic_T() = default;

				explicit Topic_T( std::shared_ptr< Topic > topic ) : mTopic( topic ) {}

				void publish( TType const& value )
				{
					auto underlyingTopic = mTopic.lock();
					if ( !underlyingTopic.get() )
						return;

					_2Real::io::bson::Serializer serializer;
					serializer( value );
					std::shared_ptr< _2Real::DataItem > serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );

					underlyingTopic->set( serialized );
				}

			private:

				std::weak_ptr< Publisher::Topic >	mTopic;

			};

			template< >
			class Topic_T< _2Real::CustomDataItem >
			{

			public:

				Topic_T( Topic_T< _2Real::CustomDataItem > const& ) = default;
				Topic_T& operator=( Topic_T< _2Real::CustomDataItem > const& ) = default;
				~Topic_T() = default;
				Topic_T() = default;

				Topic_T( std::string const& typeName, std::shared_ptr< Topic > topic ) : mExpectedType( typeName ), mTopic( topic ) {}

				void publish( _2Real::CustomDataItem const& value )
				{
					//std::cout << value << std::endl;

					auto underlyingTopic = mTopic.lock();
					if ( !underlyingTopic.get() )
						return;

					if ( value.getName() != mExpectedType )
						throw TypeMismatch( std::string( "this topic expects" ).append( mExpectedType ) );

					_2Real::io::bson::Serializer serializer;
					serializer( value );
					std::shared_ptr< _2Real::DataItem > serialized = serializer.getDataItem( _2Real::network::Constants::MaxTopicNameLength );

					underlyingTopic->set( serialized );
				}

			private:

				std::string							mExpectedType;
				std::weak_ptr< Publisher::Topic >	mTopic;

			};

			Publisher( Publisher const& ) = delete;
			Publisher& operator=( Publisher const& ) = delete;

			static std::shared_ptr< Publisher > create( std::string const& address, _2Real::app::Engine &engine, _2Real::app::ThreadpoolHandle threadpool )
			{
				auto underlyingPublisher = _2Real::network::PublisherBlockWrapper::create( address, engine, threadpool );
				std::shared_ptr< Publisher > result( new Publisher( underlyingPublisher ), Publisher::Deleter() );
				return result;
			}

			template< typename TType >
			Topic_T< TType > addTopic( std::string const& topicName )
			{
				if ( !PublisherBlockWrapper::topicNameCheck( topicName ) )
					throw _2Real::Exception( "name too long, should be max 18 characters" );

				std::lock_guard< std::mutex > lock( mLock );
				mTopics.push_back( std::shared_ptr< Topic >( new Topic( topicName ) ) );
				Topic_T< TType> result( mTopics.back() );
				return result;
			}

			Topic_T< _2Real::CustomDataItem > addTopic( std::string const& typeName, std::string const& topicName )
			{
				if ( !PublisherBlockWrapper::topicNameCheck( topicName ) )
					throw _2Real::Exception( "name too long, should be max 18 characters" );
				
				std::lock_guard< std::mutex > lock( mLock );
				mTopics.push_back( std::shared_ptr< Topic >( new Topic( topicName ) ) );
				Topic_T< _2Real::CustomDataItem > result( typeName, mTopics.back() );
				return result;
			}

		private:

			Publisher( std::shared_ptr< _2Real::network::PublisherBlockWrapper > publisher ) : mUnderlyingBasicPublisher( publisher ), mShouldRun( true )
			{
				mThread.swap( std::thread( &Publisher::publishStuff, this ) );
			}

			~Publisher() = default;

			std::shared_ptr< _2Real::network::PublisherBlockWrapper >		mUnderlyingBasicPublisher;	// the basic publisher
			std::thread														mThread;					// 
			std::atomic< bool >												mShouldRun;					// keeps thread running
			mutable std::mutex												mLock;						// used to lock down the topics
			std::vector< std::shared_ptr< Topic > >							mTopics;					// containing data for the topics ( one publisher = many topics )

			void publishStuff()
			{
				while ( mShouldRun )
				{
					try
					{
						//auto start_time = std::chrono::high_resolution_clock::now();

						unsigned int numDataItems = 0;
						{
							std::lock_guard< std::mutex > lock( mLock );
							for ( auto topic : mTopics )
							{
								if ( !topic->empty() )
								{
									numDataItems += 1;
									mUnderlyingBasicPublisher->setTopicMessage( topic->getName(), topic->getData() );
								}
							}
						}

						// will throw an exception if it takes longer than 30 milliseconds to singlestep
						if ( numDataItems > 0 )
							mUnderlyingBasicPublisher->blockingSend( 30 );

						//auto end_time = std::chrono::high_resolution_clock::now();

						//auto sleep_dur = std::chrono::milliseconds( 33 ) - std::chrono::duration_cast< std::chrono::milliseconds >( end_time - start_time );
						//std::this_thread::sleep_for( sleep_dur );
						std::this_thread::yield();
					}
					catch ( std::exception &e )
					{
						std::cout << e.what() << std::endl;
					}
				}
			}

			struct Deleter
			{
				void operator()( Publisher *& pub )
				{
					pub->mShouldRun = false;
					pub->mThread.join();

					pub->mLock.lock();
					pub->mTopics.clear();
					pub->mLock.unlock();

					pub->mUnderlyingBasicPublisher.reset();

					delete pub;
					pub = nullptr;
				}
			};
		};
	}
}
