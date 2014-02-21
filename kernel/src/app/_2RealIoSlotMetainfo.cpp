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

#include "app/_2RealIoSlotMetainfo.h"
#include "app/_2RealHandleValidity.h"
#include "engine/_2RealSharedServiceIoSlotMetainfo.h"

namespace _2Real
{
	namespace app
	{
		IoSlotMetainfo::IoSlotMetainfo() :
			mImpl()
		{
		}

		IoSlotMetainfo::IoSlotMetainfo( std::shared_ptr< const SharedServiceIoSlotMetainfo > meta ) :
			mImpl( meta )
		{
		}

		IoSlotMetainfo::~IoSlotMetainfo()
		{
		}

		bool IoSlotMetainfo::isValid() const
		{
			std::shared_ptr< const SharedServiceIoSlotMetainfo > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		std::string const& IoSlotMetainfo::getName() const
		{
			std::shared_ptr< const SharedServiceIoSlotMetainfo > meta = checkValidity< const SharedServiceIoSlotMetainfo >( mImpl, "io slot metainfo" );
			return meta->getName();
		}

		std::string const& IoSlotMetainfo::getDescription() const
		{
			std::shared_ptr< const SharedServiceIoSlotMetainfo > meta = checkValidity< const SharedServiceIoSlotMetainfo >( mImpl, "io slot metainfo" );
			return meta->getDescription();
		}

		std::string const& IoSlotMetainfo::getDatatype() const
		{
			std::shared_ptr< const SharedServiceIoSlotMetainfo > meta = checkValidity< const SharedServiceIoSlotMetainfo >( mImpl, "io slot metainfo" );
			return meta->getDatatype();
		}
	}
}