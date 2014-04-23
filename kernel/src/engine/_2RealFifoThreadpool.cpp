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

#include "engine/_2RealFifoThreadpool.h"
#include "common/_2RealException.h"

namespace _2Real
{

	FifoThreadpool::FifoThreadpool( const size_t numThreads ) :
		ThreadpoolImpl_I(),
		mWorkerThreads( numThreads )
	{
		for ( std::unique_ptr< std::thread > &worker : mWorkerThreads )
		{
			worker.reset( new std::thread( &FifoThreadpool::loop, this ) );
		}
	}

	FifoThreadpool::~FifoThreadpool()
	{
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
				worker->detach();
		}
		catch( std::system_error const& e )
		{
			throw Exception( e.what() );
		}
	}

	ThreadpoolImpl_I::Id FifoThreadpool::createId()
	{
		return NullId();
	}

	void FifoThreadpool::enqueueJob( ThreadpoolImpl_I::Id const&, JobPtr j, CbPtr cb )
	{
		std::lock_guard< std::mutex > lock( mJobsAccess );
		Job job;
		job.doWork = j;
		job.isDone = cb;
		mJobs.push_back( job );		// done
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
			job.doWork();
			job.isDone();
		}
	}

}
