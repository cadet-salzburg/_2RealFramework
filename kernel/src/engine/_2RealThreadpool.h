/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH

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

#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealCallback_T.h"

#include "enums/_2RealThreadpoolPolicy.h"

#include <thread>
#include <atomic>
#include <condition_variable>

namespace _2Real
{
	class Threadpool
	{

	protected:

		typedef std::function< void() > JobPtr;
		typedef std::function< void() > CbPtr;

		struct Job
		{
			JobPtr			doWork;
			CbPtr			isDone;
		};

	public:

		static std::shared_ptr< Threadpool > create( const ThreadpoolPolicy p );

		class Id
		{
		public:
			virtual ~Id() {}
		};

		virtual ~Threadpool() {}

		virtual Id createId() = 0;
		virtual void enqueueJob( Threadpool::Id const&, JobPtr, CbPtr ) = 0;

	};

	class FifoThreadpool : public Threadpool
	{

	public:

		class NullId : public Threadpool::Id
		{
		};

		FifoThreadpool( const size_t numThreads );
		~FifoThreadpool();

		Threadpool::Id createId();
		void enqueueJob( Threadpool::Id const&, JobPtr, CbPtr );

	private:

		void loop();

		struct Job
		{
			JobPtr			doWork;
			CbPtr			isDone;
		};

		std::mutex										mJobsAccess;
		std::condition_variable							mNewJob;
		std::atomic<bool>								mShouldTerminate;

		std::vector< std::unique_ptr< std::thread > >	mWorkerThreads;
		std::deque< Job >								mJobs;

	};

	class UniqueThreadpool : public Threadpool
	{

	public:

		class UniqueId : public Threadpool::Id
		{
		public:
			UniqueId( const unsigned int i ) : mVal( i )
			{
			}
			bool operator<( UniqueId const& other ) const
			{
				return mVal < other.mVal;
			}
		private:
			unsigned int mVal;
		};

		class IdGenerator
		{
		public:
			IdGenerator() : mCounter( 0 ) {}
			UniqueId genId() const
			{
				unsigned int id = mCounter;
				mCounter+=1;
				return UniqueId( id );
			}
		private:
			mutable unsigned int mCounter;
		};

		UniqueThreadpool();
		~UniqueThreadpool();

		Threadpool::Id createId();
		void enqueueJob( Threadpool::Id const&, JobPtr, CbPtr );

	private:

		void loop( UniqueId const& );

		struct PerWorker
		{
			std::unique_ptr< std::thread >	thread;
			std::condition_variable			hasWork;
			std::mutex						mutex;
			Job								job;
		};

		IdGenerator							mGenerator;

		std::mutex							mMutex;
		std::atomic<bool>					mShouldTerminate;

		std::map< UniqueId, PerWorker >		mWorkerThreads;

	};
}