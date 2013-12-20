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

namespace _2Real
{
	namespace app
	{
		class Engine;
	}

	class EngineImpl;

	class TypeRegistry;
	class BundleCollection;
	class Bundle;

	class EngineImpl : public std::enable_shared_from_this< EngineImpl >
	{

	public:

		static std::shared_ptr< EngineImpl > create();

		~EngineImpl();

		std::shared_ptr< Bundle > 		loadLibrary( std::string const& filename );
		Path const&						getBundleDirectory() const;

	private:

		// sets up connections for objects that need it
		void privateInit();

		EngineImpl();
		EngineImpl( EngineImpl const& other );
		EngineImpl& operator=( EngineImpl const& other );

		std::shared_ptr< TypeRegistry >		mTypeRegistry;
		std::shared_ptr< BundleCollection >	mBundleCollection;

	};

}