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

#include "helpers/_2RealHelpers.h"

#include <string>
#include <map>

namespace _2Real
{

	template< typename K, typename V >
	class MapInitializer
	{

	public:

		MapInitializer( K const& key, V const& val )
		{
			m_Map[ key ] = val;
		}

		MapInitializer& operator()( K const& key, V const& val )
		{
			m_Map[ key ] = val;
			return *this;
		}

		operator std::map< K, V > const& () const
		{
			return m_Map;
		}

	private:

		std::map< K, V > m_Map;

	};

	typedef MapInitializer< std::string, std::string >			StringEnums;

	template< typename V >
	class Enumeration
	{

	public:

		typedef std::map< std::string, V >	EnumMap;

		Enumeration() {}

		Enumeration( EnumMap const& enums, std::string const& undefinedVal ) :
			m_Content( enums ),
			m_UndefinedValue( undefinedVal )
		{
		}

		V & operator[]( std::string const& key )
		{
			return m_Content[ key ];
		}

		V const& valueFor( std::string const& key ) const
		{
			if ( m_Content.find( key ) != m_Content.end() )
			{
				return m_Content.at( key );
			}

			return m_UndefinedValue;
		}

		friend std::ostream& operator<<( std::ostream& out, Enumeration< V > const& e );

		friend std::istream& operator>>( std::istream& in, Enumeration< V > &e );

	private:

		void writeTo( std::ostream& out ) const
		{
			out << m_Content;
		}

		void readFrom( std::istream& in )
		{
			in >> m_Content;
		}

		EnumMap			m_Content;
		V				m_UndefinedValue;

	};

	template< typename V >
	std::ostream& operator<<( std::ostream& out, Enumeration< V > const& e )
	{
		e.writeTo( out );
		return out;
	}

	template< typename V >
	std::istream& operator>>( std::istream& in, Enumeration< V > &e )
	{
		e.readFrom( in );
		return in;
	}

	typedef Enumeration< std::string >			StringEnumeration;

}
