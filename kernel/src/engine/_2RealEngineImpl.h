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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealPath.h"

#include "common/_2RealThreadpoolPolicy.h"

namespace _2Real
{
	class TypeCollection;
	class BundleCollection;
	class ThreadpoolCollection;
	class TimerCollection;

	class ThreadpoolImpl_I;
	class TimerImpl;
	class BundleImpl;
	class BundleMetainfoImpl;

	class EngineImpl : public std::enable_shared_from_this< EngineImpl >
	{

	public:

		EngineImpl();
		~EngineImpl();

		void clear();

		std::pair< std::shared_ptr< BundleImpl >, std::shared_ptr< const BundleMetainfoImpl > > loadLibrary( std::string const& filename );
		Path									getBundleDirectory() const;

		std::shared_ptr< TimerImpl >			createTimer( const double fps );

		std::shared_ptr< ThreadpoolImpl_I >		createThreadpool( const ThreadpoolPolicy );

	private:

		EngineImpl( EngineImpl const& other ) = delete;
		EngineImpl& operator=( EngineImpl const& other ) = delete;

		// --- members already in correct order

		std::shared_ptr< TimerCollection >			mTimerCollection;
		std::shared_ptr< ThreadpoolCollection >		mThreadpoolCollection;
		std::shared_ptr< TypeCollection >			mTypeCollection;
		std::shared_ptr< BundleCollection >			mBundleCollection;
		
	};

}