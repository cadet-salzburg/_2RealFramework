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

#include "app/_2RealContextBlockHandle.h"
#include "app/_2RealOutletHandle.h"
#include "app/_2RealBlockMetainfo.h"
#include "engine/_2RealFunctionBlock.h"
#include "helpers/_2RealStringHelpers.h"

namespace _2Real
{
	namespace app
	{
		ContextBlockHandle::ContextBlockHandle() :
			mImpl()
		{
		}

		ContextBlockHandle::ContextBlockHandle( std::shared_ptr< FunctionBlock > block ) :
			mImpl( block )
		{
		}

		bool ContextBlockHandle::isValid() const
		{
			std::shared_ptr< FunctionBlock > block = mImpl.lock();
			return ( block.get() != nullptr );
		}

		BlockMetainfo ContextBlockHandle::getBlockMetainfo() const
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "context" );
			return BlockMetainfo( block->getBlockMetadata() );
		}

		OutletHandle ContextBlockHandle::getOutletHandle( std::string const& name )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "context" );
			return OutletHandle( block->getOutlet( name ) );
		}

		void ContextBlockHandle::getAllOutletHandles( OutletHandles &handles )
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "context" );
			FunctionBlock::Outlets &outlets = block->getAllOutlets();
			handles.clear();
			for ( FunctionBlock::Outlets::iterator it = outlets.begin(); it != outlets.end(); ++it )
				handles.push_back( OutletHandle( *it ) );
		}

		void ContextBlockHandle::registerToNewData( BlockDataCallback callback, void *userData ) const
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "context" );
			BlockCallback *cb = new FunctionCallback< std::vector< std::shared_ptr< const CustomType > > >( callback, userData );
			block->registerToNewData( *cb );
		}

		void ContextBlockHandle::unregisterFromNewData( BlockDataCallback callback, void *userData ) const
		{
			std::shared_ptr< FunctionBlock > block = checkValidity< FunctionBlock >( mImpl, "context" );
			BlockCallback *cb = new FunctionCallback< std::vector< std::shared_ptr< const CustomType > > >( callback, userData );
			block->unregisterFromNewData( *cb );
		}
	}
}