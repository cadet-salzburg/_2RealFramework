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
#include "helpers/_2RealException.h"
#include "helpers/_2RealPath.h"

namespace _2Real
{

	template< typename THeld >
	class SingletonHolder_T;

	class EngineImpl;

	namespace app
	{
		class BundleHandle;

		class Engine
		{

			template< typename THeld >
			friend class _2Real::SingletonHolder_T;

		public:

			static Engine& instance();

	//--------------------- bundle loading
			// returns the absolute path to the bundle directory
			Path const& getBundleDirectory() const;
			// loads a bundle, path must be relative to bundle dir, with or without the suffix
			BundleHandle loadBundle( std::string const& libname );
	//--------------------- bundle loading

		private:

			Engine();
			Engine( Engine const& src );
			Engine& operator=( Engine const& other );

			std::shared_ptr< EngineImpl > mImpl;

		};
	}
}