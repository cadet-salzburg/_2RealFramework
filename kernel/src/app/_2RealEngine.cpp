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

#include "app/_2RealEngine.h"
#include "app/_2RealBundleHandle.h"
#include "app/_2RealTimerHandle.h"
#include "app/_2RealSystemHandle.h"
#include "enums/_2RealThreadpoolPolicy.h"
#include "engine/_2RealEngineImpl.h"
#include "helpers/_2RealBoost.h"

namespace _2Real
{
	namespace app
	{
		Engine::Engine() :
			mImpl( new EngineImpl )
		{
		}

		Engine::~Engine()
		{
		}

		void Engine::clear()
		{
			mImpl->clear();
		}

	// ---------------------------------- bundle

		Path const& Engine::getBundleDirectory() const
		{
			return mImpl->getBundleDirectory();
		}

		BundleHandle Engine::loadBundle( std::string const& libname )
		{
			auto bundle = mImpl->loadLibrary( libname );
			return BundleHandle( bundle );
		}

	// ---------------------------------- bundle

	// ---------------------------------- timer

		TimerHandle Engine::createTimer( const double fps )
		{
			auto timer = mImpl->createTimer( fps );
			return TimerHandle( timer );
		}

	// ---------------------------------- timer

	// ---------------------------------- threadpool

		SystemHandle Engine::createSystem( const ThreadpoolPolicy policy )
		{
			auto system = mImpl->createThreadpool( policy );
			return SystemHandle( system );
		}

	// ---------------------------------- threadpool

	}
}