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

#include "engine/_2RealFunctionBlock.h"
#include "engine/_2RealFunctionBlockIOManager.h"
#include "engine/_2RealFunctionBlockStateManager.h"
#include "engine/_2RealFunctionBlockUpdateManager.h"
#include "engine/_2RealBlockMetadata.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealSystem.h"

namespace _2Real
{
	FunctionBlock::FunctionBlock( EngineImpl *engine, std::shared_ptr< InstanceId > id, std::shared_ptr< const Bundle > bundle, std::shared_ptr< bundle::Block > instance, std::shared_ptr< const BlockMetadata > meta ) :
		AbstractUberBlock(),
		mEngineImpl( engine ),
		mIdentifier( id ),
		mUpdateManager( new FunctionBlockUpdateManager( engine, this ) ),
		mIOManager( new FunctionBlockIOManager( engine, this ) ),
		mStateManager( new FunctionBlockStateManager( engine, this, true ) ),
		mBundle( bundle ),
		mBlock( instance ),
		mBlockMetadata( meta )
	{
		// state manager: does not do anything, b/c no triggers exist yet
		mStateManager->mFunctionBlock = instance;
		mStateManager->mIOManager = mIOManager;
		mStateManager->mUpdateManager = mUpdateManager;

		// update policy: empty on init
		mUpdateManager->mStateManager = mStateManager;

		// io mgr: empty on init
		mIOManager->mUpdateManager = mUpdateManager;
	}

	FunctionBlock::~FunctionBlock()
	{
		mUpdateManager.reset();
		mStateManager.reset();		// must go second
		mIOManager.reset();
	}

	std::string const& FunctionBlock::getFullHumanReadableName() const
	{
		return mIdentifier->getFullHumanReadableName();
	}

	std::string const& FunctionBlock::getHumanReadableName() const
	{
		return mIdentifier->getHumanReadableName();
	}

	//std::string const& FunctionBlock::getCode() const
	//{
	//	return mIdentifier.getCode();
	//}

	std::shared_ptr< const InstanceId > FunctionBlock::getIdentifier() const
	{
		return mIdentifier;
	}

	void FunctionBlock::setSelfRef( std::shared_ptr< FunctionBlock > ref )
	{
		mSelfRef = ref;
	}

	std::shared_ptr< FunctionBlock > FunctionBlock::getSelfRef()
	{
		return mSelfRef.lock();
	}

	std::shared_ptr< const FunctionBlock > FunctionBlock::getSelfRef() const
	{
		return mSelfRef.lock();
	}

	bool FunctionBlock::isContext() const
	{
		return mBlockMetadata->isContext();
	}

	void FunctionBlock::addInlet( std::shared_ptr< const IOMetadata > meta )
	{
		mIOManager->addInlet( meta );
	}

	void FunctionBlock::addOutlet( std::shared_ptr< const IOMetadata > meta )
	{
		mIOManager->addOutlet( meta );
	}

	void FunctionBlock::addParameter( std::shared_ptr< const IOMetadata > meta )
	{
		mIOManager->addParameter( meta );
	}

	std::shared_ptr< const BlockMetadata > FunctionBlock::getBlockMetadata() const
	{
		return mBlockMetadata;
	}

	std::shared_ptr< AbstractInletIO > FunctionBlock::getInlet( std::string const& name )
	{
		return mIOManager->getInlet( name );
	}

	std::shared_ptr< OutletIO > FunctionBlock::getOutlet( std::string const& name )
	{
		return mIOManager->getOutlet( name );
	}

	std::shared_ptr< AbstractInletIO > FunctionBlock::getParameter( std::string const& name )
	{
		return mIOManager->getParameter( name );
	}

	FunctionBlock::Inlets & FunctionBlock::getAllInlets()
	{
		return mIOManager->mInlets;
	}

	FunctionBlock::Parameters & FunctionBlock::getAllParameters()
	{
		return mIOManager->mParameters;
	}

	FunctionBlock::Outlets & FunctionBlock::getAllOutlets()
	{
		return mIOManager->mOutlets;
	}

	void FunctionBlock::registerToNewData( app::BlockCallback &callback )
	{
		mIOManager->registerToNewData( callback );
	}

	void FunctionBlock::unregisterFromNewData( app::BlockCallback &callback )
	{
		mIOManager->unregisterFromNewData( callback );
	}

	void FunctionBlock::setUp()
	{
		mStateManager->setUp();
	}

	void FunctionBlock::start()
	{
		mStateManager->start();
	}

	void FunctionBlock::stop( const bool blocking, const long timeout )
	{
		Poco::Event & ev = mStateManager->stop();
		if ( blocking )
		{
			if ( !ev.tryWait( timeout ) )
			{
				std::ostringstream msg;
				msg << "timeout reached on " << getFullHumanReadableName() << " stop()" << std::endl;
				throw TimeOutException( msg.str() );
			}
		}
	}

	void FunctionBlock::prepareForShutDown()
	{
		mStateManager->prepareForShutDown();
	}

	bool FunctionBlock::shutDown( const long timeout )
	{
		return mStateManager->shutDown( timeout );
	}

	//void FunctionBlock::singleStep()
	//{
	//	mStateManager->singleStep();
	//}

	void FunctionBlock::setUpdateTimer( std::shared_ptr< Timer > timer )
	{
		mUpdateManager->setUpdateTimer( timer );
	}

	void FunctionBlock::handleException( Exception const& e )
	{
		mEngineImpl->handleException( mSelfRef.lock(), e );
	}

	void FunctionBlock::suicide( const long timeout )
	{
		mEngineImpl->getBlockManager()->removeBlockInstance( mBundle.lock(), getSelfRef(), timeout );
	}
}