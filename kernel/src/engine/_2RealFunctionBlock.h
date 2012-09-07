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

namespace _2Real
{

	class AnyOptionSet;

	template< typename THandle >
	class FunctionBlock :  public AbstractUberBlock, private Handleable< FunctionBlock< THandle >, THandle >
	{

	public:

		FunctionBlock( Bundle const& owningBundle, bundle::Block &block, app::BlockInfo const& info );
		~FunctionBlock();

		using Handleable< FunctionBlock< THandle >, THandle >::getHandle;
		using Handleable< FunctionBlock< THandle >, THandle >::registerHandle;
		using Handleable< FunctionBlock< THandle >, THandle >::unregisterHandle;

		using AbstractUberBlock::setName;
		using AbstractUberBlock::getName;
		using AbstractUberBlock::getFullName;

		std::string const&			getBundleName() const;
		const std::string			getUpdateRateAsString() const;
		const std::string			getUpdatePolicyAsString( std::string const& inlet ) const;
		const std::string			getBufferSizeAsString( std::string const& inlet ) const;
		bool						isRunning() const;

		app::BlockInfo const&		getBlockInfo();

		bundle::InletHandle &		getBundleInletHandle( std::string const& inletName ) const;
		bundle::OutletHandle &		getBundleOutletHandle( std::string const& outletName ) const;
		app::InletHandle &			getAppInletHandle( std::string const& inletName ) const;
		app::OutletHandle &			getAppOutletHandle( std::string const& outletName ) const;

		AppInletHandles const&		getAppInletHandles() const;
		AppOutletHandles const&		getAppOutletHandles() const;
		BundleInletHandles const&	getBundleInletHandles() const;
		BundleOutletHandles const&	getBundleOutletHandles() const;

		void						registerToNewData( app::BlockCallback &callback );
		void						unregisterFromNewData( app::BlockCallback &callback );

		void						setUp();
		void						start();
		void						stop( const bool blocking, const long timeout );
		void						prepareForShutDown();
		bool						shutDown( const long timeout );
		void						singleStep();

		void						updateWithFixedRate( const double updatesPerSecond );

		void						handleException( Exception &e );

		void						addInlet( std::string const& name, TypeDescriptor const& type, Any const& initialValue, AnyOptionSet const& options, const bool isMulti );
		void						addOutlet( std::string const& name, TypeDescriptor const& type, Any const& initialValue );

	private:

		EngineImpl					&m_Engine;
		Bundle						const& m_Bundle;
		bundle::Block				*m_Block;
		app::BlockInfo				m_BlockInfo;

		FunctionBlockUpdatePolicy	*m_UpdatePolicy;
		FunctionBlockIOManager		*m_IOManager;
		FunctionBlockStateManager	*m_StateManager;

		Refs						m_Refs;

	};

	template< typename THandle >
	FunctionBlock< THandle >::FunctionBlock( Bundle const& owningBundle, bundle::Block &block, app::BlockInfo const& info ) :
		AbstractUberBlock( owningBundle.getIds(), info.name ),
		Handleable< FunctionBlock< THandle >, THandle >( *this ),
		m_Engine( EngineImpl::instance() ),
		m_Bundle( owningBundle ),
		m_Block( &block ),
		m_BlockInfo( info ),
		m_UpdatePolicy( new FunctionBlockUpdatePolicy( *this ) ),
		m_IOManager( new FunctionBlockIOManager( *this ) ),
		m_StateManager( new FunctionBlockStateManager( *this ) ),
		m_Refs( m_Engine, *this, *m_IOManager, *m_StateManager, *m_UpdatePolicy )
	{
		m_StateManager->m_FunctionBlock = &block;
		m_StateManager->m_IOManager = m_IOManager;
		m_StateManager->m_UpdatePolicy = m_UpdatePolicy;

		m_UpdatePolicy->m_IOManager = m_IOManager;
		m_UpdatePolicy->m_StateManager = m_StateManager;

		m_IOManager->m_StateManager = m_StateManager;
		m_IOManager->m_UpdatePolicy = m_UpdatePolicy;
	}

	template< typename THandle >
	FunctionBlock< THandle >::~FunctionBlock()
	{
		delete m_UpdatePolicy;
		delete m_IOManager;
		delete m_StateManager;
		delete m_Block;
	}

	template< typename THandle >
	std::string const& FunctionBlock< THandle >::getBundleName() const
	{
		return m_Bundle.getName();
	}

