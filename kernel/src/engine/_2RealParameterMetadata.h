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
	class TypeMetadata;

	// TODO: replace by object....

	struct InletMetadata
	{
		InletMetadata( std::string const&, std::string const&, std::shared_ptr< const CustomType >, TypeMetadata const*, InletPolicy const&, const bool );
		~InletMetadata();

		std::string										name;
		std::string										type;
		TypeMetadata									const* metadata;
		std::shared_ptr< const CustomType >				initValue;
		InletPolicy										defaultPolicy;
		bool											isMulti;
	};

	struct OutletMetadata
	{
		OutletMetadata( std::string const&, std::string const&, TypeMetadata const* );
		~OutletMetadata();

		std::string										name;
		std::string										type;
		TypeMetadata									const* metadata;
	};

	struct ParameterMetadata
	{
		ParameterMetadata( std::string const&, std::string const&, std::shared_ptr< const CustomType >, TypeMetadata const* );
		~ParameterMetadata();

		std::string										name;
		std::string										type;
		TypeMetadata									const* metadata;
		std::shared_ptr< const CustomType >				initValue;
	};
}