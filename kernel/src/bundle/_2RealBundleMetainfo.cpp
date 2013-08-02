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

#include "bundle/_2RealBundleMetainfo.h"
#include "bundle/_2RealTypeMetainfo.h"
#include "engine/_2RealBundleMetadata.h"
#include "helpers/_2RealVersion.h"
#include "helpers/_2RealStringHelpers.h"

namespace _2Real
{
	namespace bundle
	{
		BundleMetainfo::BundleMetainfo( std::shared_ptr< BundleMetadata > meta ) :
			mImpl( meta )
		{
		}

		void BundleMetainfo::setName( std::string const& name )
		{
			std::shared_ptr< BundleMetadata > bundle = mImpl.lock();
			bundle->setName( trim( name ) );
		}

		void BundleMetainfo::setDescription( std::string const& description )
		{
			std::shared_ptr< BundleMetadata > bundle = mImpl.lock();
			bundle->setDescription( trim( description ) );
		}

		void BundleMetainfo::setVersion( unsigned int major, unsigned int minor, unsigned int revision )
		{
			std::shared_ptr< BundleMetadata > bundle = mImpl.lock();
			bundle->setVersion( Version( major, minor, revision ) );
		}

		void BundleMetainfo::setAuthor( std::string const& author )
		{
			std::shared_ptr< BundleMetadata > bundle = mImpl.lock();
			bundle->setAuthor( trim( author ) );
		}

		void BundleMetainfo::setContact( std::string const& contact )
		{
			std::shared_ptr< BundleMetadata > bundle = mImpl.lock();
			bundle->setContact( trim( contact ) );
		}

		void BundleMetainfo::setCategory( std::string const& category )
		{
			std::shared_ptr< BundleMetadata > bundle = mImpl.lock();
			bundle->setCategory( trim( category ) );
		}

		ContextBlockMetainfo BundleMetainfo::exportContextBlockInternal( AbstractBlockCreator *obj )
		{
			std::shared_ptr< BundleMetadata > bundle = mImpl.lock();
			return ContextBlockMetainfo( bundle->exportContextBlock( obj ) );
		}

		FunctionBlockMetainfo BundleMetainfo::exportFunctionBlockInternal( AbstractBlockCreator *obj, std::string const&  name )
		{
			std::shared_ptr< BundleMetadata > bundle = mImpl.lock();
			return FunctionBlockMetainfo( bundle->exportFunctionBlock( obj, name ) );
		}

		TypeMetainfo BundleMetainfo::exportCustomType( std::string const& name )
		{
			checkChars( toLower( trim( name ) ) );
			std::shared_ptr< BundleMetadata > bundle = mImpl.lock();
			return TypeMetainfo( bundle->exportCustomType( name ) );
		}
	}
}