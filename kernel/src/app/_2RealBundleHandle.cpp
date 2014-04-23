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
#include "app/_2RealThreadpoolHandle.h"

#include "engine/_2RealBundleImpl.h"

#include "common/_2RealWeakPtrCheck.h"

namespace _2Real
{
	namespace app
	{
		BundleHandle::BundleHandle( std::shared_ptr< BundleImpl > bundle ) :
			mImpl( bundle )
		{
		}

		bool BundleHandle::isValid() const
		{
			std::shared_ptr< BundleImpl > bundle = mImpl.lock();
			return ( nullptr != bundle.get() );
		}

		BlockHandle BundleHandle::createBlock( std::string const& name, ThreadpoolHandle system, std::vector< BlockHandle > const& dependencies )
		{
			std::shared_ptr< BundleImpl > bundle = checkValidity< BundleImpl >( mImpl, "bundle" );
			std::shared_ptr< ThreadpoolImpl_I > threads = checkValidity< ThreadpoolImpl_I >( system.mImpl, "threadpool" );
			std::vector< std::shared_ptr< BlockImpl > > blocks;
			for ( auto it : dependencies )
			{
				std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( it.mImpl, "block" );
				blocks.push_back( block );
			}

			return BlockHandle( bundle->createBlock( name, threads, blocks ) );
		}

		void BundleHandle::unload( const uint64_t timeout )
		{
			std::shared_ptr< BundleImpl > bundle = checkValidity< BundleImpl >( mImpl, "bundle" );
			bundle->unload( timeout );
		}
	}
}
