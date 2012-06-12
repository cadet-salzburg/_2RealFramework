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

#include "_2RealBlockIdentifier.h"
#include "_2RealBundleIdentifier.h"

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

	const std::string validateName(std::string const& s);
	const std::string pathToName(Poco::Path const& path);

	template< typename Datatype >
	std::ostream& operator<<(std::ostream &out, typename const std::map< BundleIdentifier, Datatype > &m)
	{
		if (m.empty())
		{
			return out;
		}

		for (typename std::map< BundleIdentifier, Datatype >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << it->second << "\n";
		}
		return out;
	}

	template< typename Datatype >
	std::ostream& operator<<(std::ostream &out, typename const std::map< BlockIdentifier, Datatype > &m)
	{
		if (m.empty())
		{
			return out;
		}

		for (typename std::map< BlockIdentifier, Datatype >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << it->second << "\n";
		}
		return out;
	}

	template< typename Datatype >
	std::ostream& operator<<(std::ostream &out, typename const std::map< BundleIdentifier, Datatype * > &m)
	{
		if (m.empty())
		{
			return out;
		}

		for (typename std::map< BundleIdentifier, Datatype * >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << *it->second << "\n";
		}
		return out;
	}

	template< typename Datatype >
	std::ostream& operator<<(std::ostream &out, typename const std::map< BlockIdentifier, Datatype * > &m)
	{
		if (m.empty())
		{
			return out;
		}

		for (typename std::map< BlockIdentifier, Datatype * >::const_iterator it = m.begin(); it != m.end(); ++it)
		{
			out << *it->second << "\n";
		}
		return out;
	}

}