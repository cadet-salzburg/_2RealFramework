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

#include "helpers/_2RealStdIncludes.h"
#include "engine/_2RealFieldDescriptor.h"

namespace _2Real
{
	class CustomData;
	class SharedTypeMetainfo;

	namespace bundle
	{
		class TypeMetainfo
		{

		public:

			explicit TypeMetainfo( std::shared_ptr< SharedTypeMetainfo > );

			// add fields of basic types
			template< typename TType >
			void addBasicField( std::string const& fieldName, TType const& init = FrameworkCompatibleType< TType >::defaultValue() )
			{
				std::shared_ptr< const AbstractFieldDescriptor > desc( new FieldDescriptor_T< TType >( init ) );
				addFieldInternal( fieldName, FrameworkCompatibleType< TType >::humanReadableName(), desc );
			}

			// add fields of custom types
			void addField( std::string const& fieldName, std::string const& typeName );

			// creates actual data
			std::shared_ptr< CustomData > makeData();

			void exportType();

		private:

			void addFieldInternal( std::string const& fieldName, std::string const& typeName, std::shared_ptr< const AbstractFieldDescriptor > );

			std::weak_ptr< SharedTypeMetainfo >		mImpl;

		};
	}
}