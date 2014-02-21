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
#include "engine/_2RealSharedTypeMetainfo.h"

namespace _2Real
{
	namespace bundle
	{

		TypeMetainfo::TypeMetainfo( std::shared_ptr< SharedTypeMetainfo > metadata ) :
			mImpl( metadata )
		{
		}

		void TypeMetainfo::addFieldInternal( std::string const& fieldName, std::string const& typeName, std::shared_ptr< const AbstractFieldDescriptor > desc )
		{
			mImpl.lock()->addField( fieldName, typeName, desc );
		}

		void TypeMetainfo::addField( std::string const& fieldName, std::string const& typeName )
		{
			mImpl.lock()->addField( fieldName, typeName, nullptr );
		}

		std::shared_ptr< CustomData > TypeMetainfo::makeData()
		{
			return mImpl.lock()->makeData();
		}

		void TypeMetainfo::exportType()
		{
			mImpl.lock()->finalize();
		}

	}
}