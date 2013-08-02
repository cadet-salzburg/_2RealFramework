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
	std::shared_ptr< CustomType > TypeConverter::convert( std::shared_ptr< const CustomType > data ) const
	{
		// create empty custom type
		std::shared_ptr< CustomType > newtype( new CustomType() );

		// set name to other type name
		newtype->mTypeId = mTypeId;
		// change all fields
		for ( CustomType::DataFields::const_iterator it = data->mDataFields.begin(); it != data->mDataFields.end(); ++it )
		{
			// argh
			AbstractAnyHolder *value = it->second;
			std::string name = it->first;

			std::map< std::string, Conversion >::const_iterator lookupIt = mLookupTable.find( name );
#ifdef _DEBUG
			assert( lookupIt != mLookupTable.end() );
#endif

			// might have new name, might have entirely new type
			Conversion conversion = lookupIt->second;
			if ( nullptr == conversion.second.get() )
			{
				// if no converter, i know that this is a simple field
				// so just change the name
				AbstractAnyHolder *copied = value->copy_create();
				newtype->mDataFields[ conversion.first ] = copied;
			}
			else
			{
				// so it's a complex type....
				AbstractAnyHolder *copied = value->copy_create();
				std::shared_ptr< const CustomType > subfield = extract< CustomType >( *copied );
				std::shared_ptr< CustomType > converted = conversion.second->convert( subfield );
				AbstractAnyHolder *a = new AnyHolder< CustomType >( converted );
				newtype->mDataFields[ conversion.first ] = a;
				delete copied;
			}
		}

		return newtype;
	}

}