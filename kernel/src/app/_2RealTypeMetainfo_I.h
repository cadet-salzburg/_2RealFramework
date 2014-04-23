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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealData.h"

/*
*	base class for type metainfo:
*	use isBasicType to find out if you should cast to TypeMetainfo or to CustomTypeMetainfo
*	( TypeMetainfo is for simple types, CustomTypeMetainfo is for complex types )
*/

namespace _2Real
{
	namespace app
	{
		class TypeMetainfo_I
		{

		public:

			virtual ~TypeMetainfo_I() = default;

			/*
			*	@return: true is underlying object is valid
			*/
			virtual bool isValid() const = 0;

			/*
			*	@return: true if the type is within the set types that make up our DataItem, except for CustomType
			*	( see common/_2RealDataItem.h )
			*/
			virtual bool isBasicType() const = 0;

			/*
			*	@return: the name
			*/
			virtual std::string getName() const = 0;

			/*
			*	@return: a boost::variant of the correct type
			*/
			virtual DataItem makeData() const = 0;

		};
	}
}