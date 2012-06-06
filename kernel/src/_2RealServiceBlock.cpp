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
#include "_2RealFrameworkContext.h"
#include "_2RealSystemImpl.h"

namespace _2Real
{

	ServiceBlock::ServiceBlock( BlockData const& data, Block &block, SystemImpl &owner, UpdatePolicyImpl const& updateTriggers ) :
		UberBlock< ServiceIO, DisabledBlocks, DisabledBlocks, ServiceStates>( data.getName(), &owner ),
		m_Block( block )
	{
		ServiceIO *io = dynamic_cast< ServiceIO * >(m_IOManager);
		ServiceStates *states = dynamic_cast< ServiceStates * >(m_StateManager);
		states->m_IO = io;
		states->m_System = &owner;

		io->initFrom( data, owner.getTimestamp(), updateTriggers );
		states->initFrom( updateTriggers );
	}

	ServiceBlock::~ServiceBlock()
	{
		//delete &m_Block;
	}

	void ServiceBlock::setUpService()
	{
		FrameworkContext context(*this);
		m_Block.setup( context );	//might throw
	}

	void ServiceBlock::executeService()
	{
		m_Block.update();		//might throw
	}

	void ServiceBlock::shutDownService()
	{
		m_Block.shutdown();		//might throw
	}

	InletHandle ServiceBlock::createInletHandle(std::string const& name)
	{
		return dynamic_cast< ServiceIO * >(m_IOManager)->createInletHandle(name);
	}

	OutletHandle ServiceBlock::createOutletHandle(std::string const& name)
	{
		return dynamic_cast< ServiceIO * >(m_IOManager)->createOutletHandle(name);
	}

}