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
#include "helpers/_2RealPath.h"

#include "enums/_2RealThreadpoolPolicy.h"

namespace _2Real
{
	class TypeCollection;
	class BundleCollection;
	class ThreadpoolCollection;
	class TimerCollection;

	class Threadpool;
	class Timer;
	class Bundle;

	class EngineImpl : public std::enable_shared_from_this< EngineImpl >
	{

	public:

		EngineImpl();
		~EngineImpl();

		std::shared_ptr< Bundle > 			loadLibrary( std::string const& filename );
		Path const&							getBundleDirectory() const;

		std::shared_ptr< Timer >			createTimer( const double fps );

		std::shared_ptr< Threadpool >		createThreadpool( const ThreadpoolPolicy );

	private:

		EngineImpl( EngineImpl const& other ) = delete;
		EngineImpl& operator=( EngineImpl const& other ) = delete;

		// --- members already in correct order

		std::shared_ptr< ThreadpoolCollection >		mThreadpoolCollection;
		// keep around here, for the time being
		// maybe some time in the future, threadpools will be created at user request
		std::weak_ptr< Threadpool >					mStdThreads;		// owned by threadpool collection
		std::weak_ptr< Threadpool >					mCtxtThreads;		// owned by threadpool collection
		std::shared_ptr< TypeCollection >			mTypeCollection;
		std::shared_ptr< BundleCollection >			mBundleCollection;
		std::shared_ptr< TimerCollection >			mTimerCollection;
		
	};

}