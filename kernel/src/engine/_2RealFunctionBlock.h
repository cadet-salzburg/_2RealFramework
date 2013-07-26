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
#include "app/_2RealBlockInfo.h"
#include "helpers/_2RealHandleable.h"
#include "../_2RealBlock.h"
#include "engine/_2RealParameterMetadata.h"

namespace _2Real
{
	template< typename THandle >
	class FunctionBlock : public AbstractUberBlock, private Handleable< FunctionBlock< THandle >, THandle >
	{

	public:

		FunctionBlock( EngineImpl *engine, Bundle *owningBundle, bundle::Block *blockInstance, app::BlockInfo const& info );
		~FunctionBlock();

		using Handleable< FunctionBlock< THandle >, THandle >::getHandle;
		using Handleable< FunctionBlock< THandle >, THandle >::registerHandle;
		using Handleable< FunctionBlock< THandle >, THandle >::unregisterHandle;

		using AbstractUberBlock::setName;
		using AbstractUberBlock::getName;
		using AbstractUberBlock::getFullName;

		Bundle *						getOwningBundle() { return mBundle; }
		const std::string				getUpdateRateAsString() const;
		bool							isRunning() const;

		app::BlockInfo const&			getBlockInfo();

		app::InletHandle				getAppInletHandle( std::string const& inletName ) const;
		app::OutletHandle				getAppOutletHandle( std::string const& outletName ) const;
		app::ParameterHandle			getAppParameterHandle( std::string const& paramName ) const;
		AppInletHandles const&			getAppInletHandles() const;
		AppOutletHandles const&			getAppOutletHandles() const;
		AppParameterHandles const&		getAppParameterHandles() const;

		/* added 13/05/2013 */

		AbstractInletIO &				getInlet( std::string const& inletName );
		OutletIO &						getOutlet( std::string const& outletName );

		void							registerToNewData( app::BlockCallback &callback );
		void							unregisterFromNewData( app::BlockCallback &callback );

		void							setUp();
		void							start();
		void							stop( const bool blocking, const long timeout );
		void							prepareForShutDown();
		bool							shutDown( const long timeout );
		void							singleStep();

		void							updateWithFixedRate( const double updatesPerSecond );

		void							handleException( Exception &e );

		void							addInlet( InletMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type  );
		void							addOutlet( OutletMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type  );
		void							addParameter( ParameterMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type  );

		void suicide( const long timeout ) {}

	private:

		EngineImpl					*const mEngineImpl;
		FunctionBlockUpdatePolicy	*const mUpdatePolicy;
		FunctionBlockIOManager		*const mIOManager;
		FunctionBlockStateManager	*const mStateManager;

		Bundle						*const mBundle;
		bundle::Block				*const mBlock;
		app::BlockInfo				mBlockInfo;

	};

	template< typename THandle >
	bool dedicatedThread();

	template< >
	inline bool dedicatedThread< app::ContextBlockHandle >()
	{
		return true;
	}

	template< >
	inline bool dedicatedThread< app::BlockHandle >()
	{
		return false;
	}

	template< typename THandle >
	FunctionBlock< THandle >::FunctionBlock( EngineImpl *engine, Bundle *bundle, bundle::Block *blockInstance, app::BlockInfo const& info ) :
		AbstractUberBlock( engine, bundle->getIds(), info.name ),
		Handleable< FunctionBlock< THandle >, THandle >( *this ),
		mEngineImpl( engine ),
		mUpdatePolicy( new FunctionBlockUpdatePolicy( engine, this ) ),
		mIOManager( new FunctionBlockIOManager( engine, this ) ),
		mStateManager( new FunctionBlockStateManager( engine, this, dedicatedThread< THandle >() ) ),
		mBundle( bundle ),
		mBlock( blockInstance ),
		mBlockInfo( info )
	{
#ifdef _DEBUG
		assert( mEngineImpl );
		assert( mBundle );
		assert( mBlock );
#endif

		// stae manager: does not do anything, b/c no triggers exist yet
		mStateManager->m_FunctionBlock = blockInstance;
		mStateManager->m_IOManager = mIOManager;
		mStateManager->m_UpdatePolicy = mUpdatePolicy;

		// update policy: empty on init
		mUpdatePolicy->m_IOManager = mIOManager;
		mUpdatePolicy->m_StateManager = mStateManager;

		// io mgr: empty on init
		mIOManager->m_StateManager = mStateManager;
		mIOManager->m_UpdatePolicy = mUpdatePolicy;
	}

	template< typename THandle >
	FunctionBlock< THandle >::~FunctionBlock()
	{
		delete mUpdatePolicy;
		delete mIOManager;
		delete mStateManager;
		delete mBlock;				// deletes the block!!!!!!!
	}

	//template< typename THandle >
	//std::string const& FunctionBlock< THandle >::getBundleName() const
	//{
	//	return mBundle->getName();
	//}

	template< typename THandle >
	bool FunctionBlock< THandle >::isRunning() const
	{
		return mStateManager->isRunning();
	}

	template< typename THandle >
	const std::string FunctionBlock< THandle >::getUpdateRateAsString() const
	{
		std::ostringstream str;
		str << mUpdatePolicy->getUpdateRate();
		return str.str();
	}

