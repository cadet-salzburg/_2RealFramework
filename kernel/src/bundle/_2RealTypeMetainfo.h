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

#include <string>

namespace _2Real
{
	class TypeMetadata;

	namespace bundle
	{
		class TypeMetainfo
		{

		public:

			TypeMetainfo( TypeMetadata &meta );
			~TypeMetainfo();

			template< typename TType >
			void addField( std::string const& name )
			{
				addFieldInternal( name, new FieldDescriptor_t< TType >( Init< TType >::defaultValue() ) );
			}

		private:

			friend class CustomType;
			TypeMetadata		&mImpl;

			void addFieldInternal( std::string const& name, FieldDescriptor *desc );

		};
	}
}