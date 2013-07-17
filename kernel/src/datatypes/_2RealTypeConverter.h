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

#include "datatypes/_2RealCustomData.h"
#include <string>
#include <map>

namespace _2Real
{
	class TypeConverter
	{
	public:

		typedef std::pair< std::string, std::shared_ptr< const TypeConverter > > Conversion;

		// convert a type
		std::shared_ptr< CustomType > convert( std::shared_ptr< const CustomType > ) const;

		// for each field in the type:
		//		either a new name ( simple field )
		//		or another type converter ( complex field )
		std::map< std::string, Conversion >		mLookupTable;
		// must also rename the custom type
		std::pair < std::string, std::string >	mTypeId;

	};
}