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

#include "app/_2RealBundleHandle.h"
#include "app/_2RealBlockHandle.h"
#include "app/_2RealContextBlockHandle.h"
#include "app/_2RealBundleMetainfo.h"
#include "engine/_2RealBundle.h"
#include "helpers/_2RealStringHelpers.h"

namespace _2Real
{
	namespace app
	{
		BundleHandle::BundleHandle() :
			mImpl()
		{
		}

		BundleHandle::BundleHandle( std::shared_ptr< Bundle > bundle ) :
			mImpl( bundle )
		{
		}

		bool BundleHandle::isValid() const
		{
			std::shared_ptr< Bundle > bundle = mImpl.lock();
			return ( nullptr != bundle.get() );
		}

		BundleMetainfo BundleHandle::getBundleMetainfo() const
		{
			std::shared_ptr< Bundle > bundle = checkValidity< Bundle >( mImpl, "bundle" );
			return BundleMetainfo( bundle->getBundleMetadata() );
		}

		ContextBlockHandle BundleHandle::createContextBlockInstance() const
		{
			std::shared_ptr< Bundle > bundle = checkValidity< Bundle >( mImpl, "bundle" );
			std::shared_ptr< FunctionBlock > block = bundle->getContextBlockInstance();
			return ContextBlockHandle( block );
		}

		BlockHandle BundleHandle::createFunctionBlockInstance( std::string const& name )
		{
			std::shared_ptr< Bundle > bundle = checkValidity< Bundle >( mImpl, "bundle" );
			std::shared_ptr< FunctionBlock > block = bundle->createFunctionBlockInstance( name );
			return BlockHandle( block );
		}

		void BundleHandle::unload( const long blockTimeout )
		{
			std::shared_ptr< Bundle > bundle = checkValidity< Bundle >( mImpl, "bundle" );
			bundle->unload( blockTimeout );
		}
	}
}
