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

namespace _2Real
{
	class Thread;

	class Threadpool
	{

	public:

		class Id
		{
		public:
			Id( const unsigned int i ) : mVal( i )
			{
			}
			bool operator<( Id const& other ) const
			{
				return mVal < other.mVal;
			}
		private:
			unsigned int mVal;
		};

		class IdGenerator
		{
		public:
			IdGenerator() : mCounter( 0 )
			{
			}
			Id genId()
			{
				unsigned int id = mCounter;
				mCounter+=1;
				return Id( id );
			}
		private:
			unsigned int mCounter;
		};

		class Policy
		{
		public:
			enum Code { FIFO, DEDICATED };
		};

		// this shit is responsible for actually giving out threads, yay
		class AssignmentPolicy
		{
		public:
			virtual ~AssignmentPolicy() {}

			static std::shared_ptr< AssignmentPolicy > create();

			virtual void addThread() = 0;
			virtual void getThread( Threadpool::Id ) = 0;
		};

		class FifoPolicy : public AssignmentPolicy
		{
		public:
		};

		class DedPolicy : public AssignmentPolicy
		{
		public:
		};

		Threadpool( const unsigned int numThreads, const unsigned int maxThreads, const Policy::Code p );

		// basically, an obj. needs an id so that the thread pool can decide
		// how to assign threads
		// currently, this uses a void ptr. as it's basically a counter, that does not
		// care about type in the slightest
		// in the future, more sophisticated methods might be possible?
		Threadpool::Id getId( void * );

		void registerToThreadAvailable( const Threadpool::Id, std::shared_ptr< AbstractCallback_T< std::shared_ptr< Thread > > > );
		void releaseThread( const Threadpool::Id, std::shared_ptr< Thread > );

	private:

		IdGenerator			mGenerator;

		unsigned int		mMaxCapacity;
		unsigned int		mCurrent;
		unsigned int		mInc;

		// available thread
		// used threads, stored by map?
		AssignmentPolicy	mThreadMgr;

	};
}