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

#pragma once

#include "engine/_2RealAbstractUberBlock.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealFunctionBlockIOManager.h"
#include "engine/_2RealFunctionBlockStateManager.h"
#include "engine/_2RealFunctionBlockUpdatePolicy.h"
#include "engine/_2RealBundle.h"
#include "app/_2RealBlockHandle.h"
#include "app/_2RealContextBlockHandle.h"
#include "../_2RealBlock.h"
#include "engine/_2RealParameterMetadata.h"

namespace _2Real
{
	class FunctionBlock : public AbstractUberBlock
	{

	public:

		typedef std::vector< std::shared_ptr< AbstractInletIO > >					Inlets;
		typedef std::vector< std::shared_ptr< AbstractInletIO > >::iterator			InletIterator;
		typedef std::vector< std::shared_ptr< AbstractInletIO > >::const_iterator	InletConstIterator;

		typedef std::vector< std::shared_ptr< OutletIO > >							Outlets;
		typedef std::vector< std::shared_ptr< OutletIO > >::iterator				OutletIterator;
		typedef std::vector< std::shared_ptr< OutletIO > >::const_iterator			OutletConstIterator;

		typedef std::vector< std::shared_ptr< ParameterIO > >						Parameters;
		typedef std::vector< std::shared_ptr< ParameterIO > >::iterator				ParameterIterator;
		typedef std::vector< std::shared_ptr< ParameterIO > >::const_iterator		ParameterConstIterator;

		FunctionBlock( EngineImpl *, std::shared_ptr< Bundle >, std::shared_ptr< bundle::Block >, app::BlockInfo const& );
		~FunctionBlock();

		using AbstractUberBlock::getName;
		using AbstractUberBlock::getFullName;

		//std::shared_ptr< Bundle >		getOwningBundle();
		//std::string					getUpdateRateAsString() const;
		//bool							isRunning() const;

		app::BlockInfo const&					getBlockInfo();

		std::shared_ptr< AbstractInletIO >		getInlet( std::string const& );
		std::shared_ptr< OutletIO >				getOutlet( std::string const& );
		std::shared_ptr< ParameterIO >			getParameter( std::string const& );

		Inlets &								getAllInlets();
		Outlets &								getAllOutlets();
		Parameters &							getAllParameters();

		void									registerToNewData( app::BlockCallback &callback );
		void									unregisterFromNewData( app::BlockCallback &callback );

		void									setUp();
		void									start();
		void									stop( const bool blocking, const long timeout );
		void									prepareForShutDown();
		bool									shutDown( const long timeout );
		void									singleStep();
		void									suicide( const long timeout );

		void									updateWithFixedRate( const double updatesPerSecond );

		void									handleException( Exception &e );

		void									addInlet( InletMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type  );
		void									addOutlet( OutletMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type  );
		void									addParameter( ParameterMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type  );

	private:

		EngineImpl												*const mEngineImpl;

		FunctionBlockUpdatePolicy								*mUpdatePolicy;
		std::shared_ptr< FunctionBlockIOManager >				mIOManager;			// handled by bundle::block handle
		FunctionBlockStateManager								*mStateManager;

		std::weak_ptr< Bundle >									mBundle;			// weak ptr b/c of circualr referencing

		std::shared_ptr< bundle::Block >						mBlock;		// the only pointer to the actual block instance....
		app::BlockInfo											mBlockInfo;

	};

	inline FunctionBlock::FunctionBlock( EngineImpl *engine, std::shared_ptr< Bundle > bundle, std::shared_ptr< bundle::Block > instance, app::BlockInfo const& info ) :
		AbstractUberBlock( engine, bundle->getIds(), info.name ),
		mEngineImpl( engine ),
		mUpdatePolicy( new FunctionBlockUpdatePolicy( engine, this ) ),
		mIOManager( new FunctionBlockIOManager( engine, this ) ),
		mStateManager( new FunctionBlockStateManager( engine, this, true ) ),
		mBundle( bundle ),
		mBlock( instance ),
		mBlockInfo( info )
	{
		// stae manager: does not do anything, b/c no triggers exist yet
		mStateManager->m_FunctionBlock = instance;
		mStateManager->m_IOManager = mIOManager;
		mStateManager->m_UpdatePolicy = mUpdatePolicy;

		// update policy: empty on init
		mUpdatePolicy->m_IOManager = mIOManager;
		mUpdatePolicy->m_StateManager = mStateManager;

		// io mgr: empty on init
		mIOManager->m_StateManager = mStateManager;
		mIOManager->m_UpdatePolicy = mUpdatePolicy;
	}

