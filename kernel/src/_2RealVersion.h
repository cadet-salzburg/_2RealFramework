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

#include <string>
#include <sstream>

namespace _2Real
{

	/**
	*	helper class for plugin version
	*/

	class Version
	{

	public:

		Version(unsigned int const& major, unsigned int const& minor, unsigned int const& revision);

		bool operator==(Version const& rhs);
		bool operator<(Version const& rhs);

		friend std::ostream& operator<<(std::ostream &out, Version const& version);

		std::string asString() const;

		unsigned int const& major() const;
		unsigned int const& minor() const;
		unsigned int const& revision() const;

	private:

		unsigned int		m_Major;
		unsigned int		m_Minor;
		unsigned int		m_Revision;

	};

	inline Version::Version(unsigned int const& major, unsigned int const& minor, unsigned int const& revision) :
		m_Major(major),
		m_Minor(minor),
		m_Revision(revision)
	{
	}

	inline bool Version::operator==(Version const& rhs)
	{
		return (m_Major == rhs.m_Major && m_Minor == rhs.m_Minor && m_Revision == rhs.m_Revision);
	}

	inline bool Version::operator<(Version const& rhs)
	{
		return (m_Major < rhs.m_Major && m_Minor < rhs.m_Minor && m_Revision < rhs.m_Revision);
	}

	inline std::string Version::asString() const
	{
		std::ostringstream result;
		result << m_Major << "." << m_Minor << "." << m_Revision;
		return result.str();
	}

	inline unsigned int const& Version::major() const
	{
		return m_Major;
	}

	inline unsigned int const& Version::minor() const
	{
		return m_Minor;
	}

	inline unsigned int const& Version::revision() const
	{
		return m_Revision;
	}

	inline std::ostream& operator<<(std::ostream &out, Version const& version)
	{
		out << version.major() << "." << version.minor() << "." << version.revision();
	}

}