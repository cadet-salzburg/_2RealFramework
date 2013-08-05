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

#include "app/_2RealInfo.h"

#include "engine/_2RealTypeMetadata.h"
#include "datatypes/_2RealCustomData.h"

#include "engine/_2RealBundleMetadata.h"

#include "engine/_2RealBlockMetadata.h"

#include "engine/_2RealIOMetadata.h"

namespace _2Real
{
	namespace app
	{
		TypeMetainfo::TypeMetainfo() :
			mImpl()
		{
		}

		TypeMetainfo::TypeMetainfo( std::shared_ptr< const TypeMetadata > type ) :
			mImpl( type )
		{
		}

		bool TypeMetainfo::isValid() const
		{
			std::shared_ptr< const TypeMetadata > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		void TypeMetainfo::getDataFields( DataFields &fields ) const
		{
			std::shared_ptr< const TypeMetadata > meta = checkValidity< const TypeMetadata >( mImpl, "type metainfo" );
			meta->getDataFields( fields );
		}

		std::pair< std::string, std::string > TypeMetainfo::getTypename() const
		{
			std::shared_ptr< const TypeMetadata > meta = checkValidity< const TypeMetadata >( mImpl, "type metainfo" );
			return meta->getTypeId();
		}

		std::shared_ptr< CustomType > TypeMetainfo::makeData() const
		{
			std::shared_ptr< const TypeMetadata > meta = checkValidity< const TypeMetadata >( mImpl, "type metainfo" );
			return std::shared_ptr< CustomType >( new CustomType( meta ) );
		}

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
			return meta->name;
		}

		std::pair< std::string, std::string > const& InputMetainfo::getTypeName() const
		{
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
			return meta->typeMetadata->getTypeId();
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
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
			return meta->name;
		}

		std::pair< std::string, std::string > const& OutputMetainfo::getTypeName() const
		{
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
			return meta->typeMetadata->getTypeId();
		}

		TypeMetainfo OutputMetainfo::getTypeMetainfo() const
		{
			std::shared_ptr< const IOMetadata > meta = checkValidity< const IOMetadata  >( mImpl, "input metainfo" );
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

		BundleMetainfo::BundleMetainfo() :
			mImpl()
		{
		}

		BundleMetainfo::BundleMetainfo( std::shared_ptr< const BundleMetadata > meta ) :
			mImpl( meta )
		{
		}

		bool BundleMetainfo::isValid() const
		{
			std::shared_ptr< const BundleMetadata > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		std::string const& BundleMetainfo::getName() const
		{
			std::shared_ptr< const BundleMetadata > meta = checkValidity< const BundleMetadata  >( mImpl, "bundle metainfo" );
			return meta->getName();
		}

		std::string const& BundleMetainfo::getInstallDirectory() const
		{
			std::shared_ptr< const BundleMetadata > meta = checkValidity< const BundleMetadata  >( mImpl, "bundle metainfo" );
			return meta->getInstallDirectory();
		}

		std::string const& BundleMetainfo::getDescription() const
		{
			std::shared_ptr< const BundleMetadata > meta = checkValidity< const BundleMetadata  >( mImpl, "bundle metainfo" );
			return meta->getDescription();
		}

		std::string const& BundleMetainfo::getAuthor() const
		{
			std::shared_ptr< const BundleMetadata > meta = checkValidity< const BundleMetadata  >( mImpl, "bundle metainfo" );
			return meta->getAuthor();
		}

		std::string const& BundleMetainfo::getContact() const
		{
			std::shared_ptr< const BundleMetadata > meta = checkValidity< const BundleMetadata  >( mImpl, "bundle metainfo" );
			return meta->getContact();
		}

		std::string const& BundleMetainfo::getCategory() const
		{
			std::shared_ptr< const BundleMetadata > meta = checkValidity< const BundleMetadata  >( mImpl, "bundle metainfo" );
			return meta->getCategory();
		}

		Version const& BundleMetainfo::getVersion() const
		{
			std::shared_ptr< const BundleMetadata > meta = checkValidity< const BundleMetadata  >( mImpl, "bundle metainfo" );
			return meta->getVersion();
		}

		void BundleMetainfo::getExportedBlocks( std::vector< BlockMetainfo > &metainfos ) const
		{
			std::shared_ptr< const BundleMetadata > meta = checkValidity< const BundleMetadata  >( mImpl, "bundle metainfo" );
			std::vector< std::shared_ptr< const BlockMetadata > > blocks;
			meta->getBlockMetadata( blocks );
			metainfos.clear();
			metainfos.reserve( blocks.size() );
			for ( std::vector< std::shared_ptr< const BlockMetadata > >::const_iterator it = blocks.begin(); it != blocks.end(); ++it )
				metainfos.push_back( BlockMetainfo( *it ) );
		}
	}
}