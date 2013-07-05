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

#include "engine/_2RealParameterMetadata.h"

using std::string;

namespace _2Real
{
	InletMetadata::InletMetadata( string const& inlet, std::string const& type, CustomType const& init, InletPolicy const& policy, const bool multi ) :
		name( inlet ), customName( type ), defaultPolicy( policy ), isMulti( multi )
	{
		// NOFORGET - copy appropriate?
		// will be passed on to the inlet buffer of each inlet created!
		CustomType *t = new CustomType( init ); // clone
		initValue.reset( t );
	}

	InletMetadata::InletMetadata( string const& inlet, std::string const& type, CustomType const* init, InletPolicy const& policy, const bool multi ) :
		name( inlet ), customName( type ), initValue( init ), defaultPolicy( policy ), isMulti( multi )
	{
	}

	//InletMetadata::InletMetadata( string const& inlet, std::string const& type, InletPolicy const& policy, const bool multi ) :
	//	name( inlet ), customName( type ), defaultPolicy( policy ), isMulti( multi )
	//{
	//}

	InletMetadata::~InletMetadata()
	{
	}

	OutletMetadata::OutletMetadata( string const& outlet, std::string const& type ) :
		name( outlet ), customName( type )
	{
	}

	OutletMetadata::~OutletMetadata()
	{
	}
}