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

#include "bundle/_2RealContextBlockMetainfo.h"
#include "engine/_2RealBlockMetadata.h"
#include "engine/_2RealIOMetadata.h"
#include "helpers/_2RealStringHelpers.h"

namespace _2Real
{
	namespace bundle
	{
		ContextBlockMetainfo::ContextBlockMetainfo( std::shared_ptr< BlockMetadata > meta ) :
			mImpl( meta )
		{
		}

		void ContextBlockMetainfo::setDescription( std::string const& description )
		{
			std::shared_ptr< BlockMetadata > block = mImpl.lock();
			block->setDescription( description );
		}

		//void ContextBlockMetainfo::setThreadingPolicy( ThreadingPolicy const& policy )
		//{
		//	std::shared_ptr< BlockMetadata > block = mImpl.lock();
		//	block->setThreadingPolicy( policy );
		//}

		void ContextBlockMetainfo::addCustomTypeOutlet( std::string const& name, std::string const& type )
		{
			privateAddOutlet( name, type, nullptr );
		}

		void ContextBlockMetainfo::privateAddOutlet( std::string const& name, std::string const& type, std::shared_ptr< const TypeMetadata > meta )
		{
			std::string trimmed = trim( name );
			checkChars( toLower( trimmed ) );

			std::shared_ptr< IOMetadata > data( new IOMetadata );

			data->identifier = IdGenerator::makeExportedOutletId( mImpl.lock()->getIdentifier(), name );
			data->typeMetadata = meta;
			//data->initializer = copied;
			data->canExpand = false;
			data->expansionSize = 0;
			data->canLink = true;
			data->isBuffered = false;
			data->bufferSize = 0;
			data->canTriggerUpdates = false;
			data->updatePolicy = UpdatePolicy::INVALID;
			data->synchronizationFlags = IOMetadata::SYNC_ON_UPDATE;
			data->typeMetadata = meta;

			std::shared_ptr< BlockMetadata > block = mImpl.lock();
			block->addOutlet( data, type );
		}
	}
}