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

#include "datatypes/_2RealTypeConverter.h"

namespace _2Real
{
	TypeConverter::TypeConverter()
	{
	}

	TypeConverter::~TypeConverter()
	{
	}

	std::shared_ptr< const CustomType > TypeConverter::convert( std::shared_ptr< const CustomType > data ) const
	{
		// empty custom type
		CustomType *t = new CustomType( nullptr );

		// set name to other type name
		t->mTypeId = mTypeId;
		// change all fields
		for ( CustomType::DataFields::const_iterator it = data->mDataFields.begin(); it != data->mDataFields.end(); ++it )
		{
			std::string oldname = it->first;
			// check how the field should be named
			std::map< std::string, std::string >::const_iterator lit = mLookupTable.find( oldname );
#ifdef _DEBUG
			assert( lit != mLookupTable.end() );
#endif
			std::string newname = lit->second;
			// insert under new field name
			t->mDataFields[ newname ] = it->second;
		}

		return std::shared_ptr< const CustomType >( t );
	}

}