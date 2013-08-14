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

#include "app/_2RealOutletMetainfo.h"
#include "app/_2RealTypeMetainfo.h"
#include "engine/_2RealIOMetadata.h"

namespace _2Real
{
	namespace app
	{
		OutputMetainfo::OutputMetainfo() :
			mImpl()
		{
		}

		OutputMetainfo::OutputMetainfo( std::shared_ptr< const IOMetadata > meta ) :
			mImpl( meta )
		{
		}

		bool OutputMetainfo::isValid() const
		{
			std::shared_ptr< const IOMetadata > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		std::string const& OutputMetainfo::getName() const
		{
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "output metainfo" );
			return meta->identifier->getObjectName();
		}

		std::string const& OutputMetainfo::getTypeName() const
		{
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "output metainfo" );
			return meta->typeMetadata->getHumanReadableName();
		}

		TypeMetainfo OutputMetainfo::getTypeMetainfo() const
		{
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "output metainfo" );
			return TypeMetainfo( meta->typeMetadata );
		}

		//bool OutputMetainfo::canExpand() const
		//{
		//	std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
		//	return false;
		//}

		//bool OutputMetainfo::canLink() const
		//{
		//	std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
		//	return true;
		//}
	}
}