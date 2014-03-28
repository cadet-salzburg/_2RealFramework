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

#include "app/_2RealCustomTypeMetainfo.h"
#include "app/_2RealHandleValidity.h"
#include "engine/_2RealSharedTypeMetainfo.h"

namespace _2Real
{
	namespace app
	{

		CustomTypeMetainfo::CustomTypeMetainfo( std::shared_ptr< const SharedTypeMetainfo > meta ) :
			TypeMetainfo( meta ),
			mImpl( meta )
		{
		}

		std::string CustomTypeMetainfo::getName() const
		{
			std::shared_ptr< const SharedTypeMetainfo > meta = checkValidity< const SharedTypeMetainfo >( mImpl, "custom type metainfo" );
			return meta->getName();
		}

		std::string CustomTypeMetainfo::getDescription() const
		{
			std::shared_ptr< const SharedTypeMetainfo > meta = checkValidity< const SharedTypeMetainfo >( mImpl, "custom type metainfo" );
			return meta->getDescription();
		}

		CustomDataItem CustomTypeMetainfo::makeCustomData() const
		{
			std::shared_ptr< const SharedTypeMetainfo > meta = checkValidity< const SharedTypeMetainfo >( mImpl, "custom type metainfo" );
			return meta->makeCustomData();
		}

	}
}