	inline FunctionBlock::~FunctionBlock()
	{
		delete mUpdatePolicy;
		delete mStateManager;		// must go second
	}

	//inline bool FunctionBlock::isRunning() const
	//{
	//	return mStateManager->isRunning();
	//}

	//inline std::string FunctionBlock::getUpdateRateAsString() const
	//{
	//	std::ostringstream str;
	//	str << mUpdatePolicy->getUpdateRate();
	//	return str.str();
	//}

	inline void FunctionBlock::addInlet( InletMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type )
	{
		IOInfo *info = new IOInfo( meta.name, initializer, type, meta.defaultPolicy, meta.isMulti );
		mIOManager->addInlet( info );
	}

	inline void FunctionBlock::addOutlet( OutletMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type )
	{
		IOInfo *info = new IOInfo( meta.name, initializer, type, Policy::INVALID, false );
		mIOManager->addOutlet( info );
	}

	inline void FunctionBlock::addParameter( ParameterMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type )
	{
		IOInfo *info = new IOInfo( meta.name, initializer, type, Policy::INVALID, false );
		mIOManager->addParameter( info );
	}

	inline app::BlockInfo const& FunctionBlock::getBlockInfo()
	{
		return mBlockInfo;
	}

	inline std::shared_ptr< AbstractInletIO > FunctionBlock::getInlet( std::string const& name )
	{
		return mIOManager->getInlet( name );
	}

	inline std::shared_ptr< OutletIO > FunctionBlock::getOutlet( std::string const& name )
	{
		return mIOManager->getOutlet( name );
	}

	inline std::shared_ptr< ParameterIO > FunctionBlock::getParameter( std::string const& name )
	{
		return mIOManager->getParameter( name );
	}

	inline FunctionBlock::Inlets & FunctionBlock::getAllInlets()
	{
		return mIOManager->mInlets;
	}

	inline FunctionBlock::Parameters & FunctionBlock::getAllParameters()
	{
		return mIOManager->mParameters;
	}

	inline FunctionBlock::Outlets & FunctionBlock::getAllOutlets()
	{
		return mIOManager->mOutlets;
	}

	inline void FunctionBlock::registerToNewData( app::BlockCallback &callback )
	{
		mIOManager->registerToNewData( callback );
	}

	inline void FunctionBlock::unregisterFromNewData( app::BlockCallback &callback )
	{
		mIOManager->unregisterFromNewData( callback );
	}

	inline void FunctionBlock::setUp()
	{
		mStateManager->setUp();
	}

	inline void FunctionBlock::start()
	{
		mStateManager->start();
	}

	inline void FunctionBlock::stop( const bool blocking, const long timeout )
	{
		Poco::Event & ev = mStateManager->stop();
		if ( blocking )
		{
			if ( !ev.tryWait( timeout ) )
			{
				std::ostringstream msg;
				msg << "timeout reached on " << getFullName() << " stop()" << std::endl;
				throw TimeOutException( msg.str() );
			}
		}
	}

	inline void FunctionBlock::prepareForShutDown()
	{
		mStateManager->prepareForShutDown();
	}

	inline bool FunctionBlock::shutDown( const long timeout )
	{
		return mStateManager->shutDown( timeout );
	}

	inline void FunctionBlock::singleStep()
	{
		mStateManager->singleStep();
	}

	inline void FunctionBlock::updateWithFixedRate( const double updatesPerSecond )
	{
		mUpdatePolicy->setNewUpdateRate( updatesPerSecond );
	}

	inline void FunctionBlock::handleException( Exception &e )
	{
		//mEngineImpl->handleException( getHandle(), e );
	}

	inline void FunctionBlock::suicide( const long timeout )
	{
	}

}
