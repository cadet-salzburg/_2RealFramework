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

#include "app/_2RealFunctionBlockHandle.h"
#include "app/_2RealHandleValidity.h"
#include "app/_2RealBlockMetainfo.h"
#include "app/_2RealTimerHandle.h"
#include "engine/_2RealBlock.h"
#include "engine/_2RealTimer.h"

namespace _2Real
{
	namespace app
	{
		FunctionBlockHandle::FunctionBlockHandle() :
			mImpl()
		{
		}

		FunctionBlockHandle::FunctionBlockHandle( std::shared_ptr< Block > block ) :
			mImpl( block )
		{
		}

		bool FunctionBlockHandle::isValid() const
		{
			std::shared_ptr< Block > block = mImpl.lock();
			return ( nullptr != block.get() );
		}

		BlockMetainfo FunctionBlockHandle::getMetainfo() const
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			return BlockMetainfo( block->getMetainfo() );
		}

		/*
		void FunctionBlockHandle::setup( std::function< void() > const& cb )
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			block->setup( cb );
		}

		void FunctionBlockHandle::startUpdating( TimerHandle handle )
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			block->startUpdating( static_cast< std::shared_ptr< UpdateTrigger > >( handle ), nullptr );
		}

		void FunctionBlockHandle::stopUpdating( std::function< void() > const& cb )
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			block->stopUpdating( cb );
		}

		void FunctionBlockHandle::singlestep( std::function< void() > const& cb )
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			block->singleUpdate( cb );
		}

		void FunctionBlockHandle::shutdown( std::function< void() > const& cb )
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			block->shutdown( cb );
		}
		*/

		std::future< BlockState > FunctionBlockHandle::setup()
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			return block->setup();
		}

		std::future< BlockState > FunctionBlockHandle::singlestep()
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			return block->singlestep();
		}

		std::future< BlockState > FunctionBlockHandle::shutdown()
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			return block->shutdown();
		}

		std::future< BlockState > FunctionBlockHandle::startUpdating( TimerHandle handle )
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			block->startUpdating( static_cast< std::shared_ptr< UpdateTrigger > >( handle ) );
		}

		std::future< BlockState > FunctionBlockHandle::stopUpdating()
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			block->stopUpdating();
		}
	}
}
