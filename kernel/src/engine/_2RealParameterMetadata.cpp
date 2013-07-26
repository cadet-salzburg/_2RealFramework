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

namespace _2Real
{
	InletMetadata::InletMetadata( std::string const& n, std::string const& t, std::shared_ptr< const CustomType > i, TypeMetadata const* m, Policy const& p, const bool b ) :
		name( n ), type( t ), metadata( m ), initValue( i ), defaultPolicy( p ), isMulti( b )
	{
	}

	InletMetadata::~InletMetadata()
	{
	}

	OutletMetadata::OutletMetadata( std::string const& n, std::string const& t, TypeMetadata const*m ) :
		name( n ), type( t ), metadata( m )
	{
	}

	OutletMetadata::~OutletMetadata()
	{
	}

	ParameterMetadata::ParameterMetadata( std::string const& n, std::string const& t, std::shared_ptr< const CustomType > i, TypeMetadata const*m ) :
		name( n ), type( t ), initValue( i ), metadata( m )
	{
	}

	ParameterMetadata::~ParameterMetadata()
	{
	}
}