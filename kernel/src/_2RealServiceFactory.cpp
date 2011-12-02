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

	const Identifier ServiceFactory::createService(std::string const& name, Plugin const& plugin, std::string const& service, SystemGraph &system)
	{
		IService *userService = plugin.createService(service);

		const Identifier id = Entity::createIdentifier(name, "service");
		Service *runnable = new Service(id, userService, system);

		const ServiceMetadata data = plugin.getMetadata(service);

		typedef std::map< std::string, std::string > StringMap;
		StringMap const& setup = data.getSetupParameters();
		StringMap const& input = data.getInputParameters();
		StringMap const& output = data.getOutputParameters();

		for (StringMap::const_iterator it = setup.begin(); it != setup.end(); it++)
		{
			const Identifier id = Entity::createIdentifier(it->first, "setup parameter");
			SetupParameter *setup = new SetupParameter(id, m_Engine.types().getTypename(it->second), it->second);
			runnable->addSetupParameter(*setup);
		}

		for (StringMap::const_iterator it = input.begin(); it != input.end(); it++)
		{
			const Identifier id = Entity::createIdentifier(it->first, "input slot");
			InputSlot *in = new InputSlot(id, runnable, m_Engine.types().getTypename(it->second), it->second);
			runnable->addInputSlot(*in);
		}

		for (StringMap::const_iterator it = output.begin(); it != output.end(); it++)
		{
			SharedAny init;
			m_Engine.types().create(it->second, init);
			const Identifier id = Entity::createIdentifier(it->first, "output slot");
			OutputSlot *out = new OutputSlot(id, runnable, m_Engine.types().getTypename(it->second), it->second, init);
			runnable->addOutputSlot(*out);
		}

		unsigned int index = system.childCount();
		system.insertChild(*runnable, index);

		return id;
	}
}