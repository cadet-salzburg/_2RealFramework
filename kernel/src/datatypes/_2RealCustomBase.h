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
#include "engine/_2RealTypeMetadata.h"
#include "datatypes/_2RealTypeRegistry.h"

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

			std::shared_ptr< const FieldDescriptor > desc( DataField< TType >::createFieldDescriptor( "default", Init< TType >::defaultValue() ) );

			if ( !desc )
			{
				std::stringstream msg;
				msg << typeid( TType ).name() << " is not a framework base type, thus no conversion to a custom type exists" << std::endl;
				throw _2Real::Exception( msg.str() );
			}

			result = new TypeMetadata( TypeMetadata::TypeId( "basic type", Name< TType >::humanReadableName() ), nullptr );
			result->addField( "default", TypeMetadata::TypeId( "", Name< TType >::humanReadableName() ), desc );
			return result;
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

		TypeMetadata * getTypeMetadataInternal();
		void initInternal();


	};
}