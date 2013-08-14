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

#include "app/_2RealInletMetainfo.h"
#include "app/_2RealTypeMetainfo.h"
#include "engine/_2RealIOMetadata.h"

namespace _2Real
{
	namespace app
	{
		InputMetainfo::InputMetainfo() :
			mImpl()
		{
		}

		InputMetainfo::InputMetainfo( std::shared_ptr< const IOMetadata > meta ) :
			mImpl( meta )
		{
		}

		bool InputMetainfo::isValid() const
		{
			std::shared_ptr< const IOMetadata > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		std::string const& InputMetainfo::getName() const
		{
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
			return meta->identifier->getObjectName();
		}

		std::string const& InputMetainfo::getTypeName() const
		{
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
			return meta->typeMetadata->getHumanReadableName();
		}

		TypeMetainfo InputMetainfo::getTypeMetainfo() const
		{
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
			return TypeMetainfo( meta->typeMetadata );
		}

		bool InputMetainfo::canExpand() const
		{
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
			return meta->canExpand;
		}

		//bool InputMetainfo::canLink() const
		//{
		//	std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
		//	return meta->canLink;
		//}

		//bool InputMetainfo::isBuffered() const
		//{
		//	std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
		//	return meta->isBuffered;
		//}
	}
}