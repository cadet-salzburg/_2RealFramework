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

namespace _2Real
{

	class BundleIdentifier
	{

		friend class BundleInternal;

	public:

		BundleIdentifier();
		BundleIdentifier( BundleIdentifier const& src );
		BundleIdentifier& operator=( BundleIdentifier const& src );

		bool isValid() const;
		bool operator==( BundleIdentifier const& rhs ) const;
		bool operator!=( BundleIdentifier const& rhs ) const;
		bool operator<( BundleIdentifier const& rhs ) const;
		bool operator<=( BundleIdentifier const& rhs ) const;
		bool operator>( BundleIdentifier const& rhs ) const;
		bool operator>=( BundleIdentifier const& rhs ) const;

		friend std::ostream& operator<<( std::ostream &out, BundleIdentifier const& id );

		std::string const& getName() const { return m_Name; }

	private:

		BundleIdentifier( std::string const& name, const unsigned int id );

		std::string			m_Name;
		unsigned int		m_Id;

	};

	class BlockIdentifier
	{

		friend class AbstractBlock;

	public:

		BlockIdentifier();
		BlockIdentifier( BlockIdentifier const& src );
		BlockIdentifier& operator=( BlockIdentifier const& src );

		bool isValid() const;
		bool operator==( BlockIdentifier const& rhs ) const;
		bool operator!=( BlockIdentifier const& rhs ) const;
		bool operator<( BlockIdentifier const& rhs ) const;
		bool operator<=( BlockIdentifier const& rhs ) const;
		bool operator>( BlockIdentifier const& rhs ) const;
		bool operator>=( BlockIdentifier const& rhs ) const;

		std::string const& getName() const { return m_Name; }

	private:

		BlockIdentifier( std::string const& name, const unsigned int id );

		std::string			m_Name;
		unsigned int		m_Id;

	};

}