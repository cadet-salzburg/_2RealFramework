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

#include "bundle/_2RealBlockMetainfo.h"
#include "bundle/_2RealTypeMetainfo.h"
#include "engine/_2RealBlockMetadata.h"
#include "engine/_2RealIOMetadata.h"
#include "helpers/_2RealStringHelpers.h"

namespace _2Real
{
	namespace bundle
	{
		FunctionBlockMetainfo::FunctionBlockMetainfo( std::shared_ptr< BlockMetadata > meta ) :
			mImpl( meta )
		{
		}

		void FunctionBlockMetainfo::setDescription( std::string const& description )
		{
			std::shared_ptr< BlockMetadata > block = mImpl.lock();
			block->setDescription( description );
		}

		void FunctionBlockMetainfo::setCategory( std::string const& category )
		{
			std::shared_ptr< BlockMetadata > block = mImpl.lock();
			block->setDescription( category );
		}

		void FunctionBlockMetainfo::setThreadingPolicy( ThreadingPolicy const& policy )
		{
			std::shared_ptr< BlockMetadata > block = mImpl.lock();
			block->setThreadingPolicy( policy );
		}

		void FunctionBlockMetainfo::addCustomTypeInlet( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init, UpdatePolicy const& defaultPolicy )
		{
			privateAddInlet( name, type, init, nullptr, defaultPolicy );
		}

		void FunctionBlockMetainfo::privateAddInlet( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init, std::shared_ptr< const TypeMetadata > meta, UpdatePolicy const& defaultPolicy )
		{
			std::string trimmed = trim( name );
			checkChars( toLower( trimmed ) );

			// copy for safety reasons
			std::shared_ptr< const CustomType > copied;
			if ( init.get() )
				copied.reset( new CustomType( *( init.get() ) ) );

			std::shared_ptr< IOMetadata > data( new IOMetadata );

			data->name = trimmed;
			data->typeMetadata = meta;
			data->initializer = copied;
			data->canExpand = false;
			data->expansionSize = 0;
			data->canLink = true;
			data->isBuffered = true;
			data->bufferSize = 10;
			data->canTriggerUpdates = true;
			data->updatePolicy = defaultPolicy;
			data->synchronizationFlags = IOMetadata::SYNC_ON_UPDATE;

			std::shared_ptr< BlockMetadata > block = mImpl.lock();
			block->addInlet( data, type );
		}

		void FunctionBlockMetainfo::addCustomTypeParameter( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init )
		{
			privateAddParameter( name, type, init, nullptr );
		}

		void FunctionBlockMetainfo::privateAddParameter( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init, std::shared_ptr< const TypeMetadata > meta )
		{
			std::string trimmed = trim( name );
			checkChars( toLower( trimmed ) );

			// copy for safety reasons
			std::shared_ptr< const CustomType > copied;
			if ( init.get() )
				copied.reset( new CustomType( *( init.get() ) ) );

			std::shared_ptr< IOMetadata > data( new IOMetadata );

			data->name = trimmed;
			data->typeMetadata = meta;
			data->initializer = copied;
			data->canExpand = false;
			data->expansionSize = 0;
			data->canLink = false;
			data->isBuffered = false;
			data->bufferSize = 0;
			data->canTriggerUpdates = false;
			data->updatePolicy = UpdatePolicy::INVALID;
			data->synchronizationFlags = IOMetadata::SYNC_ON_UPDATE;

			std::shared_ptr< BlockMetadata > block = mImpl.lock();
			block->addParameter( data, type );
		}

		void FunctionBlockMetainfo::addCustomTypeOutlet( std::string const& name, std::string const& type )
		{
			privateAddOutlet( name, type, nullptr );
		}

		void FunctionBlockMetainfo::privateAddOutlet( std::string const& name, std::string const& type, std::shared_ptr< const TypeMetadata > meta )
		{
			std::string trimmed = trim( name );
			checkChars( toLower( trimmed ) );

			std::shared_ptr< IOMetadata > data( new IOMetadata );

			data->name = trimmed;
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

			std::shared_ptr< BlockMetadata > block = mImpl.lock();
			block->addOutlet( data, type );
		}
	}
}