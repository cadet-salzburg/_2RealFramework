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

#include "_2RealIdentifier.h"

#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>
#include <sstream>
#include <iostream>

#include "Poco/Path.h"

namespace _2Real
{
	/**
	*	to lowercasw
	*/
	const std::string toLower(std::string const& s);

	/**
	*	remove leading & ending whitespaces from a string
	*/
	const std::string trim(std::string const&s , std::string const& whitespace = " \t");

	/**
	*	trims, converts to lower case, checks for bad characters
	*/
	const std::string validateName(std::string const& s);

	/**
	*	converts a poco path to a name that may be used in the system
	*/
	const std::string pathToName(Poco::Path const& path);

	template< typename DataType >
	std::istream& operator>>(std::istream &in, typename std::vector< DataType > &v)
	{
		std::string element;
		char delim = ',';

		while (getline(in, element, delim))
		{
			trim(element);
			DataType tmp;
			std::stringstream stream;
			stream << element;
			stream >> tmp;
			v.push_back(tmp);
		}

		return in;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::set< DataType > &s)
	{
		if (s.empty())
		{
			return out;
		}

		typename std::set< DataType >::const_iterator it = s.begin();
		out << *it;
		++it;
		for (; it != s.end(); ++it)
		{
			out << "\n" << *it;
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::set< DataType * > &s)
	{
		if (s.empty())
		{
			return out;
		}

		typename std::set< DataType >::const_iterator it = s.begin();
		out << **it;
		++it;
		for (; it != s.end(); ++it)
		{
			out << "\n" << **it;
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::vector< DataType > &v)
	{
		if (v.empty())
		{
			return out;
		}

		typename std::vector< DataType >::const_iterator it = v.begin();
		out << *it;
		++it;
		for (; it != v.end(); ++it)
		{
			out << ", " << *it;
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::vector< DataType * > &v)
	{
		if (v.empty())
		{
			return out;
		}

		typename std::vector< DataType >::const_iterator it = v.begin();
		out << **it;
		++it;
		for (; it != v.end(); ++it)
		{
			out << ", " << **it;
		}
		return out;
	}

	template< typename DataType >
	std::istream& operator>>(std::istream &in, typename std::list< DataType > &l)
	{
		std::string element;
		char delim = ',';

		while (getline(in, element, delim))
		{
			trim(element);
			DataType tmp;
			std::stringstream stream;
			stream << element;
			stream >> tmp;
			l.push_back(tmp);
		}

		return in;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::list< DataType > &l)
	{
		if (l.empty())
		{
			return out;
		}

		typename std::list< DataType >::const_iterator it = l.begin();
		out << *it;
		++it;
		for (; it != l.end(); ++it)
		{
			out << ", " << *it;
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::list< DataType * > &l)
	{
		if (l.empty())
		{
			return out;
		}

		typename std::list< DataType >::const_iterator it = l.begin();
		out << **it;
		++it;
		for (; it != l.end(); ++it)
		{
			out << ", " << **it;
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< std::string, DataType > &m)
	{
		if (m.empty())
		{
			return out;
		}

		for (typename std::map< std::string, DataType >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << it->second << "\n";
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< BundleIdentifier, DataType > &m)
	{
		if (m.empty())
		{
			return out;
		}

		for (typename std::map< BundleIdentifier, DataType >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << it->second << "\n";
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< BlockIdentifier, DataType > &m)
	{
		if (m.empty())
		{
			return out;
		}

		for (typename std::map< BlockIdentifier, DataType >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << it->second << "\n";
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< std::string, DataType * > &m)
	{
		if (m.empty())
		{
			return out;
		}

		for (typename std::map< std::string, DataType * >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << *(it->second) << "\n";
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< BundleIdentifier, DataType * > &m)
	{
		if (m.empty())
		{
			return out;
		}

		for (typename std::map< BundleIdentifier, DataType * >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << *it->second << "\n";
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< BlockIdentifier, DataType * > &m)
	{
		if (m.empty())
		{
			return out;
		}

		for (typename std::map< BlockIdentifier, DataType * >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << *it->second << "\n";
		}
		return out;
	}

}