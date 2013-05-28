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

#include "helpers/_2RealAny.h"
#include "helpers/_2RealAnyHolder.h"

#include <map>

namespace _2Real
{

	// this alone won't work... i need to be able to create shit from the app side
	namespace bundle
	{
		class TypeMetainfo;
	}

	class CustomType
	{

	public:

		explicit CustomType( bundle::TypeMetainfo const& meta );
		CustomType( CustomType const& other );
		CustomType& operator=( CustomType const& other );

		template< typename TType >
		void set( std::string const& field, TType const& value )
		{
			// makes a copy of the value!
			setValueInternal( field, Any( value ) );
		}

		//	TODO: set + range
		//	TODO: set + set of valid values

		// todo: extract ( inlet handle no longer extracts, since all inlets / oulets now hold CustomTypes )

		bool isNull() const;

		void writeTo( std::ostringstream &out ) const;

		bool isEqualTo( CustomType const& other ) const;
		void cloneFrom( CustomType const& other );

	private:

		typedef std::map< std::string, Any >		DataFields;
		DataFields									mDataFields;

		void initField( std::string const& name, Any &init );
		DataFields::iterator iter( std::string const& name );
		DataFields::const_iterator constIter( std::string const& name ) const;

		void setValueInternal( std::string const& field, Any &any );

	};
}
