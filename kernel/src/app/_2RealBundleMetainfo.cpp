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
#include "app/_2RealHandleValidity.h"
#include "engine/_2RealSharedLibraryMetainfo.h"

namespace _2Real
{
	namespace app
	{
		BundleMetainfo::BundleMetainfo() :
			mImpl()
		{
		}

		BundleMetainfo::BundleMetainfo( std::shared_ptr< const SharedLibraryMetainfo > meta ) :
			mImpl( meta )
		{
		}

		bool BundleMetainfo::isValid() const
		{
			std::shared_ptr< const SharedLibraryMetainfo > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		std::string const& BundleMetainfo::getName() const
		{
			std::shared_ptr< const SharedLibraryMetainfo > meta = checkValidity< const SharedLibraryMetainfo >( mImpl, "bundle metainfo" );
			return meta->getName();
		}

		std::string const& BundleMetainfo::getInstallDirectory() const
		{
			std::shared_ptr< const SharedLibraryMetainfo > meta = checkValidity< const SharedLibraryMetainfo >( mImpl, "bundle metainfo" );
			return meta->getInstallDirectory();
		}

		std::string const& BundleMetainfo::getDescription() const
		{
			std::shared_ptr< const SharedLibraryMetainfo > meta = checkValidity< const SharedLibraryMetainfo >( mImpl, "bundle metainfo" );
			return meta->getDescription();
		}

		std::string const& BundleMetainfo::getAuthor() const
		{
			std::shared_ptr< const SharedLibraryMetainfo > meta = checkValidity< const SharedLibraryMetainfo >( mImpl, "bundle metainfo" );
			return meta->getAuthor();
		}

		std::string const& BundleMetainfo::getContact() const
		{
			std::shared_ptr< const SharedLibraryMetainfo > meta = checkValidity< const SharedLibraryMetainfo >( mImpl, "bundle metainfo" );
			return meta->getContact();
		}

		std::string const& BundleMetainfo::getCategory() const
		{
			std::shared_ptr< const SharedLibraryMetainfo > meta = checkValidity< const SharedLibraryMetainfo >( mImpl, "bundle metainfo" );
			return meta->getCategory();
		}

		Version const& BundleMetainfo::getVersion() const
		{
			std::shared_ptr< const SharedLibraryMetainfo > meta = checkValidity< const SharedLibraryMetainfo >( mImpl, "bundle metainfo" );
			return meta->getVersion();
		}
	}
}