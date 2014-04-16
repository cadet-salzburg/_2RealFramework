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

#include "common/_2RealVersion.h"

namespace _2Real
{
	Version::Version() :
		mMajor( 0 ),
		mMinor( 0 ),
		mRevision( 0 )
	{
	}

	Version::Version( unsigned int major, unsigned int minor, unsigned int revision ) :
		mMajor( major ),
		mMinor( minor ),
		mRevision( revision )
	{
	}

	bool Version::operator==( Version const& rhs )
	{
		return ( mMajor == rhs.mMajor && mMinor == rhs.mMinor && mRevision == rhs.mRevision );
	}

	bool Version::operator<( Version const& rhs )
	{
		return ( mMajor < rhs.mMajor && mMinor < rhs.mMinor && mRevision < rhs.mRevision );
	}

	std::string Version::string() const
	{
		std::ostringstream result;
		result << mMajor << "." << mMinor << "." << mRevision;
		return result.str();
	}

	unsigned int Version::major() const
	{
		return mMajor;
	}

	unsigned int Version::minor() const
	{
		return mMinor;
	}

	unsigned int Version::revision() const
	{
		return mRevision;
	}

	std::ostream& operator<<( std::ostream &out, Version const& version )
	{
		out << version.major() << "." << version.minor() << "." << version.revision();
		return out;
	}
}