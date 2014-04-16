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
#include "common/_2RealDataItem.h"

namespace _2Real
{
	class DataField
	{

	public:

		DataField( std::string fieldName, DataItem value );
		DataField( DataField const& other );
		DataField( DataField && other );
		DataField& operator=( DataField const& other );
		DataField& operator=( DataField && other );
		~DataField() = default;

		std::string getName() const;
		DataItem const& getValue() const;
		DataItem & getValue();
		void setValue( DataItem value );

	private:

		std::string		mName;
		DataItem		mValue;

	};
}
