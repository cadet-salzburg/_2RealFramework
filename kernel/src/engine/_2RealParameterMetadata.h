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
#include "helpers/_2RealAny.h"
#include "helpers/_2RealOptions.h"

#include <string>

namespace _2Real
{
	class TypeDescriptor;

	struct InletMetadata
	{
		InletMetadata( std::string const& n, TypeDescriptor const *const t, Any const& i, AnyOptionSet const& o, InletPolicy const& p, const bool m );
		~InletMetadata();

		TypeDescriptor			const *const type;
		std::string				const name;
		Any						const initValue;
		AnyOptionSet			const options;
		InletPolicy				const defaultPolicy;
		bool					const isMulti;
	};

	struct OutletMetadata
	{
		OutletMetadata( std::string const& n, TypeDescriptor const *const t, Any const& i );
		~OutletMetadata();

		TypeDescriptor			const *const type;
		std::string				const name;
		Any						const initValue;
	};
}