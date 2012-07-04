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
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"

using std::string;

namespace _2Real
{

	FunctionBlock::FunctionBlock( BlockData const& meta, bundle::Block &block, System &owner, Identifier const& id ) :
		AbstractUberBlock( id ),
		HandleAble< app::BlockHandle >( *this ),
		HandleAble< app::ContextBlockHandle >( *this ),
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
	}

	FunctionBlock::~FunctionBlock()
	{
		delete m_UpdatePolicy;
		delete m_IOManager;
		delete m_StateManager;
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

		BlockData::ParamMetas const& input = m_Metadata.getInlets();
		BlockData::ParamMetas const& output = m_Metadata.getOutlets();

		for ( BlockData::ParamMetaConstIterator it = input.begin(); it != input.end(); ++it )
		{
			app::ParameterData paramData;

			paramData.m_Name = it->getName();
			paramData.m_Typename = it->getTypename();
			paramData.m_LongTypename = it->getLongTypename();

			blockData.m_Inlets.push_back( paramData );
		}

		for ( BlockData::ParamMetaConstIterator it = output.begin(); it != output.end(); ++it )
		{
			app::ParameterData paramData;

			paramData.m_Name = it->getName();
			paramData.m_Typename = it->getTypename();
			paramData.m_LongTypename = it->getLongTypename();

			blockData.m_Outlets.push_back( paramData );
		}

		return blockData;
	}

	app::InletHandle & FunctionBlock::getAppInletHandle( string const& name ) const
	{
		return m_IOManager->getAppInletHandle( name );
	}

	app::OutletHandle & FunctionBlock::getAppOutletHandle( string const& name ) const
	{
		return m_IOManager->getAppOutletHandle( name );
	}

	bundle::InletHandle & FunctionBlock::getBundleInletHandle( string const& name ) const
	{
		return m_IOManager->getBundleInletHandle( name );
	}

	bundle::OutletHandle & FunctionBlock::getBundleOutletHandle( string const& name ) const
	{
		return m_IOManager->getBundleOutletHandle( name );
	}

	AppInletHandles const& FunctionBlock::getAppInletHandles() const
	{
		return m_IOManager->getAppInletHandles();
	}

	AppOutletHandles const& FunctionBlock::getAppOutletHandles() const
	{
		return m_IOManager->getAppOutletHandles();
	}

	BundleInletHandles const& FunctionBlock::getBundleInletHandles() const
	{
		return m_IOManager->getBundleInletHandles();
	}

	BundleOutletHandles const& FunctionBlock::getBundleOutletHandles() const
	{
		return m_IOManager->getBundleOutletHandles();
	}

	void FunctionBlock::createLink( InletIO &inlet, OutletIO &outlet )
	{
		m_System.createLink( inlet, outlet );
	}

	void FunctionBlock::destroyLink( InletIO &inlet, OutletIO &outlet )
	{
		m_System.destroyLink( inlet, outlet );
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

	void FunctionBlock::updateWhenInletDataNew( InletIO &inletIO, const bool isSingleWeight )
	{
		if ( isSingleWeight )
		{
			m_UpdatePolicy->setNewInletPolicy( inletIO, new InletTriggerCtor< NewerTimestamp, true >() );
		}
		else
		{
			m_UpdatePolicy->setNewInletPolicy( inletIO, new InletTriggerCtor< NewerTimestamp, false >() );
		}
	}

	void FunctionBlock::updateWhenInletDataValid( InletIO &inletIO )
	{
		m_UpdatePolicy->setNewInletPolicy( inletIO, new InletTriggerCtor< ValidData, false >() );
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