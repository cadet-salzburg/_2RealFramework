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

#include "helpers/_2RealAnyHolder.h"
#include <map>

namespace _2Real
{
	class TypeMetadata;

	namespace bundle
	{
		class TypeMetainfo;
	}

	namespace app
	{
		class TypeMetainfo;
	}

	class CustomType
	{

	public:

		// creation -> allocate new ptrs
		explicit CustomType( bundle::TypeMetainfo const& meta );
		explicit CustomType( app::TypeMetainfo const& meta );
		explicit CustomType( TypeMetadata const& meta );

		// copy -> same ptrs as other obj
		CustomType( CustomType const& other );

		~CustomType();

		// clone -> allocate new ptrs
		void cloneFrom( CustomType const& other );

		template< typename TType >
		void set( std::string const& field, TType const& value )
		{
			// makes a copy of the value!
			setValueInternal( field, new AnyHolder< TType >( value ) );
		}

		template< typename TType >
		TType const& get( std::string const& field ) const
		{
			AbstractAnyHolder const* value = getValueInternal( field );
			// may throw
			TType const& result = value->extract< TType >();
			return result;
		}

		// called by getCurrentValueAsString on an inlet -> what the fuck
		void writeTo( std::ostringstream &out ) const;
		// called by hasChanged() on inlet handles
		bool isEqualTo( CustomType const& other ) const;

		unsigned int size() const { return mDataFields.size(); }

	private:

		CustomType& operator=( CustomType const& other );

		typedef std::map< std::string, _2Real::AbstractAnyHolder * >		DataFields;
		DataFields															mDataFields;

		void initField( std::string const& name, AbstractAnyHolder *init );
		DataFields::iterator iter( std::string const& name );
		DataFields::const_iterator constIter( std::string const& name ) const;

		void setValueInternal( std::string const& field, AbstractAnyHolder *value );
		AbstractAnyHolder const* getValueInternal( std::string const& field ) const;

	};
}
