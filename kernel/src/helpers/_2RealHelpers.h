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

#include "helpers/_2RealStringHelpers.h"

#include <vector>
#include <map>
#include <list>
#include <string>
#include <sstream>

namespace _2Real
{
	template< class T > void safeDelete( T*& pVal )
	{
		delete pVal;
		pVal = nullptr;
	}

	template< class T > void safeDeleteArray( T*& pVal )
	{
		delete[] pVal;
		pVal = nullptr;
	}

	template< typename TData >
	std::ostream& operator<<( std::ostream &out, typename std::vector< TData > const& v )
	{
		if ( v.empty() )
		{
			return out;
		}

		typename std::vector< TData >::const_iterator it = v.begin();
		out << *it;
		++it;
		for ( ; it != v.end(); ++it )
		{
			out << ", " << *it;
		}
		return out;
	}

	template< typename TData >
	std::ostream& operator<<( std::ostream &out, typename std::list< TData > const& l )
	{
		if ( l.empty() )
		{
			return out;
		}

		typename std::list< TData >::const_iterator it = l.begin();
		out << *it;
		++it;
		for ( ; it != l.end(); ++it )
		{
			out << ", " << *it;
		}
		return out;
	}

	template< typename TData >
	std::ostream& operator<<( std::ostream &out, typename std::map< std::string, TData > const& m )
	{
		if ( m.empty() )
		{
			return out;
		}

		typename std::map< std::string, TData >::const_iterator it = m.begin();
		out << "[ " << it->first << " , " << it->second << " ]";
		++it;
		for ( ; it != m.end(); ++it )
		{
			out << "\n[ " << it->first << " , " << it->second << " ]";
		}
		return out;
	}

	template< typename TData >
	std::istream& operator>>( std::istream &in, typename std::list< TData > &l )
	{
		std::string element;
		char delim = ',';

		while ( getline( in, element, delim ) )
		{
			trim( element );
			TData tmp;
			std::stringstream stream;
			stream << element;
			stream >> tmp;
			l.push_back( tmp );
		}

		return in;
	}

	template< typename TData >
	std::istream& operator>>( std::istream &in, typename std::vector< TData > &v )
	{
		std::string element;
		char delim = ',';

		while ( getline( in, element, delim ) )
		{
			trim( element );
			TData tmp;
			std::stringstream stream;
			stream << element;
			stream >> tmp;
			v.push_back( tmp );
		}

		return in;
	}

	template< typename TData >
	std::istream& operator>>( std::istream &in, typename std::map< std::string, TData > &m )
	{
		std::string element;

		while ( getline( in, element ) )
		{
			trim( element );
			std::string key;
			TData tmp;
			std::stringstream stream;
			stream << element;
			char delim;
			stream >> key >> delim >> tmp >> delim;
			m[ key ] = tmp;
		}

		return in;
	}
}