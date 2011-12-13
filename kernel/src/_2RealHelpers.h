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
#include <sstream>

namespace _2Real
{

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::vector< DataType > &v)
	{
		for (typename std::vector< DataType >::const_iterator it = v.begin(); it != v.end(); ++it)
		{
			out << " " << *it;
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::vector< DataType * > &v)
	{
		for (typename std::vector< DataType * >::const_iterator it = v.begin(); it != v.end(); ++it)
		{
			out << " " << *it;
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< std::string, DataType > &m)
	{
		for (typename std::map< DataType >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << it->second << std::endl;
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< Identifier, DataType > &m)
	{
		for (typename std::map< DataType >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << it->second << std::endl;
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< std::string, DataType * > &m)
	{
		for (typename std::map< std::string, DataType * >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << *it->second << std::endl;
		}
		return out;
	}

	template< typename DataType >
	std::ostream& operator<<(std::ostream &out, typename const std::map< Identifier, DataType * > &m)
	{
		for (typename std::map< Identifier, DataType * >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << *it->second << std::endl;
		}
		return out;
	}


}