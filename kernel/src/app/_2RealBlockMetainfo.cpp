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

#include "app/_2RealBlockMetainfo.h"
#include "app/_2RealInletMetainfo.h"
#include "app/_2RealOutletMetainfo.h"
#include "app/_2RealParameterMetainfo.h"
#include "engine/_2RealBlockMetadata.h"

namespace _2Real
{
	namespace app
	{
		BlockMetainfo::BlockMetainfo() :
			mImpl()
		{
		}

		BlockMetainfo::BlockMetainfo( std::shared_ptr< const BlockMetadata > meta ) :
			mImpl( meta )
		{
		}

		bool BlockMetainfo::isValid() const
		{
			std::shared_ptr< const BlockMetadata > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		std::string const& BlockMetainfo::getName() const
		{
			std::shared_ptr< const BlockMetadata > meta = checkValidity< const BlockMetadata  >( mImpl, "block metainfo" );
			return meta->getName();
		}

		std::string const& BlockMetainfo::getDescription() const
		{
			std::shared_ptr< const BlockMetadata > meta = checkValidity< const BlockMetadata  >( mImpl, "block metainfo" );
			return meta->getDescription();
		}

		std::string const& BlockMetainfo::getCategory() const
		{
			std::shared_ptr< const BlockMetadata > meta = checkValidity< const BlockMetadata  >( mImpl, "block metainfo" );
			return meta->getCategory();
		}

		bool BlockMetainfo::isContext() const
		{
			std::shared_ptr< const BlockMetadata > meta = checkValidity< const BlockMetadata  >( mImpl, "block metainfo" );
			return meta->isContext();
		}

		bool BlockMetainfo::needsContext() const
		{
			std::shared_ptr< const BlockMetadata > meta = checkValidity< const BlockMetadata  >( mImpl, "block metainfo" );
			return meta->needsContext();
		}

		void BlockMetainfo::getInletMetainfo( std::vector< InputMetainfo > &metainfos ) const
		{
			std::shared_ptr< const BlockMetadata > meta = checkValidity< const BlockMetadata  >( mImpl, "block metainfo" );
			std::vector< std::shared_ptr< IOMetadata > > const& inlets = meta->getInlets();
			metainfos.clear();
			metainfos.reserve( inlets.size() );
			for ( std::vector< std::shared_ptr< IOMetadata > >::const_iterator it = inlets.begin(); it != inlets.end(); ++it )
				metainfos.push_back( InputMetainfo( *it ) );
		}

		void BlockMetainfo::getParameterMetainfo( std::vector< InputMetainfo > &metainfos ) const
		{
			std::shared_ptr< const BlockMetadata > meta = checkValidity< const BlockMetadata  >( mImpl, "block metainfo" );
			std::vector< std::shared_ptr< IOMetadata > > params = meta->getParameters();
			metainfos.clear();
			metainfos.reserve( params.size() );
			for ( std::vector< std::shared_ptr< IOMetadata > >::const_iterator it = params.begin(); it != params.end(); ++it )
				metainfos.push_back( InputMetainfo( *it ) );
		}

		void BlockMetainfo::getOutletMetainfo( std::vector< OutputMetainfo > &metainfos ) const
		{
			std::shared_ptr< const BlockMetadata > meta = checkValidity< const BlockMetadata  >( mImpl, "block metainfo" );
				std::vector< std::shared_ptr< IOMetadata > > outlets = meta->getOutlets();
			metainfos.clear();
			metainfos.reserve( outlets.size() );
			for ( std::vector< std::shared_ptr< IOMetadata > >::const_iterator it = outlets.begin(); it != outlets.end(); ++it )
				metainfos.push_back( OutputMetainfo( *it ) );
		}
	}
}