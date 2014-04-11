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

#include "engine/_2RealBlock.h"
#include "engine/_2RealSharedServiceMetainfo.h"
#include "engine/_2RealIoSlotMetainfo.h"
#include "engine/_2RealAbstractSharedServiceFactory.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealParameter.h"
#include "engine/_2RealMultiInlet.h"
#include "engine/_2RealId.h"
#include "engine/_2RealStateMachine.h"
#include "engine/_2RealBundle.h"
#include "engine/_2RealUpdatePolicy.h"

namespace _2Real
{

	void BlockIo::doNothing()
	{
	}

	void BlockIo::doUpdate()
	{
		for ( auto it : mInlets )
		{
			it->update();
		}

		mBlockObj->update();
	}

	void BlockIo::doSetup()
	{
		for ( auto it : mParameters )
			it->update();

		mBlockObj->setup();
	}

	void BlockIo::doShutdown()
	{
		mBlockObj->shutdown();
	}

	/////////////////////////

	std::shared_ptr< Block > Block::createFromMetainfo( std::shared_ptr< Bundle > parent, std::shared_ptr< const SharedServiceMetainfo > meta, std::shared_ptr< Threadpool > threads, const uint64_t count )
	{
		std::vector< std::shared_ptr< AbstractSharedService > > dependencies;
		std::vector< std::string > dependenciesByName = meta->getDependencies();
		for ( auto dependency : dependenciesByName )
		{
			//dependencies.push_back( owner->createBlock( dependency ) );
		}

		auto blockId = InstanceId::create( meta->getId(), parent->getId(), InstanceType::BLOCK, meta->getId()->getName() + std::to_string( count ) );
		std::shared_ptr< Block > block( new Block( parent, meta, blockId, threads ) );


		std::shared_ptr< BlockIo > blockIo( new BlockIo );
		for ( auto it : meta->getInletMetainfos() )
			blockIo->mInlets.push_back( it->isMulti() ? MultiInlet::createFromMetainfo( block, it ) : Inlet::createFromMetainfo( block, it ) );
		for ( auto it : meta->getOutletMetainfos() )
			blockIo->mOutlets.push_back( Outlet::createFromMetainfo( block, it ) );
		for ( auto it : meta->getParameterMetainfos() )
			blockIo->mParameters.push_back( Parameter::createFromMetainfo( block, it ) );

		_2Real::bundle::BlockIo ioHandle;
		for ( auto it : blockIo->mInlets )
		{
			if ( it->isMultiInlet() )
				ioHandle.mInlets.push_back( std::shared_ptr< bundle::AbstractInletHandle >( new bundle::MultiInletHandle( std::dynamic_pointer_cast< MultiInlet >( it ) ) ) );
			else
				ioHandle.mInlets.push_back( std::shared_ptr< bundle::AbstractInletHandle >( new bundle::InletHandle( std::dynamic_pointer_cast< Inlet >( it ) ) ) );
		}

		for ( auto it : blockIo->mOutlets )
			ioHandle.mOutlets.push_back( std::shared_ptr< bundle::OutletHandle >( new bundle::OutletHandle( it ) ) );

		for ( auto it : blockIo->mParameters )
			ioHandle.mParameters.push_back( std::shared_ptr< bundle::ParameterHandle >( new bundle::ParameterHandle( it ) ) );

		auto factory = meta->getFactory();
		blockIo->mBlockObj = meta->getFactory()->create( ioHandle, dependencies );

		block->mIo = blockIo;

		std::shared_ptr< UpdatePolicy > policy = UpdatePolicy::createFromMetainfo( block, meta->getDefaultUpdatePolicy(), blockIo->mInlets );
		
		block->mUpdatePolicy = policy;
		
		block->mStateHandler.reset( new StateMachine( threads, blockIo ) );

		return block;
	}

	Block::Block( std::shared_ptr< Bundle > parent, std::shared_ptr< const SharedServiceMetainfo > meta, std::shared_ptr< const InstanceId > id, std::shared_ptr< Threadpool > threads ) :
		enable_shared_from_this< Block >(),
		mParent( parent ),
		mMetainfo( meta ),
		mId( id ),
		mUpdatePolicy( nullptr ),
		mStateHandler( nullptr ),
		mIo( nullptr )
	{
	}

	std::shared_ptr< const InstanceId > Block::getId() const
	{
		return mId;
	}

	std::shared_ptr< Bundle > Block::getParent()
	{
		return mParent.lock();
	}

	bool Block::isSingleton() const
	{
		return mMetainfo.lock()->isSingleton();
	}

	std::shared_ptr< BlockIo > Block::getBlockIo()
	{
		return mIo;
	}

	std::shared_ptr< UpdatePolicy > Block::getUpdatePolicy()
	{
		return mUpdatePolicy;
	}

	std::future< BlockState > Block::setup()
	{
		return mStateHandler->setup();
	}

	std::future< BlockState > Block::singlestep()
	{
		return mStateHandler->singlestep();
	}

	std::future< BlockState > Block::shutdown()
	{
		return mStateHandler->shutdown();
	}

	std::future< BlockState > Block::startUpdating( std::shared_ptr< UpdateTrigger > trigger )
	{
		return mStateHandler->startRunning( trigger );
	}

	std::future< BlockState > Block::startUpdating()
	{
		return mStateHandler->startRunning( std::dynamic_pointer_cast< UpdateTrigger >( mUpdatePolicy ) );
	}

	std::future< BlockState > Block::stopUpdating()
	{
		return mStateHandler->stopRunning();
	}

}
