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

#include "engine/_2RealThreadpool.h"

namespace _2Real
{
	std::shared_ptr< Threadpool > Threadpool::create( const ThreadpoolPolicy p )
	{
		std::shared_ptr< Threadpool > result;
		switch ( p )
		{
		case ThreadpoolPolicy::FIFO:
			result.reset( new FifoThreadpool( 8 ) );
			break;
		case ThreadpoolPolicy::DEDICATED:
			result.reset( new UniqueThreadpool );
			break;
		default:
			break;
		}
		return result;
	}

	FifoThreadpool::FifoThreadpool( const size_t numThreads ) :
		Threadpool(),
		mWorkerThreads( numThreads )
	{
		for ( std::unique_ptr< std::thread > &worker : mWorkerThreads )
		{
			worker.reset( new std::thread( &FifoThreadpool::loop, this ) );
		}

		std::cout << "created fifo pool" << std::endl;
	}

	FifoThreadpool::~FifoThreadpool()
	{
		std::cout << "destroying fifo pool" << std::endl;
		{
			std::unique_lock< std::mutex > lock( mJobsAccess );
			mShouldTerminate = true;
			mNewJob.notify_all();		// all blocking threads are now notified
										// and will attempt to lock, one after another
										// to lock the mutex
										// the termination cond is true, which causes the function to exit
		}

		try
		{
			for ( std::unique_ptr< std::thread > &worker: mWorkerThreads )
			{
				worker->join();
			}
		}
		catch( std::system_error const& e )
		{
			std::cout << "SYSERROR" << std::endl;
		}
	}

	Threadpool::Id FifoThreadpool::createId()
	{
		std::cout << "threadpool: create id" << std::endl;
		return NullId();
	}

	void FifoThreadpool::enqueueJob( Threadpool::Id const&, JobPtr j, CbPtr cb )
	{
		std::lock_guard< std::mutex > lock( mJobsAccess );
		Job job;
		job.doWork = j;
		job.isDone = cb;
		mJobs.push_back( job );		// done
		std::cout << "threadpool: job enqueued" << std::endl;
		mNewJob.notify_one();		// now, let's notify one of the worker threads that there's a new job
	}

	void FifoThreadpool::loop()
	{
		Job job;

		for ( ;; )
		{
			{
				std::unique_lock< std::mutex > lock( mJobsAccess );

				for ( ;; )
				{
					if ( mShouldTerminate )
					{
						return;
					}

					if ( !mJobs.empty() )
					{
						job = mJobs.front();
						mJobs.pop_front();
						break;
					}

					mNewJob.wait( lock );
				}
			}

			// job execution
			std::cout << "threadpool: executing job" << std::endl;
			job.doWork();
			std::cout << "threadpool: on finished" << std::endl;
			job.isDone();
		}
	}

	UniqueThreadpool::UniqueThreadpool() :
		Threadpool(),
		mGenerator()
	{
		std::cout << "created unique pool" << std::endl;
	}

	UniqueThreadpool::~UniqueThreadpool()
	{
		std::cout << "destroying unique pool" << std::endl;
	}

	Threadpool::Id UniqueThreadpool::createId()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		UniqueId id = mGenerator.genId();
		//PerWorker &worker = mWorkerThreads[ id ];
		//worker.thread.reset( new std::thread( &UniqueThreadpool::loop, this, id ) );
		return id;
	}

	void UniqueThreadpool::enqueueJob( Threadpool::Id const& id, JobPtr work, CbPtr callback )
	{
		//std::lock_guard< std::mutex > lock( mMutex );

		//try
		//{
		//	UniqueId const& uniqueId = dynamic_cast< UniqueId const& >( id );
		//	PerWorker &worker = mWorkerThreads[ uniqueId ];
		//	std::unique_lock< std::mutex > lock( worker.mutex );
		//	worker.job.doWork = work;
		//	worker.job.isDone = callback;
		//	worker.hasWork.notify_one();
		//}
		//catch ( std::bad_cast const& e )
		//{
		//}
		//catch ( std::exception const& e )
		//{
		//}
	}

	void UniqueThreadpool::loop( UniqueId const& id )
	{
		//PerWorker &worker = mWorkerThreads[ id ];

		//for ( ;; )
		//{
		//	{
		//		std::unique_lock< std::mutex > lock( worker.mutex );

		//		for ( ;; )
		//		{
		//			if ( mShouldTerminate )
		//				return;

		//			worker.hasWork.wait( lock );
		//		}
		//	}

		//	worker.job.doWork();
		//	worker.job.isDone();
		//}
	}
}
