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

#include "datatypes/_2RealAbstractFieldDescriptor.h"
#include "datatypes/_2RealAnyHolder.h"
#include "datatypes/_2RealDataField.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	template< typename TType >
	struct FieldDesc;

	template< typename TType >
	class FieldDescriptor_t : public FieldDescriptor
	{

	public:

		~FieldDescriptor_t() {}
		AbstractAnyHolder * createAnyHolder() const { return new AnyHolder< TType >( mInitValue ); }
		DataFieldRef getField() const { return mField; }

	private:

		friend struct FieldDesc< TType >;

		FieldDescriptor_t( TType const& initValue, DataFieldRef field ) :
			FieldDescriptor(),
			mInitValue( initValue ),
			mField( field )
		{
		}

		DataFieldRef				mField;
		TType						mInitValue;

	};
}
