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

/*
#include "engine/_2RealUniqueThreadpool.h"

namespace _2Real
{
	ThreadpoolImpl_I::Id UniqueThreadpool::createId()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		UniqueId id = mGenerator.genId();
		//PerWorker &worker = mWorkerThreads[ id ];
		//worker.thread.reset( new std::thread( &UniqueThreadpool::loop, this, id ) );
		return id;
	}

	void UniqueThreadpool::enqueueJob( ThreadpoolImpl_I::Id const& id, JobPtr work, CbPtr callback )
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
*/
