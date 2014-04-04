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
#include "app/_2RealTimerHandle.h"
#include "app/_2RealBlockIo.h"
#include "app/_2RealOutletHandle.h"
#include "app/_2RealParameterHandle.h"

#include "engine/_2RealBlock.h"
#include "engine/_2RealTimer.h"
#include "engine/_2RealAbstractInlet.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealMultiInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealParameter.h"

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
			return block->startUpdating( static_cast< std::shared_ptr< UpdateTrigger > >( handle ) );
		}

		std::future< BlockState > FunctionBlockHandle::startUpdating()
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			return block->startUpdating();
		}

		std::future< BlockState > FunctionBlockHandle::stopUpdating()
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			return block->stopUpdating();
		}

		BlockIo FunctionBlockHandle::getBlockIo()
		{
			std::shared_ptr< Block > block = checkValidity< Block >( mImpl, "block" );
			std::shared_ptr< _2Real::BlockIo > io = block->getBlockIo();
			BlockIo result;
			for ( auto it : io->mInlets )
				result.mInlets.push_back( it->isMultiInlet() ? ( AbstractInletHandle * )new MultiInletHandle( std::dynamic_pointer_cast< MultiInlet >( it ) ) : new InletHandle( std::dynamic_pointer_cast< Inlet >( it ) ) );
			for ( auto it : io->mParameters )
				result.mParameters.push_back( new ParameterHandle( it ) );
			for ( auto it : io->mOutlets )
				result.mOutlets.push_back( new OutletHandle( it ) );
			return result;
		}
	}
}
