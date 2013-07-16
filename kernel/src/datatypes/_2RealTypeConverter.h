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

		TypeConverter();
		~TypeConverter();

		std::shared_ptr< const CustomType > convert( std::shared_ptr< const CustomType > ) const;

	private:

		std::pair< std::string, std::string >		mTypeId;
		std::map< std::string, std::string >		mLookupTable;

	};
}