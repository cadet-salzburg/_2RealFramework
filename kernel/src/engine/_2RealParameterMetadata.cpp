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
#include "helpers/_2RealTypeDescriptor.h"

using std::string;

namespace _2Real
{
	InletMetadata::InletMetadata( string const& n, TypeDescriptor const *const t, Any const& i, AnyOptionSet const& o, InletPolicy const& p, const bool m ) :
		name( n ), type( t ), initValue( i ), options( o ), defaultPolicy( p ), isMulti( m )
	{
	}

	InletMetadata::~InletMetadata()
	{
		delete type;
	}

	OutletMetadata::OutletMetadata( string const& n, TypeDescriptor const *const t, Any const& i ) :
		name( n ), type( t ), initValue( i )
	{
	}

	OutletMetadata::~OutletMetadata()
	{
		delete type;
	}
}