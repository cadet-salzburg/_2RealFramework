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

#include "engine/_2RealBlockImpl.h"
#include "engine/_2RealBlockMetainfoImpl.h"
#include "engine/_2RealIoSlotMetainfoImpl.h"
#include "engine/_2RealBlockFactory_I.h"
#include "engine/_2RealInletImpl.h"
#include "engine/_2RealOutletImpl.h"
#include "engine/_2RealParameterImpl.h"
#include "engine/_2RealMultiInletImpl.h"
#include "engine/_2RealId.h"
#include "engine/_2RealBlockStateHandler.h"
#include "engine/_2RealBundleImpl.h"
#include "engine/_2RealUpdatePolicyImpl.h"

#include "bundle/_2RealBlock.h"
#include "bundle/_2RealBlockIo.h"
#include "bundle/_2RealMultiInletHandle.h"
#include "bundle/_2RealInletHandle.h"

namespace _2Real
{

	BlockIo::BlockIo( BlockAccess const& key ) : mKey( key ) {}

	void BlockIo::doNothing()
	{
	}

	void BlockIo::doUpdate()
	{
		for ( auto it : mParameters )
		{
			it->update();
		}

		for ( auto it : mInlets )
		{
			it->update();
		}

		mBlockObj->doUpdate( mKey );

		for ( auto it : mOutlets )
		{
			it->update();
		}
	}

	void BlockIo::doSetup()
	{
		for ( auto it : mParameters )
			it->update();

		mBlockObj->doSetup( mKey );

		for ( auto it : mOutlets )
		{
			it->update();
		}
	}

	void BlockIo::doShutdown()
	{
		mBlockObj->doShutdown( mKey );
	}

	/////////////////////////

	std::shared_ptr< BlockImpl > BlockImpl::createFromMetainfo( std::shared_ptr< BundleImpl > parent, std::shared_ptr< const BlockMetainfoImpl > meta, std::shared_ptr< ThreadpoolImpl_I > threads, std::vector< std::shared_ptr< BlockImpl > > const& dependencies, const uint64_t count )
	{
		auto blockId = InstanceId::create( meta->getId(), parent->getId(), InstanceType::BLOCK, meta->getId()->getName() + std::to_string( count ) );
		std::shared_ptr< BlockImpl > block( new BlockImpl( parent, meta, blockId, threads ) );

		BlockAccess access;
		std::shared_ptr< BlockIo > blockIo( new BlockIo( access ) );
		for ( auto it : meta->getInletMetainfos() )
			blockIo->mInlets.push_back( it->isMulti() ? MultiInletImpl::createFromMetainfo( block, it ) : InletImpl::createFromMetainfo( block, it ) );
		for ( auto it : meta->getOutletMetainfos() )
			blockIo->mOutlets.push_back( OutletImpl::createFromMetainfo( block, it ) );
		for ( auto it : meta->getParameterMetainfos() )
			blockIo->mParameters.push_back( ParameterImpl::createFromMetainfo( block, it ) );

		_2Real::bundle::BlockIo ioHandle;
		for ( auto it : blockIo->mInlets )
		{
			if ( it->isMultiInlet() )
				ioHandle.mInlets.push_back( std::shared_ptr< bundle::InletHandle_I >( new bundle::MultiInletHandle( std::static_pointer_cast< MultiInletImpl >( it ) ) ) );
			else
				ioHandle.mInlets.push_back( std::shared_ptr< bundle::InletHandle_I >( new bundle::InletHandle( std::static_pointer_cast< InletImpl >( it ) ) ) );
		}

		for ( auto it : blockIo->mOutlets )
			ioHandle.mOutlets.push_back( std::shared_ptr< bundle::OutletHandle >( new bundle::OutletHandle( it ) ) );

		for ( auto it : blockIo->mParameters )
			ioHandle.mParameters.push_back( std::shared_ptr< bundle::ParameterHandle >( new bundle::ParameterHandle( it ) ) );

		std::vector< std::shared_ptr< bundle::Block > > services;
		for ( auto it : dependencies )
			services.push_back( it->getBlockIo()->mBlockObj );

		auto factory = meta->getFactory();
		blockIo->mBlockObj = meta->getFactory()->create( ioHandle, services );

		block->mIo = blockIo;

		std::shared_ptr< UpdatePolicyImpl > policy = UpdatePolicyImpl::createFromMetainfo( block, meta->getDefaultUpdatePolicy(), blockIo->mInlets );
		
		block->mUpdatePolicy = policy;
		
		block->mStateHandler.reset( new BlockStateHandler( threads, blockIo ) );

		return block;
	}

	BlockImpl::BlockImpl( std::shared_ptr< BundleImpl > parent, std::shared_ptr< const BlockMetainfoImpl > meta, std::shared_ptr< const InstanceId > id, std::shared_ptr< ThreadpoolImpl_I > threads ) :
		enable_shared_from_this< BlockImpl >(),
		mParent( parent ),
		mMetainfo( meta ),
		mId( id ),
		mUpdatePolicy( nullptr ),
		mStateHandler( nullptr ),
		mIo( nullptr )
	{
	}

	std::shared_ptr< const InstanceId > BlockImpl::getId() const
	{
		return mId;
	}

	std::shared_ptr< BundleImpl > BlockImpl::getParent()
	{
		return mParent.lock();
	}

	std::shared_ptr< BlockIo > BlockImpl::getBlockIo()
	{
		return mIo;
	}

	std::shared_ptr< UpdatePolicyImpl > BlockImpl::getUpdatePolicy()
	{
		return mUpdatePolicy;
	}

	std::future< BlockResult > BlockImpl::setup()
	{
		return mStateHandler->setup();
	}

	std::future< BlockResult > BlockImpl::singlestep()
	{
		return mStateHandler->singlestep();
	}

	std::future< BlockResult > BlockImpl::shutdown()
	{
		return mStateHandler->shutdown();
	}

	std::future< BlockResult > BlockImpl::startUpdating( std::shared_ptr< UpdateTrigger_I > trigger )
	{
		return mStateHandler->startRunning( trigger );
	}

	std::future< BlockResult > BlockImpl::startUpdating()
	{
		return mStateHandler->startRunning( std::static_pointer_cast< UpdateTrigger_I >( mUpdatePolicy ) );
	}

	std::future< BlockResult > BlockImpl::stopUpdating()
	{
		return mStateHandler->stopRunning();
	}

}
