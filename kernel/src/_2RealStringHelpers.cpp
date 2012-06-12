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

#include "_2RealHelpers.h"
#include "_2RealStringHelpers.h"
#include "_2RealException.h"

#include "Poco/Path.h"

namespace _2Real
{

	const std::string validateName(std::string const& s)
	{
		std::string result = toLower(trim(s));

		size_t pos = result.find_first_not_of("abcedefghijklmnopqrstuvwxyz_ .0123456789");
		if (pos != std::string::npos)
		{
			std::stringstream msg;
			msg << "invalid name " << result << "; contains forbidden character " << result[pos];
			throw InvalidNameException(msg.str());
		}

		return result;
	}

	const std::string pathToName(Poco::Path const& path)
	{
		std::string result;

		for (unsigned int i=0; i<path.depth(); ++i)
		{
			std::string dir = toLower(path.directory(i));
			result.append(dir);
			if (i < (path.depth() - 1)) result.append(".");
		}

		//if (path.isFile())
		//{
		//	result.append(toLower(path.getFileName()));
		//}

		return result;
	}

}