	template< typename THandle >
	bool FunctionBlock< THandle >::isRunning() const
	{
		return m_StateManager->isRunning();
	}

	template< typename THandle >
	const std::string FunctionBlock< THandle >::getBufferSizeAsString( std::string const& name ) const
	{
		std::ostringstream str;
		str << m_IOManager->getInletBufferSize( name );
		return str.str();
	}

	template< typename THandle >
	const std::string FunctionBlock< THandle >::getUpdatePolicyAsString( std::string const& name ) const
	{
		std::ostringstream str;
		str << m_UpdatePolicy->getUpdatePolicyAsString( name );
		return str.str();
	}

	template< typename THandle >
	const std::string FunctionBlock< THandle >::getUpdateRateAsString() const
	{
		std::ostringstream str;
		str << m_UpdatePolicy->getUpdateRate();
		return str.str();
	}

	template< typename THandle >
	void FunctionBlock< THandle >::addInlet( std::string const& name, TypeDescriptor const& type, Any const& initialValue, AnyOptionSet const& options, const bool isMulti )
	{
		AbstractInletIO::InletInfo info( name, getName(), type, options, initialValue );
		isMulti ? m_IOManager->addMultiInlet( info ) : m_IOManager->addBasicInlet( info );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::addOutlet( std::string const& name, TypeDescriptor const& type, Any const& initialValue )
	{
		m_IOManager->addOutlet( name, type, initialValue );
	}

	template< typename THandle >
	app::BlockInfo const& FunctionBlock< THandle >::getBlockInfo()
	{
		return m_BlockInfo;
	}

	template< typename THandle >
	app::InletHandle & FunctionBlock< THandle >::getAppInletHandle( std::string const& name ) const
	{
		return m_IOManager->getAppInletHandle( name );
	}

	template< typename THandle >
	app::OutletHandle & FunctionBlock< THandle >::getAppOutletHandle( std::string const& name ) const
	{
		return m_IOManager->getAppOutletHandle( name );
	}

	template< typename THandle >
	bundle::InletHandle & FunctionBlock< THandle >::getBundleInletHandle( std::string const& name ) const
	{
		return m_IOManager->getBundleInletHandle( name );
	}

	template< typename THandle >
	bundle::OutletHandle & FunctionBlock< THandle >::getBundleOutletHandle( std::string const& name ) const
	{
		return m_IOManager->getBundleOutletHandle( name );
	}

	template< typename THandle >
	AppInletHandles const& FunctionBlock< THandle >::getAppInletHandles() const
	{
		return m_IOManager->getAppInletHandles();
	}

	template< typename THandle >
	AppOutletHandles const& FunctionBlock< THandle >::getAppOutletHandles() const
	{
		return m_IOManager->getAppOutletHandles();
	}

	template< typename THandle >
	BundleInletHandles const& FunctionBlock< THandle >::getBundleInletHandles() const
	{
		return m_IOManager->getBundleInletHandles();
	}

	template< typename THandle >
	BundleOutletHandles const& FunctionBlock< THandle >::getBundleOutletHandles() const
	{
		return m_IOManager->getBundleOutletHandles();
	}

	template< typename THandle >
	void FunctionBlock< THandle >::registerToNewData( app::BlockCallback &callback )
	{
		m_IOManager->registerToNewData( callback );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::unregisterFromNewData( app::BlockCallback &callback )
	{
		m_IOManager->unregisterFromNewData( callback );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::setUp()
	{
		m_StateManager->setUp();
	}

	template< typename THandle >
	void FunctionBlock< THandle >::start()
	{
		m_StateManager->start();
	}

	template< typename THandle >
	void FunctionBlock< THandle >::stop( const bool blocking, const long timeout )
	{
		Poco::Event & ev = m_StateManager->stop();
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
		m_StateManager->prepareForShutDown();
	}

	template< typename THandle >
	bool FunctionBlock< THandle >::shutDown( const long timeout )
	{
		return m_StateManager->shutDown( timeout );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::singleStep()
	{
		m_StateManager->singleStep();
	}

	template< typename THandle >
	void FunctionBlock< THandle >::updateWithFixedRate( const double updatesPerSecond )
	{
		m_UpdatePolicy->setNewUpdateRate( updatesPerSecond );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::handleException( Exception &e )
	{
		m_Engine.handleException( getHandle(), e );
	}

}
