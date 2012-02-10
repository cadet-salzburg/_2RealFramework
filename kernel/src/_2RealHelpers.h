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
#include <string>
#include <sstream>
#include <iostream>

namespace _2Real
{
	/**
	*	to lowercasw
	*/
	inline std::string toLower(std::string const& s)
	{
		std::string result;
		result.clear();
		for (unsigned int i=0; i<s.length(); i++)
		{
			if (s[i] == ' ')
			{
				result.push_back('_');
			}
			else
			{
				result.push_back(tolower(s[i]));
			}
		}
		return result;
	}

	/**
	*	remove leading & ending whitespace or tab from a string
	*/
	inline std::string trim(std::string const&s , std::string const& whitespace = " \t")
	{
		const size_t beginStr = s.find_first_not_of(whitespace);

		if (beginStr == std::string::npos)
		{
			return "";
		}

		const size_t endStr = s.find_last_not_of(whitespace);
		const size_t range = endStr - beginStr + 1;

		std::string result = s.substr(beginStr, range);
		return result;
	}


	/**
	*	trims, converts to lower case, checks for bad characters
	*/
	inline const std::string validateName(std::string const& s)
	{
		std::string result = toLower(trim(s));

		size_t pos = result.find_first_not_of("abcedefghijklmnopqrstuvwxyz_:0123456789");
		if (pos != std::string::npos)
		{
			std::stringstream msg;
			msg << "invalid name " << result << "; contains forbidden character " << result[pos];
			throw InvalidNameException(msg.str());
		}

		return result;
	}

	/**
	*	input operator for std::vector
	*	the comma, followed by a blank space (', ') shall be used as delimiter
	*/
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
	std::ostream& operator<<(std::ostream &out, typename const std::vector< DataType > &v)
	{
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
		typename std::vector< DataType >::const_iterator it = v.begin();
		out << *it;
		++it;
		for (; it != v.end(); ++it)
		{
			out << ", " << *it;
		}
		return out;
	}

	/**
	*	input operator for std::list
	*	the comma, followed by a blank space (', ') shall be used as delimiter
	*/
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
	std::ostream& operator<<(std::ostream &out, typename const std::map< std::string, DataType > &m)
	{
		for (typename std::map< DataType >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << it->second << "\n";
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< Identifier, DataType > &m)
	{
		for (typename std::map< DataType >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << it->second << "\n";
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< std::string, DataType * > &m)
	{
		for (typename std::map< std::string, DataType * >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << *(it->second) << "\n";
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< Identifier, DataType * > &m)
	{
		for (typename std::map< Identifier, DataType * >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << *it->second << "\n";
		}
		return out;
	}

}