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

#include "common/_2RealDeclarations.h"

namespace _2Real
{
	FieldDeclaration::FieldDeclaration( const std::string name, const std::string type ) : mName( name ), mType( type )
	{
	}

	InletDeclaration::InletDeclaration( const std::string name, const bool isMulti ) : mName( name ), mIsMulti( isMulti )
	{
	}

	OutletDeclaration::OutletDeclaration( const std::string name ) : mName( name )
	{
	}

	ParameterDeclaration::ParameterDeclaration( const std::string name ) : mName( name )
	{
	}

	FieldDeclaration declareField( const std::string name, const std::string type )
	{
		return FieldDeclaration( name, type );
	}

	InletDeclaration declareInlet( const std::string name )
	{
		return InletDeclaration( name, false );
	}

	InletDeclaration declareMultiInlet( const std::string name )
	{
		return InletDeclaration( name, true );
	}

	OutletDeclaration declareOutlet( const std::string name )
	{
		return OutletDeclaration( name );
	}

	ParameterDeclaration declareParameter( const std::string name )
	{
		return ParameterDeclaration( name );
	}
}
