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

#include "helpers/_2RealVersion.h"

using std::ostringstream;
using std::string;
using std::ostream;

namespace _2Real
{
	Version::Version() :
		m_Major( 0 ),
		m_Minor( 0 ),
		m_Revision( 0 )
	{
	}

	Version::Version( unsigned int major, unsigned int minor, unsigned int revision ) :
		m_Major( major ),
		m_Minor( minor ),
		m_Revision( revision )
	{
	}

	bool Version::operator==( Version const& rhs )
	{
		return ( m_Major == rhs.m_Major && m_Minor == rhs.m_Minor && m_Revision == rhs.m_Revision );
	}

	bool Version::operator<( Version const& rhs )
	{
		return ( m_Major < rhs.m_Major && m_Minor < rhs.m_Minor && m_Revision < rhs.m_Revision );
	}

	string Version::asString() const
	{
		ostringstream result;
		result << m_Major << "." << m_Minor << "." << m_Revision;
		return result.str();
	}

	unsigned int Version::major() const
	{
		return m_Major;
	}

	unsigned int Version::minor() const
	{
		return m_Minor;
	}

	unsigned int Version::revision() const
	{
		return m_Revision;
	}

	ostream& operator<<( ostream &out, Version const& version )
	{
		out << version.major() << "." << version.minor() << "." << version.revision();
		return out;
	}
}