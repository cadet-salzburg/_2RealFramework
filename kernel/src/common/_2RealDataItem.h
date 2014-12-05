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

#include "common/_2RealBoostDefines.h"
#include "boost/variant.hpp"

#include "common/_2RealStdIncludes.h"

namespace _2Real
{
	class CustomDataItem;

	/*
	*	represents a data item within the engine; this used to be represented via an 'any' datatype,
	*	i.e. type erasure - now we use boost::variant with all it's template magic glory -> this fits
	*	our data model a lot better. since a custom data item is a data item, while at the same time
	*	consiting of several data items, boost::recursive_wrapper needs to be used here.
	*/

	/*
	*	boost::variants are never empty: an unitialized data item while thus contain an uint8_t
	*	also, there's a max amount of types, defined by BOOST_MPL_LIMIT_LIST_SIZE, default: 20
	*	i've changed it here to 40, and if necessary, it can be further increased.
	*/

	typedef boost::variant< uint8_t,		std::vector< uint8_t >,
							int8_t,			std::vector< int8_t >,
							uint16_t,		std::vector< uint16_t >,
							int16_t,		std::vector< int16_t >,
							uint32_t,		std::vector< uint32_t >,
							int32_t,		std::vector< int32_t >,
							uint64_t,		std::vector< uint64_t >,
							int64_t,		std::vector< int64_t >,
							double,			std::vector< double >,
							float,			std::vector< float >,
							std::string,	std::vector< std::string >,
							bool,			std::vector< bool >,
							boost::recursive_wrapper< CustomDataItem > > DataItem;

}