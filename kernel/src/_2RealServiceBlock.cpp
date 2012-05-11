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
#include "_2RealIService.h"
#include "_2RealServiceMetadata.h"
#include "_2RealServiceContext.h"
#include "_2RealSystemImpl.h"

namespace _2Real
{

	ServiceBlock::ServiceBlock(ServiceData const& data, SystemImpl &owner, UpdatePolicyImpl const& updateTriggers) :
		Block< ServiceIO, DisabledBlocks, DisabledBlocks, ServiceStates/*, ServiceUpdates */>(data.name, &owner),
		m_Service(data.service)
	{
		ServiceIO *io = dynamic_cast< ServiceIO * >(m_IOManager);
		//ServiceUpdates *triggers = dynamic_cast< ServiceUpdates * >(m_TriggerManager);
		ServiceStates *states = dynamic_cast< ServiceStates * >(m_StateManager);

		//triggers->m_States = states;
		//triggers->m_IO = io;
		//states->m_Triggers = triggers;
		states->m_IO = io;

		io->initFrom(*data.metainfo, owner.getTimestamp());
		states->initFrom(updateTriggers);
	}

	ServiceBlock::~ServiceBlock()
	{
		delete m_Service;
	}

	void ServiceBlock::setUpService()
	{
		ServiceContext context(*this);
		m_Service->setup(context);	//might throw
	}

	void ServiceBlock::executeService()
	{
		m_Service->update();		//might throw
	}

	void ServiceBlock::shutDownService()
	{
		m_Service->shutdown();		//might throw
	}

	InputHandle ServiceBlock::createInputHandle(std::string const& name)
	{
		return dynamic_cast< ServiceIO * >(m_IOManager)->createInputHandle(name);
	}

	OutputHandle ServiceBlock::createOutputHandle(std::string const& name)
	{
		return dynamic_cast< ServiceIO * >(m_IOManager)->createOutputHandle(name);
	}

}