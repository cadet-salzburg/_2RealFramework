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

#include "_2RealServiceFactory.h"
#include "_2RealPlugin.h"
#include "_2RealEntityTable.h"
#include "_2RealTypes.h"
#include "_2RealSystemGraph.h"
#include "_2RealService.h"
#include "_2RealInputSlot.h"
#include "_2RealOutputSlot.h"
#include "_2RealSetupParameter.h"

//#include <sstream>
//#include <iostream>

namespace _2Real
{

	ServiceFactory::ServiceFactory(Engine &_engine) :
		m_Engine(_engine)
	{
	}

	const Identifier ServiceFactory::createService(std::string const& _name, Plugin *const _plugin, std::string const& _service, SystemGraph *const _system)
	{
		IService *userService = _plugin->createService(_service);

		if (userService == NULL)
		{
			throw Exception("service factory: internal exception - could not create instance of service " + _service);
		}

		const Identifier id = m_Engine.entities().createIdentifier(_name, "service");
		Service *service = new Service(userService, id, _system);

		const ServiceMetadata data = _plugin->serviceMetadata(_service);

		typedef std::map< std::string, std::string > StringMap;
		StringMap setup = data.getSetupParameters();
		StringMap input = data.getInputParameters();
		StringMap output = data.getOutputParameters();

		for (StringMap::iterator it = setup.begin(); it != setup.end(); it++)
		{
			const Identifier id = m_Engine.entities().createIdentifier(it->first, "setup parameter");
			SetupParameter *setup = new SetupParameter(id, m_Engine.types().getTypename(it->second), it->second);
			service->addSetupParameter(setup);
		}

		for (StringMap::iterator it = input.begin(); it != input.end(); it++)
		{
			const Identifier id = m_Engine.entities().createIdentifier(it->first, "input slot");
			InputSlot *in = new InputSlot(id, service, m_Engine.types().getTypename(it->second), it->second);
			service->addInputSlot(in);
		}

		for (StringMap::iterator it = output.begin(); it != output.end(); it++)
		{
			SharedAny init;
			// ~~ createfromkeyword
			m_Engine.types().create(it->second, init);
			const Identifier id = m_Engine.entities().createIdentifier(it->first, "output slot");
			OutputSlot *out = new OutputSlot(id, service, m_Engine.types().getTypename(it->second), it->second, init);
			service->addOutputSlot(out);
		}

		unsigned int index = _system->childCount();
		_system->insertChild(service, index);

		return id;
	}
}