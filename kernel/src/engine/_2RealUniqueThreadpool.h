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
#pragma once

#include "engine/_2RealThreadpoolImpl_I.h"

namespace _2Real
{

	class UniqueThreadpool : public ThreadpoolImpl_I
	{

	public:

		class UniqueId : public ThreadpoolImpl_I::Id
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

		UniqueThreadpool() = default;
		~UniqueThreadpool() = default;

		ThreadpoolImpl_I::Id createId();
		void enqueueJob( ThreadpoolImpl_I::Id const&, JobPtr, CbPtr );

	private:

		void loop( UniqueId const& );

		struct Job
		{
			JobPtr			doWork;
			CbPtr			isDone;
		};

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
*/