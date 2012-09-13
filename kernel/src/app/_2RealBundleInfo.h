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

#include "helpers/_2RealVersion.h"
#include "app/_2RealBlockInfo.h"

#include <vector>
#include <string>

namespace _2Real
{
	namespace app
	{
		struct BundleInfo
		{
			typedef std::vector< BlockInfo >					BlockInfos;
			typedef std::vector< BlockInfo >::iterator			BlockInfoIterator;
			typedef std::vector< BlockInfo >::const_iterator	BlockInfoConstIterator;

			BundleInfo() : exportedBlocks(), name( "undefined" ), directory( "undefined" ), description( "undefined" ),
				author( "undefined" ), contact( "undefined" ), category( "undefined" ), version( 0, 0, 0 ) {}

			BlockInfos			exportedBlocks;
			std::string			name;
			std::string			directory;
			std::string			description;
			std::string			author;
			std::string			contact;
			std::string			category;
			Version				version;
		};
	}
}