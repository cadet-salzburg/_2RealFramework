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

#include "engine/_2RealInletPolicy.h"
#include "datatypes/_2RealCustomData.h"

#include <string>

namespace _2Real
{
	// the shared ptrs are copies of the init values, owned by the framework

	struct InletMetadata
	{
		InletMetadata( std::string const& inlet, CustomType const& init, InletPolicy const& policy, const bool multi );
		~InletMetadata();

		std::string										const name;
		std::shared_ptr< const CustomType >				initValue;
		InletPolicy										const defaultPolicy;
		bool											const isMulti;
	};

	struct OutletMetadata
	{
		OutletMetadata( std::string const& outlet, CustomType const& init );
		~OutletMetadata();

		std::string										const name;
		std::shared_ptr< const CustomType >				initValue;
	};

	//struct SetupParameterMetadata
	//{
	//	SetupParameterMetadata( std::string const& outlet, CustomType const& init );
	//	~SetupParameterMetadata();
	//};
}