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
#include "engine/_2RealFunctionBlockStateManager.h"
#include "engine/_2RealFunctionBlockIOManager.h"
#include "../_2RealBlock.h"
#include "engine/_2RealBlockData.h"
#include "app/_2RealBlockData.h"
#include "engine/_2RealSystem.h"
#include "engine/_2RealFunctionBlockUpdatePolicy.h"
#include "bundle/_2RealParameterHandle.h"
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "app/_2RealParameterHandle.h"

using std::string;

namespace _2Real
{

	FunctionBlock::FunctionBlock( BlockData const& meta, bundle::Block &block, System &owner, Identifier const& id ) :
		AbstractUberBlock( id ),
		m_Metadata( meta ),
		m_Block( block ),
		m_System( owner ),
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

		BlockData::ParamMetas const& params = meta.getParameters();
		BlockData::ParamMetas const& input = meta.getInlets();
		BlockData::ParamMetas const& output = meta.getOutlets();

		for ( BlockData::ParamMetasConstIterator it = params.begin(); it != params.end(); ++it )
		{
			m_IOManager->addParameter( *it );
		}

		for ( BlockData::ParamMetasConstIterator it = input.begin(); it != input.end(); ++it )
		{
			m_IOManager->addInlet( *it );
		}

		for ( BlockData::ParamMetasConstIterator it = output.begin(); it != output.end(); ++it )
		{
			m_IOManager->addOutlet( *it );
		}
	}

	FunctionBlock::~FunctionBlock()
	{
	}

	BlockData const& FunctionBlock::getMetadata() const
	{
		return m_Metadata;
	}

	app::BlockInfo FunctionBlock::getBlockData()
	{
		app::BlockInfo blockData;

		blockData.m_Name = m_Metadata.getName();
		blockData.m_Description = m_Metadata.getDescription();
		blockData.m_Category = m_Metadata.getCategory();

		BlockData::ParamMetas const& params = m_Metadata.getParameters();
		BlockData::ParamMetas const& input = m_Metadata.getInlets();
		BlockData::ParamMetas const& output = m_Metadata.getOutlets();
		for ( BlockData::ParamMetasConstIterator it = params.begin(); it != params.end(); ++it )
		{
			app::ParameterData paramData;

			paramData.m_Name = it->getName();
			paramData.m_Typename = it->getTypename();
			paramData.m_LongTypename = it->getLongTypename();

			blockData.m_Parameters.push_back( paramData );
		}

		for ( BlockData::ParamMetasConstIterator it = input.begin(); it != input.end(); ++it )
		{
			app::ParameterData paramData;

			paramData.m_Name = it->getName();
			paramData.m_Typename = it->getTypename();
			paramData.m_LongTypename = it->getLongTypename();

			blockData.m_Inlets.push_back( paramData );
		}

		for ( BlockData::ParamMetasConstIterator it = output.begin(); it != output.end(); ++it )
		{
			app::ParameterData paramData;

			paramData.m_Name = it->getName();
			paramData.m_Typename = it->getTypename();
			paramData.m_LongTypename = it->getLongTypename();

			blockData.m_Outlets.push_back( paramData );
		}

		return blockData;
	}

	app::InletHandle FunctionBlock::createAppInletHandle( string const& name )
	{
		return m_IOManager->createAppInletHandle( name );
	}

	app::OutletHandle FunctionBlock::createAppOutletHandle( string const& name )
	{
		return m_IOManager->createAppOutletHandle( name );
	}

	app::ParameterHandle FunctionBlock::createAppParameterHandle( string const& name )
	{
		return m_IOManager->createAppParameterHandle( name );
	}

	bundle::InletHandle FunctionBlock::createBundleInletHandle( string const& name )
	{
		return m_IOManager->createBundleInletHandle( name );
	}

	bundle::OutletHandle FunctionBlock::createBundleOutletHandle( string const& name )
	{
		return m_IOManager->createBundleOutletHandle( name );
	}

	bundle::ParameterHandle FunctionBlock::createBundleParameterHandle( string const& name )
	{
		return m_IOManager->createBundleParameterHandle( name );
	}

	void FunctionBlock::createLink( Inlet &inlet, Outlet &outlet )
	{
		m_System.createLink( inlet, outlet );
	}

	void FunctionBlock::destroyLink( Inlet &inlet, Outlet &outlet )
	{
		m_System.destroyLink( inlet, outlet );
	}

	void FunctionBlock::registerToNewData( Outlet const& outlet, app::OutletCallback &callback )
	{
		m_IOManager->registerToNewData( outlet, callback );
	}

	void FunctionBlock::unregisterFromNewData( Outlet const& outlet, app::OutletCallback &callback )
	{
		m_IOManager->unregisterFromNewData( outlet, callback );
	}

	void FunctionBlock::registerToNewData( app::BlockCallback &callback )
	{
		m_IOManager->registerToNewData( callback );
	}

	void FunctionBlock::unregisterFromNewData( app::BlockCallback &callback )
	{
		m_IOManager->unregisterFromNewData( callback );
	}

	void FunctionBlock::setUp()
	{
		m_StateManager->setUp();
	}

	void FunctionBlock::start()
	{
		m_StateManager->start();
	}

	void FunctionBlock::stop( const bool blocking, const long timeout )
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

	void FunctionBlock::prepareForShutDown()
	{
		m_StateManager->prepareForShutDown();
	}

	bool FunctionBlock::shutDown( const long timeout )
	{
		return m_StateManager->shutDown( timeout );
	}

	void FunctionBlock::updateWhenInletDataNew( Inlet &inlet )
	{
		m_UpdatePolicy->setNewInletPolicy( inlet, FunctionBlockUpdatePolicy::InletTriggerCtor( new InletTriggerCreator< NewerTimestamp >() ) );
	}

	void FunctionBlock::updateWhenInletDataValid( Inlet &inlet )
	{
		m_UpdatePolicy->setNewInletPolicy( inlet, FunctionBlockUpdatePolicy::InletTriggerCtor( new InletTriggerCreator< ValidData >() ) );
	}

	void FunctionBlock::updateWhenAllInletDataNew()
	{
		m_UpdatePolicy->setNewInletDefaultPolicy( FunctionBlockUpdatePolicy::InletTriggerCtor( new InletTriggerCreator< NewerTimestamp >() ) );
	}

	void FunctionBlock::updateWhenAllInletDataValid()
	{
		m_UpdatePolicy->setNewInletDefaultPolicy( FunctionBlockUpdatePolicy::InletTriggerCtor( new InletTriggerCreator< ValidData >() ) );
	}

	void FunctionBlock::updateWithFixedRate( const double updatesPerSecond )
	{
		if ( updatesPerSecond == 0. )
		{
			m_UpdatePolicy->setNewUpdateTime( -1 );
		}

		// timeslice is in microseconds, since that's the resolution of a poco::timestamp
		// however, the maximum timer update rate obtainable is ~ 2 milliseconds,
		// and that's with a totally empty system -> silly update rates greater than 60 hertz won't be met, probably
		double micros = 1000000/updatesPerSecond;
		m_UpdatePolicy->setNewUpdateTime( static_cast< long >(micros) );
	}

	void FunctionBlock::handleException( Exception &e )
	{
		m_System.handleException( *this, e );
	}

}