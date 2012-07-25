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

#include "datatypes/_2RealNumber.h"
#include "datatypes/_2RealPoint.h"

#include <typeinfo.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>

namespace _2Real
{
	template< typename TData >
	inline void writeTo( std::ostream &out, TData const& v )
	{
		std::string name = typeid( TData ).name();
		for ( unsigned int i=0; i<name.length(); ++i )
		{
			if ( name[i] == ' ' ) name[i] = '#';
		}
		out << "UNSTREAMABLE " << name;
	}

	template< typename TData >
	inline void writeTo( std::ostream &out, typename std::vector< TData > const& v )
	{
		if ( v.empty() )
		{
			return;
		}

		typename std::vector< TData >::const_iterator it = v.begin();
		writeTo( out, *it );
		++it;
		for ( ; it != v.end(); ++it )
		{
			out << ", ";
			writeTo( out, *it );
		}
	}

	template< typename TData >
	inline void writeTo( std::ostream &out, typename std::list< TData > const& v )
	{
		if ( v.empty() )
		{
			return;
		}

		typename std::list< TData >::const_iterator it = v.begin();
		writeTo( out, *it );
		++it;
		for ( ; it != v.end(); ++it )
		{
			out << ", ";
			writeTo( out, *it );
		}
	}

	template< >
	inline void writeTo( std::ostream &out, Number const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, Point const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, char const& v )
	{
		out << static_cast< int >( v );
	}

	template< >
	inline void writeTo( std::ostream &out, unsigned char const& v )
	{
		out << static_cast< int >( v );
	}

	template< >
	inline void writeTo( std::ostream &out, short const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, unsigned short const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, int const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, unsigned int const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, long const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, unsigned long const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, bool const& v )
	{
		out << v;
	}

	template< >
	inline void writeTo( std::ostream &out, std::string const& v )
	{
		out << v;
	}

	template< typename TData >
	inline void readFrom( std::istream &in, TData &v )
	{
		std::string unstreamable;
		std::string typeName;
		in >> unstreamable >> typeName;
		for ( unsigned int i=0; i<typeName.length(); ++i )
		{
			if ( typeName[i] == '#' ) typeName[i] = ' ';
		}

		std::cout << unstreamable << " " << typeName << std::endl;
	}

	template< typename TData >
	inline void readFrom( std::istream &in, typename std::vector< TData > &v )
	{
		std::string element;

		while ( getline( in, element, ',' ) )
		{
			trim( element );
			TData tmp;
			std::stringstream stream;
			stream << element;
			readFrom( stream, tmp );
			v.push_back( tmp );
		}
	}

	template< typename TData >
	inline void readFrom( std::istream &in, typename std::list< TData > &v )
	{
		std::string element;

		while ( getline( in, element, ',' ) )
		{
			trim( element );
			TData tmp;
			std::stringstream stream;
			stream << element;
			readFrom( stream, tmp );
			v.push_back( tmp );
		}
	}

	template< >
	inline void readFrom( std::istream &in, Number &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, Point &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, char &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, unsigned char &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, short &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, unsigned short &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, int &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, unsigned int &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, long &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, unsigned long &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, bool &v )
	{
		in >> v;
	}

	template< >
	inline void readFrom( std::istream &in, std::string &v )
	{
		in >> v;
	}
}