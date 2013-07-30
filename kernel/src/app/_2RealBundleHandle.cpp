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
#include "app/_2RealInfo.h"
#include "engine/_2RealBundle.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealStringHelpers.h"
#include "engine/_2RealFunctionBlock.h"

template< typename TObj >
std::shared_ptr< TObj > checkValidity( std::weak_ptr< TObj > handle, std::string const& what )
{
	std::shared_ptr< TObj > locked = handle.lock();
	if ( locked.get() == nullptr )
	{
		std::stringstream msg;
		msg << "nullptr access: " << what << " handle does not point to an object" << std::endl;
	}

	return locked;
}

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

		BundleInfo const& BundleHandle::getBundleInfo() const
		{
			std::shared_ptr< Bundle > bundle = checkValidity< Bundle >( mImpl, "bundle" );
			return bundle->getBundleInfo();
		}

		ContextBlockHandle BundleHandle::getContextBlock() const
		{
			std::shared_ptr< Bundle > bundle = checkValidity< Bundle >( mImpl, "bundle" );
			std::shared_ptr< FunctionBlock > block = bundle->getContextBlock();
			return ContextBlockHandle( block );
		}

		BlockHandle BundleHandle::createBlockInstance( std::string const& name )
		{
			std::shared_ptr< Bundle > bundle = checkValidity< Bundle >( mImpl, "bundle" );
			std::shared_ptr< FunctionBlock > block = bundle->createBlockInstance( name );
			return BlockHandle( block );
		}

		void BundleHandle::unload( const long blockTimeout )
		{
			std::shared_ptr< Bundle > bundle = checkValidity< Bundle >( mImpl, "bundle" );
			bundle->unload( blockTimeout );
		}
	}
}
