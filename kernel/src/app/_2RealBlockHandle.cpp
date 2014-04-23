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

#include "app/_2RealBlockHandle.h"
#include "app/_2RealBundleHandle.h"
#include "app/_2RealTimerHandle.h"
#include "app/_2RealBlockIo.h"
#include "app/_2RealInletHandle_I.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealMultiInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "app/_2RealParameterHandle.h"
#include "app/_2RealUpdatePolicyHandle.h"

#include "engine/_2RealBlockImpl.h"
#include "engine/_2RealTimerImpl.h"
#include "engine/_2RealInletImpl_I.h"
#include "engine/_2RealInletImpl.h"
#include "engine/_2RealMultiInletImpl.h"
#include "engine/_2RealOutletImpl.h"
#include "engine/_2RealParameterImpl.h"

#include "common/_2RealWeakPtrCheck.h"

namespace _2Real
{
	namespace app
	{
		BlockHandle::BlockHandle( std::shared_ptr< BlockImpl > bundle ) :
			mImpl( bundle )
		{
		}

		bool BlockHandle::isValid() const
		{
			std::shared_ptr< BlockImpl > block = mImpl.lock();
			return ( nullptr != block.get() );
		}

		BundleHandle BlockHandle::getBundle()
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			auto bundle = block->getParent();
			return BundleHandle( bundle );
		}

		std::future< BlockResult > BlockHandle::setup()
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return block->setup();
		}

		std::future< BlockResult > BlockHandle::singlestep()
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return block->singlestep();
		}

		std::future< BlockResult > BlockHandle::shutdown()
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return block->shutdown();
		}

		std::future< BlockResult > BlockHandle::startUpdating( TimerHandle handle )
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			std::shared_ptr< TimerImpl > timer = checkValidity< TimerImpl >( handle.mImpl, "timer" );
			return block->startUpdating( timer );
		}

		std::future< BlockResult > BlockHandle::startUpdating()
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return block->startUpdating();
		}

		std::future< BlockResult > BlockHandle::stopUpdating()
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return block->stopUpdating();
		}

		void BlockHandle::destroy( const uint64_t timeout )
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return block->destroy( timeout );
		}

		UpdatePolicyHandle BlockHandle::getUpdatePolicy()
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return UpdatePolicyHandle( block->getUpdatePolicy() );
		}

		BlockIo BlockHandle::getBlockIo()
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			std::shared_ptr< _2Real::BlockIo > io = block->getBlockIo();
			BlockIo result;
			for ( auto it : io->mInlets )
				result.mInlets.push_back( it->isMultiInlet() ? std::shared_ptr< InletHandle_I >( new MultiInletHandle( std::static_pointer_cast< MultiInletImpl >( it ) ) ) : std::shared_ptr< InletHandle_I >( new InletHandle( std::static_pointer_cast< InletImpl >( it ) ) ) );
			for ( auto it : io->mParameters )
				result.mParameters.push_back( std::shared_ptr< ParameterHandle >( new ParameterHandle( it ) ) );
			for ( auto it : io->mOutlets )
				result.mOutlets.push_back( std::shared_ptr< OutletHandle >( new OutletHandle( it ) ) );
			return result;
		}

		InletHandle BlockHandle::getInlet( std::string const& name )
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return InletHandle( block->getInlet( name ) );
		}

		MultiInletHandle BlockHandle::getMultiInlet( std::string const& name )
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return MultiInletHandle( block->getMultiInlet( name ) );
		}

		OutletHandle BlockHandle::getOutlet( std::string const& name )
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return OutletHandle( block->getOutlet( name ) );
		}

		ParameterHandle BlockHandle::getParameter( std::string const& name )
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return ParameterHandle( block->getParameter( name ) );
		}

		Connection BlockHandle::registerToException( boost::function< void( Exception ) > listener )
		{
			std::shared_ptr< BlockImpl > block = checkValidity< BlockImpl >( mImpl, "block" );
			return block->registerToException( listener );
		}
	}
}
