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

#include "app/_2RealBundleMetainfo.h"
#include "app/_2RealBlockMetainfo.h"
#include "app/_2RealCustomTypeMetainfo.h"

#include "engine/_2RealBundleMetainfoImpl.h"
#include "engine/_2RealCustomTypeMetainfoImpl.h"
#include "common/_2RealDeclarations.h"

#include "common/_2RealWeakPtrCheck.h"

namespace _2Real
{
	namespace app
	{
		BundleMetainfo::BundleMetainfo( std::shared_ptr< const BundleMetainfoImpl > meta ) :
			mImpl( meta )
		{
		}

		bool BundleMetainfo::isValid() const
		{
			std::shared_ptr< const BundleMetainfoImpl > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		std::string BundleMetainfo::getDescription() const
		{
			std::shared_ptr< const BundleMetainfoImpl > meta = checkValidity< const BundleMetainfoImpl >( mImpl, "bundle metainfo" );
			return meta->getDescription();
		}

		std::string BundleMetainfo::getAuthor() const
		{
			std::shared_ptr< const BundleMetainfoImpl > meta = checkValidity< const BundleMetainfoImpl >( mImpl, "bundle metainfo" );
			return meta->getAuthor();
		}

		std::string BundleMetainfo::getContact() const
		{
			std::shared_ptr< const BundleMetainfoImpl > meta = checkValidity< const BundleMetainfoImpl >( mImpl, "bundle metainfo" );
			return meta->getContact();
		}

		std::string BundleMetainfo::getCategory() const
		{
			std::shared_ptr< const BundleMetainfoImpl > meta = checkValidity< const BundleMetainfoImpl >( mImpl, "bundle metainfo" );
			return meta->getCategory();
		}

		Version BundleMetainfo::getVersion() const
		{
			std::shared_ptr< const BundleMetainfoImpl > meta = checkValidity< const BundleMetainfoImpl >( mImpl, "bundle metainfo" );
			return meta->getVersion();
		}

		std::vector< BlockMetainfo > BundleMetainfo::getExportedBlocks() const
		{
			std::shared_ptr< const BundleMetainfoImpl > meta = checkValidity< const BundleMetainfoImpl >( mImpl, "bundle metainfo" );
			std::vector< BlockMetainfo > result;
			for ( auto it : meta->getServiceMetainfos() )
				result.push_back( BlockMetainfo( it ) );
			return result;
		}

		std::vector< CustomTypeMetainfo > BundleMetainfo::getExportedTypes() const
		{
			std::shared_ptr< const BundleMetainfoImpl > meta = checkValidity< const BundleMetainfoImpl >( mImpl, "bundle metainfo" );
			std::vector< CustomTypeMetainfo > result;
			for ( auto it : meta->getTypeMetainfos() )
				result.push_back( CustomTypeMetainfo( it ) );
			return result;
		}

		BlockMetainfo BundleMetainfo::getExportedBlock( std::string const& name )
		{
			std::shared_ptr< const BundleMetainfoImpl > meta = checkValidity< const BundleMetainfoImpl >( mImpl, "bundle metainfo" );
			return BlockMetainfo( meta->getServiceMetainfo( name ) );
		}

		CustomTypeMetainfo BundleMetainfo::getExportedType( std::string const& name )
		{
			std::shared_ptr< const BundleMetainfoImpl > meta = checkValidity< const BundleMetainfoImpl >( mImpl, "bundle metainfo" );
			return CustomTypeMetainfo( meta->getTypeMetainfo( name ) );
		}
	}
}