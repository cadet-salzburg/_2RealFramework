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

#include "_2RealIdentifier.h"

#include <iostream>

namespace _2Real
{

	Identifier::Identifier() :
		m_Name( "invalid" ),
		m_Id( 0 )
	{
	}

	Identifier::Identifier( std::string const& name, const unsigned int id ) :
		m_Name( name ),
		m_Id( id )
	{
	}

	std::ostream& operator<<( std::ostream& out, Identifier const& id )
	{
		out << id.m_Name;
		return out;
	}

	std::istream& operator>>( std::istream& in, Identifier &id )
	{
		in >> id.m_Name;
		return in;
	}

	bool Identifier::operator<( Identifier const& rhs ) const
	{
		return m_Id < rhs.m_Id;
	}

}