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

#include <typeinfo.h>
#include <string>
#include <iostream>

namespace _2Real
{
	template< typename TData >
	inline std::ostream & writeTo( std::ostream &out, TData const& v )
	{
		out << "UNSERIALIZABLE " << typeid( TData ).name();
		return out;
	}

	template< >
	inline std::ostream & writeTo< char >( std::ostream &out, char const& v )
	{
		out << static_cast< int >( v );
		return out;
	}

	template< >
	inline std::ostream & writeTo< unsigned char >( std::ostream &out, unsigned char const& v )
	{
		out << static_cast< int >( v );
		return out;
	}

	template< >
	inline std::ostream & writeTo< short >( std::ostream &out, short const& v )
	{
		out << v;
		return out;
	}

	template< >
	inline std::ostream & writeTo< unsigned short >( std::ostream &out, unsigned short const& v )
	{
		out << v;
		return out;
	}

	template< >
	inline std::ostream & writeTo< int >( std::ostream &out, int const& v )
	{
		out << v;
		return out;
	}

	template< >
	inline std::ostream & writeTo< unsigned int >( std::ostream &out, unsigned int const& v )
	{
		out << v;
		return out;
	}

	template< >
	inline std::ostream & writeTo< long >( std::ostream &out, long const& v )
	{
		out << v;
		return out;
	}

	template< >
	inline std::ostream & writeTo< unsigned long >( std::ostream &out, unsigned long const& v )
	{
		out << v;
		return out;
	}

	template< >
	inline std::ostream & writeTo< bool >( std::ostream &out, bool const& v )
	{
		out << v;
		return out;
	}

	template< >
	inline std::ostream & writeTo< std::string >( std::ostream &out, std::string const& v )
	{
		out << v;
		return out;
	}

	template< typename TData >
	inline std::istream & readFrom( std::istream &in, TData &v )
	{
		std::string unserializable;
		std::string name;
		in >> unserializable >> name;
		return in;
	}

	template< >
	inline std::istream & readFrom< char >( std::istream &in, char &v )
	{
		in >> v;
		return in;
	}

	template< >
	inline std::istream & readFrom< unsigned char >( std::istream &in, unsigned char &v )
	{
		in >> v;
		return in;
	}

	template< >
	inline std::istream & readFrom< short >( std::istream &in, short &v )
	{
		in >> v;
		return in;
	}

	template< >
	inline std::istream & readFrom< unsigned short >( std::istream &in, unsigned short &v )
	{
		in >> v;
		return in;
	}

	template< >
	inline std::istream & readFrom< int >( std::istream &in, int &v )
	{
		in >> v;
		return in;
	}

	template< >
	inline std::istream & readFrom< unsigned int >( std::istream &in, unsigned int &v )
	{
		in >> v;
		return in;
	}

	template< >
	inline std::istream & readFrom< long >( std::istream &in, long &v )
	{
		in >> v;
		return in;
	}

	template< >
	inline std::istream & readFrom< unsigned long >( std::istream &in, unsigned long &v )
	{
		in >> v;
		return in;
	}

	template< >
	inline std::istream & readFrom< bool >( std::istream &in, bool &v )
	{
		in >> v;
		return in;
	}

	template< >
	inline std::istream & readFrom< std::string >( std::istream &in, std::string &v )
	{
		in >> v;
		return in;
	}
}