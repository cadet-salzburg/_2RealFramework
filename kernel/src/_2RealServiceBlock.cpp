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
#include "_2RealServiceBlock.h"
#include "_2RealBlock.h"
#include "_2RealBlockData.h"
#include "_2RealSystemImpl.h"

namespace _2Real
{

	FunctionBlock::FunctionBlock( BlockData const& data, Block &block, SystemImpl &owner, BlockIdentifier const& id ) :
		UberBlock< ServiceIO, DisabledBlocks, DisabledBlocks, FunctionBlockStateManager>( id, &owner )
	{
		ServiceIO *io = dynamic_cast< ServiceIO * >( m_IOManager );
		FunctionBlockStateManager *states = dynamic_cast< FunctionBlockStateManager * >( m_StateManager );
		states->m_IO = io;
		states->m_System = &owner;
		states->m_FunctionBlock = &block;

		io->initFrom( data, owner.getTimestamp() );
	}

	InletHandle FunctionBlock::createInletHandle(std::string const& name)
	{
		return dynamic_cast< ServiceIO * >( m_IOManager )->createInletHandle( name );
	}

	OutletHandle FunctionBlock::createOutletHandle(std::string const& name)
	{
		return dynamic_cast< ServiceIO * >( m_IOManager )->createOutletHandle( name );
	}

}