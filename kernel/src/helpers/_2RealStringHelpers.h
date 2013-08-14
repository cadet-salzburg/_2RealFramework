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

#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	std::string toLower( std::string const& s );
	std::string trim( std::string const& s, std::string const& whitespaces = " \t\n" );
	void checkChars( std::string const& s, std::string const& validChars = "abcedefghijklmnopqrstuvwxyz_0123456789- " );

	struct LowercaseCmp
	{
		bool operator()( std::string const& s0, std::string const& s1 ) const
		{
			return ( toLower( s0 ) < toLower( s1 ) );
		}
	};

}