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

#include "_2RealFunctionBlock.h"
#include "_2RealBlock.h"
#include "_2RealBlockData.h"
#include "_2RealSystemBlock.h"
#include "_2RealUpdatePolicy.h"
#include "bundle/_2RealParameterHandle.h"
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealOutletHandle.h"
#include "app/_2RealInletHandle.h"
#include "app/_2RealOutletHandle.h"
#include "app/_2RealParameterHandle.h"

using std::string;

namespace _2Real
{

	FunctionBlock::FunctionBlock( BlockData const& data, bundle::Block &block, SystemBlock &owner, BlockIdentifier const& id ) :
		UberBlock< FunctionBlockIOManager, FunctionBlockStateManager>( id ),
		m_BlockData( data ),
		m_Block( block ),
		m_System( owner )
	{
		m_StateManager->m_IOManager = m_IOManager;
		m_StateManager->m_System = &owner;
		m_StateManager->m_FunctionBlock = &block;
		m_StateManager->m_UpdatePolicy = m_UpdatePolicy;

		ParameterDataMap const& setup = data.getParameters();
		ParameterDataMap const& input = data.getInlets();
		ParameterDataMap const& output = data.getOutlets();

		for ( ParameterDataMap::const_iterator it = setup.begin(); it != setup.end(); ++it )
		{
			m_IOManager->addParameter( it->second );
		}

		for ( ParameterDataMap::const_iterator it = input.begin(); it != input.end(); ++it )
		{
			m_IOManager->addInlet( it->second );
			m_UpdatePolicy->addInlet( it->second.getName() );
		}

		for ( ParameterDataMap::const_iterator it = output.begin(); it != output.end(); ++it )
		{
			m_IOManager->addOutlet( it->second );
		}
	}

	BlockData const& FunctionBlock::getMetadata()
	{
		return m_BlockData;
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

}