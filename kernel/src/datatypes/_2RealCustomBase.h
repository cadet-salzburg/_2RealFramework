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
#include "datatypes/_2RealDerivedTypes.h"

namespace _2Real
{
	template< typename TType >
	class ToCustomType : public CustomType
	{

	public:

		ToCustomType() : CustomType()
		{
		}

		static TypeMetadata * getTypeMetadata()
		{
			TypeMetadata *result = nullptr;

			if ( BaseType< TType >::isBaseType() )
			{
				FieldDescriptor *field = new FieldDescriptor_t< TType >( Init< TType >::defaultValue() );
				result = new TypeMetadata();
				result->addField( "default", *field );
			}
			else if ( CustomDerivedType< TType >::isCustomDerived() )
			{
				result = CustomDerivedType< TType >::getTypeMetadata();
			}

			return result;
		}

		ToCustomType( TType const& value ) : CustomType()
		{
			std::shared_ptr< TypeMetadata > metadata( ToCustomType::getTypeMetadata() );
			if ( metadata.get() == nullptr )
			{
				std::ostringstream msg;
				msg << "invalid type: " << typeid( TType ).name() << " is neither framework base type nor custom type derived";
				throw _2Real::Exception( msg.str() );
			}
			else
			{
				CustomType::initFrom( *( metadata.get() ) );
			}
		}

	};
}