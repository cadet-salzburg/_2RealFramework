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

#include "bundle/_2RealTypeMetainfo.h"
#include "engine/_2RealTypeMetadata.h"
#include "datatypes/_2RealTypeRegistry.h"

#include <sstream>

namespace _2Real
{
	namespace bundle
	{
		TypeMetainfo::TypeMetainfo( TypeMetadata *meta ) : mImpl( meta )
		{
		}

		TypeMetainfo::~TypeMetainfo()
		{
		}

		void TypeMetainfo::addFieldInternal( std::string const& name, std::string const& type, FieldDescriptor const* desc )
		{
			mImpl->addField( name, type, desc );
		}

		void TypeMetainfo::addCustomTypeField( std::string const& name, std::string const& type )
		{
			mImpl->addField( name, type, nullptr );
		}
	}
}