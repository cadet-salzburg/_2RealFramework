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

#include "datatypes/_2RealFieldDescriptor.h"
#include "helpers/_2RealException.h"
#include "datatypes/_2RealDataField.h"

#include <string>
#include <map>

namespace _2Real
{
	class TypeMetadata;
	class TypeRegistry;

	namespace bundle
	{
		class TypeMetainfo
		{

		public:

			TypeMetainfo( TypeMetadata *meta );
			~TypeMetainfo();

			template< typename TType >
			void addField( std::string const& name, TType const& init = Init< TType >::defaultValue() )
			{
				std::shared_ptr< const FieldDescriptor > desc( DataField< TType >::createFieldDescriptor( name, init ) );
				if ( desc == nullptr )
					throw Exception( "this type is not allowed as a field" );
				else
					addFieldInternal( name, Name< TType >::humanReadableName(), desc );
			}

			void addCustomTypeField( std::string const& name, std::string const& type );

		private:

			TypeMetainfo& operator=( TypeMetainfo const& other );
			TypeMetainfo( TypeMetainfo const& other );

			friend class CustomType;
			TypeMetadata								*mImpl;

			void addFieldInternal( std::string const& name, std::string const& type, std::shared_ptr< const FieldDescriptor > desc );

		};
	}
}