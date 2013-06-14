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
	struct InletMetadata
	{
		InletMetadata( std::string const& inlet, std::string const& customName, CustomType const& init, InletPolicy const& policy, const bool multi );
		InletMetadata( std::string const& inlet, std::string const& customName, InletPolicy const& policy, const bool multi );
		~InletMetadata();

		std::string										const name;
		std::string										const customName;
		std::shared_ptr< const CustomType >				initValue;
		InletPolicy										const defaultPolicy;
		bool											const isMulti;
	};

	struct OutletMetadata
	{
		OutletMetadata( std::string const& outlet, std::string const& customName );
		~OutletMetadata();

		std::string										const name;
		std::string										const customName;
	};

	//struct SetupParameterMetadata
	//{
	//	SetupParameterMetadata( std::string const& outlet, CustomType const& init );
	//	~SetupParameterMetadata();
	//};
}