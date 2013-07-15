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
#include "datatypes/_2RealBaseTypes.h"

#include <iostream>

namespace _2Real
{
	template< typename TType >
	class BaseToCustomType
	{

	public:

		static TypeMetadata * getTypeMetadata()
		{
			TypeMetadata *result = nullptr;

			if ( BaseType< TType >::isBaseType() )
			{
				FieldDescriptor *desc = DataField< TType >::createFieldDescriptor( Init< TType >::defaultValue() );
				result = new TypeMetadata( Name< TType >::humanReadableName() );
				result->addField( "default", desc );
				return result;
			}
			else
			{
				std::stringstream msg;
				msg << typeid( TType ).name() << " is not a framework base type, thus no metadata can be derived" << std::endl;
				throw _2Real::Exception( msg.str() );
			}
		}

		BaseToCustomType( TType const& value ) : mData( nullptr )
		{
			TypeMetadata *meta( BaseToCustomType< TType >::getTypeMetadata() );
			CustomType *t = new CustomType( meta );
			t->set( "default", value );
			mData.reset( t );
			delete meta;
		}

		operator std::shared_ptr< const CustomType > () const { return mData; }

	private:

		std::shared_ptr< const CustomType >		mData;

	};
}