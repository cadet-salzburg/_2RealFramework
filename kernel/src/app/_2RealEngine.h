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
	class EngineImpl;

	namespace app
	{
		class BundleHandle;
		class BundleMetainfo;
		class TimerHandle;
		class ThreadpoolHandle;

		class Engine
		{

		public:

			Engine();
			~Engine();

			void clear();

			// returns the absolute path to the bundle directory
			Path		 getBundleDirectory() const;
			// loads a bundle, path must be relative to bundle dir, with or without the suffix
			std::pair< BundleHandle, BundleMetainfo > loadBundle( std::string const& libname );

			ThreadpoolHandle createThreadpool( const ThreadpoolPolicy );

			// creates a timer with the desired framerate
			TimerHandle createTimer( const double fps );

		private:

			std::unique_ptr< EngineImpl > mImpl;

		};
	}
}