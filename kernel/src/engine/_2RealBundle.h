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

#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealIdentifiable.h"
#include "app/_2RealInfo.h"

#include <string>

namespace _2Real
{

	class EngineImpl;
	class BundleManager;
	class FunctionBlock;

	class Bundle : private NonCopyable< Bundle >, private Identifiable< Bundle >
	{

	public:

		Bundle( EngineImpl *, app::BundleInfo const& );

		using Identifiable< Bundle >::getIds;
		using Identifiable< Bundle >::getName;

		std::string const&						getAbsPath() const;
		app::BundleInfo const&					getBundleInfo() const;
		void									unload( const long );
		std::shared_ptr< FunctionBlock >		createBlockInstance( std::string const& );
		std::shared_ptr< FunctionBlock >		getContextBlock();

	private:

		EngineImpl								*const mEngineImpl;
		app::BundleInfo							mBundleInfo;

	};

}