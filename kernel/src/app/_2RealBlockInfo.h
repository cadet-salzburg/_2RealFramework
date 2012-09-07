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

#include "app/_2RealParameterInfo.h"

#include <vector>
#include <string>

namespace _2Real
{
	namespace app
	{
		struct BlockInfo
		{
			typedef std::vector< InletInfo >					InletInfos;
			typedef std::vector< InletInfo >::iterator			InletInfoIterator;
			typedef std::vector< InletInfo >::const_iterator	InletInfoConstIterator;
			typedef std::vector< OutletInfo >					OutletInfos;
			typedef std::vector< OutletInfo >::iterator			OutletInfoIterator;
			typedef std::vector< OutletInfo >::const_iterator	OutletInfoConstIterator;

			BlockInfo() : name( "undefined" ), description( "undefined" ), category( "undefined" ), inlets(), outlets() {}

			std::string			name;
			std::string			description;
			std::string			category;
			InletInfos			inlets;
			OutletInfos			outlets;
		};
	}
}