	template< typename THandle >
	void FunctionBlock< THandle >::addInlet( InletMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type )
	{
		IOInfo *info = new IOInfo( meta.name, initializer, type, meta.defaultPolicy );
		meta.isMulti ? mIOManager->addMultiInlet( info ) : mIOManager->addBasicInlet( info );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::addOutlet( OutletMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type )
	{
		IOInfo *info = new IOInfo( meta.name, initializer, type, Policy::INVALID );
		mIOManager->addOutlet( info );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::addParameter( ParameterMetadata const& meta, std::shared_ptr< const CustomType > initializer, TypeMetadata const& type )
	{
		IOInfo *info = new IOInfo( meta.name, initializer, type, Policy::INVALID );
		mIOManager->addParameter( info );
	}

	template< typename THandle >
	app::BlockInfo const& FunctionBlock< THandle >::getBlockInfo()
	{
		return mBlockInfo;
	}

	template< typename THandle >
	app::InletHandle FunctionBlock< THandle >::getAppInletHandle( std::string const& name ) const
	{
		return mIOManager->getAppInletHandle( name );
	}

	template< typename THandle >
	app::ParameterHandle FunctionBlock< THandle >::getAppParameterHandle( std::string const& name ) const
	{
		return mIOManager->getAppParameterHandle( name );
	}

	template< typename THandle >
	app::OutletHandle FunctionBlock< THandle >::getAppOutletHandle( std::string const& name ) const
	{
		return mIOManager->getAppOutletHandle( name );
	}

	//template< typename THandle >
	//bundle::InletHandle FunctionBlock< THandle >::getBundleInletHandle( std::string const& name ) const
	//{
	//	return mIOManager->getBundleInletHandle( name );
	//}

	//template< typename THandle >
	//bundle::OutletHandle FunctionBlock< THandle >::getBundleOutletHandle( std::string const& name ) const
	//{
	//	return mIOManager->getBundleOutletHandle( name );
	//}

	//template< typename THandle >
	//bundle::ParameterHandle FunctionBlock< THandle >::getBundleParameterHandle( std::string const& name ) const
	//{
	//	return mIOManager->getBundleParameterHandle( name );
	//}

	template< typename THandle >
	AppInletHandles const& FunctionBlock< THandle >::getAppInletHandles() const
	{
		return mIOManager->getAppInletHandles();
	}

	template< typename THandle >
	AppOutletHandles const& FunctionBlock< THandle >::getAppOutletHandles() const
	{
		return mIOManager->getAppOutletHandles();
	}

	template< typename THandle >
	AppParameterHandles const& FunctionBlock< THandle >::getAppParameterHandles() const
	{
		return mIOManager->getAppParameterHandles();
	}

	//template< typename THandle >
	//BundleInletHandles const& FunctionBlock< THandle >::getBundleInletHandles() const
	//{
	//	return mIOManager->getBundleInletHandles();
	//}

	//template< typename THandle >
	//BundleOutletHandles const& FunctionBlock< THandle >::getBundleOutletHandles() const
	//{
	//	return mIOManager->getBundleOutletHandles();
	//}

	//template< typename THandle >
	//BundleParameterHandles const& FunctionBlock< THandle >::getBundleParameterHandles() const
	//{
	//	return mIOManager->getBundleParameterHandles();
	//}

	template< typename THandle >
	void FunctionBlock< THandle >::registerToNewData( app::BlockCallback &callback )
	{
		mIOManager->registerToNewData( callback );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::unregisterFromNewData( app::BlockCallback &callback )
	{
		mIOManager->unregisterFromNewData( callback );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::setUp()
	{
		mStateManager->setUp();
	}

	template< typename THandle >
	void FunctionBlock< THandle >::start()
	{
		mStateManager->start();
	}

	template< typename THandle >
	void FunctionBlock< THandle >::stop( const bool blocking, const long timeout )
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

	template< typename THandle >
	void FunctionBlock< THandle >::prepareForShutDown()
	{
		mStateManager->prepareForShutDown();
	}

	template< typename THandle >
	bool FunctionBlock< THandle >::shutDown( const long timeout )
	{
		return mStateManager->shutDown( timeout );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::singleStep()
	{
		mStateManager->singleStep();
	}

	template< typename THandle >
	void FunctionBlock< THandle >::updateWithFixedRate( const double updatesPerSecond )
	{
		mUpdatePolicy->setNewUpdateRate( updatesPerSecond );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::handleException( Exception &e )
	{
		mEngineImpl->handleException( getHandle(), e );
	}

	template< typename THandle >
	AbstractInletIO & FunctionBlock< THandle >::getInlet( std::string const& inletName )
	{
		return mIOManager->getInletIO( inletName );
	}

	template< typename THandle >
	OutletIO & FunctionBlock< THandle >::getOutlet( std::string const& outletName )
	{
		return mIOManager->getOutletIO( outletName );
	}

	typedef FunctionBlock< app::BlockHandle >			BlockInstance;
	typedef FunctionBlock< app::ContextBlockHandle >	ContextBlockInstance;

}
