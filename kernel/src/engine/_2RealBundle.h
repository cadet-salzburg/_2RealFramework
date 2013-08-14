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

#include "engine/_2RealBundleMetadata.h"
#include "helpers/_2RealNonCopyable.h"
#include "helpers/_2RealIdentifiable.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	class EngineImpl;
	class BundleManager;
	class FunctionBlock;

	class Bundle : private NonCopyable< Bundle >
	{

	public:

		Bundle( EngineImpl *, std::shared_ptr< const BundleMetadata > );

		std::string const&							getFullHumanReadableName() const;
		std::string const&							getHumanReadableName() const;
		//std::string const&							getCode() const;
		std::shared_ptr< const TemplateId > 		getIdentifier() const;

		void										setSelfRef( std::shared_ptr< Bundle > );
		std::shared_ptr< Bundle >					getSelfRef();
		std::shared_ptr< const Bundle >				getSelfRef() const;

		std::shared_ptr< const BundleMetadata >		getBundleMetadata() const;
		void										unload( const long );
		std::shared_ptr< FunctionBlock >			createFunctionBlockInstance( std::string const& );
		std::shared_ptr< FunctionBlock >			getContextBlockInstance();

	private:

		EngineImpl									*const mEngineImpl;
		std::shared_ptr< const BundleMetadata >		mBundleMetadata;
		std::weak_ptr< Bundle >						mSelfRef;

	};

}