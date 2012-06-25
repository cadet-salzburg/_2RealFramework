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

#include "_2RealException.h"
#include "_2RealFunctionBlock.h"
#include "_2RealBlock.h"
#include "_2RealBlockData.h"
#include "_2RealSystemBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"

namespace _2Real
{

	FunctionBlock::FunctionBlock( BlockData const& data, Block &block, SystemBlock &owner, BlockIdentifier const& id ) :
		UberBlock< FunctionBlockIOManager, DisabledBlockManager, DisabledBlockManager, FunctionBlockStateManager>( id ),
		m_IOManager( dynamic_cast< FunctionBlockIOManager * >( UberBlock::m_IOManager ) ),
		m_StateManager( dynamic_cast< FunctionBlockStateManager * >( UberBlock::m_StateManager ) )
	{
		// state manager needs some more stuff
		m_StateManager->m_IO = m_IOManager;
		m_StateManager->m_System = &owner;
		m_StateManager->m_FunctionBlock = &block;

		m_IOManager->initFrom( data );
	}

	InletHandle FunctionBlock::createInletHandle(std::string const& name)
	{
		return m_IOManager->createInletHandle( name );
	}

	OutletHandle FunctionBlock::createOutletHandle(std::string const& name)
	{
		return m_IOManager->createOutletHandle( name );
	}

}