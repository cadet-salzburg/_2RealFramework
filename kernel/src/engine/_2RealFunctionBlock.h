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
#include "engine/_2RealFunctionBlockIOManager.h"
#include "engine/_2RealFunctionBlockStateManager.h"
#include "engine/_2RealFunctionBlockUpdatePolicy.h"
#include "app/_2RealBlockHandle.h"
#include "app/_2RealContextBlockHandle.h"
#include "app/_2RealBlockData.h"
#include "app/_2RealParameterData.h"
#include "engine/_2RealBlockData.h"

#include "helpers/_2RealHandleable.h"
#include "helpers/_2RealNonCopyable.h"

namespace _2Real
{
	template< typename THandle >
	class FunctionBlock : public AbstractUberBlock, private NonCopyable< FunctionBlock< THandle > >, private Handleable< THandle >
	{

	public:

		FunctionBlock( BlockData const& meta, bundle::Block& block, Identifier const& id );
		~FunctionBlock();

		using Handleable< THandle >::getHandle;
		using Handleable< THandle >::registerHandle;
		using Handleable< THandle >::unregisterHandle;

		app::BlockInfo				getBlockData();
		BlockData const&			getMetadata() const;

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

		void						updateWhenInletDataNew( InletIO &inletIO, const bool isSingleWeight );
		void						updateWhenInletDataValid( InletIO &inletIO );
		void						updateWithFixedRate( const double updatesPerSecond );

		void						handleException( Exception &e );

	private:

		EngineImpl					&m_Engine;
		bundle::Block				&m_Block;
		BlockData					const& m_Metadata;

		FunctionBlockUpdatePolicy	*m_UpdatePolicy;
		FunctionBlockIOManager		*m_IOManager;
		FunctionBlockStateManager	*m_StateManager;

	};

	template< typename THandle >
	FunctionBlock< THandle >::FunctionBlock( BlockData const& meta, bundle::Block &block, Identifier const& id ) :
		AbstractUberBlock( id ),
		Handleable< THandle >( *this ),
		m_Engine( EngineImpl::instance() ),
		m_Block( block ),
		m_Metadata( meta ),
		m_StateManager( new FunctionBlockStateManager( *this ) ),
		m_IOManager( new FunctionBlockIOManager( *this ) ),
		m_UpdatePolicy( new FunctionBlockUpdatePolicy( *this ) )
	{
		m_StateManager->m_FunctionBlock = &block;

		m_StateManager->m_IOManager = m_IOManager;
		m_StateManager->m_UpdatePolicy = m_UpdatePolicy;

		m_UpdatePolicy->m_IOManager = m_IOManager;
		m_UpdatePolicy->m_StateManager = m_StateManager;
		
		m_IOManager->m_StateManager = m_StateManager;
		m_IOManager->m_UpdatePolicy = m_UpdatePolicy;

		BlockData::ParamMetas const& input = meta.getInlets();
		BlockData::ParamMetas const& output = meta.getOutlets();

		for ( BlockData::ParamMetaConstIterator it = input.begin(); it != input.end(); ++it )
		{
			m_IOManager->addInlet( *it );
		}

		for ( BlockData::ParamMetaConstIterator it = output.begin(); it != output.end(); ++it )
		{
			m_IOManager->addOutlet( *it );
		}

		string name = getName();
		name.append( " singlestep trigger" );
		Any init( false );
		ParameterData data( name, "bool", "bool", init );
		
		m_IOManager->addSingleStepTrigger( data );
	}

	template< typename THandle >
	FunctionBlock< THandle >::~FunctionBlock()
	{
		delete m_UpdatePolicy;
		delete m_IOManager;
		delete m_StateManager;
	}

	template< typename THandle >
	BlockData const& FunctionBlock< THandle >::getMetadata() const
	{
		return m_Metadata;
	}

	template< typename THandle >
	app::BlockInfo FunctionBlock< THandle >::getBlockData()
	{
		app::BlockInfo blockData;

		blockData.m_Name = m_Metadata.getName();
		blockData.m_Description = m_Metadata.getDescription();
		blockData.m_Category = m_Metadata.getCategory();

		BlockData::ParamMetas const& input = m_Metadata.getInlets();
		BlockData::ParamMetas const& output = m_Metadata.getOutlets();

		for ( BlockData::ParamMetaConstIterator it = input.begin(); it != input.end(); ++it )
		{
			app::ParameterInfo paramInfo( it->getName(), it->getTypename(), it->getLongTypename() );
			blockData.m_Inlets.push_back( paramInfo );
		}

		for ( BlockData::ParamMetaConstIterator it = output.begin(); it != output.end(); ++it )
		{
			app::ParameterInfo paramInfo( it->getName(), it->getTypename(), it->getLongTypename() );
			blockData.m_Outlets.push_back( paramInfo );
		}

		return blockData;
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
				msg << "timeout reached on " << m_Identifier.getName() << " stop()" << std::endl;
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
		m_StateManager->start();
		m_UpdatePolicy->singleStep();
	}

	template< typename THandle >
	void FunctionBlock< THandle >::updateWhenInletDataNew( InletIO &inletIO, const bool isSingleWeight )
	{
		if ( isSingleWeight )
		{
			m_UpdatePolicy->setNewInletPolicy( inletIO, new InletTriggerCtor< NewerTimestamp, true, false >() );
		}
		else
		{
			m_UpdatePolicy->setNewInletPolicy( inletIO, new InletTriggerCtor< NewerTimestamp, false, false >() );
		}
	}

	template< typename THandle >
	void FunctionBlock< THandle >::updateWhenInletDataValid( InletIO &inletIO )
	{
		m_UpdatePolicy->setNewInletPolicy( inletIO, new InletTriggerCtor< ValidData, false, false >() );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::updateWithFixedRate( const double updatesPerSecond )
	{
		if ( updatesPerSecond == 0. )
		{
			m_UpdatePolicy->setNewUpdateTime( -1 );
		}

		double micros = 1000000/updatesPerSecond;
		m_UpdatePolicy->setNewUpdateTime( static_cast< long >(micros) );
	}

	template< typename THandle >
	void FunctionBlock< THandle >::handleException( Exception &e )
	{
		m_Engine.handleException( Handleable< THandle >::getHandle(), e );
	}

	typedef FunctionBlock< app::BlockHandle >				BlockInstance;
	typedef FunctionBlock< app::ContextBlockHandle >		BundleContext;

}