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

#include "_2RealSystemPool.h"
#include "_2RealSystemGraph.h"

#include <sstream>

namespace _2Real
{

	SystemPool::SystemPool() :
		m_SystemGraphs()
	{
	}

	SystemPool::~SystemPool()
	{
	}

	void SystemPool::clearSystemGraphs()
	{
		std::ostringstream msg;
		bool error = false;
		for (SystemGraphTable::iterator it = m_SystemGraphs.begin(); it != m_SystemGraphs.end(); ++it)
		{
			Identifier id = it->first;
			try
			{
				it->second->shutdown();
			}
			catch (_2Real::Exception &e)
			{
				error = true;
				msg << "exception on production graph shutdown: " << id.name() << " " << e.message() << std::endl;
			}
			catch (std::exception &e)
			{
				error = true;
				msg << "exception on production graph shutdown: " << id.name() << " " << e.what() << std::endl;
			}
			delete it->second;
		}
		m_SystemGraphs.clear();
		if (error)
		{
			throw Exception(msg.str());
		}
	}

	const Identifier SystemPool::createSystemGraph(std::string const& idName)
	{
		const Identifier id = Entity::createIdentifier(idName, "system");
		SystemGraph *graph = new SystemGraph(id);
		m_SystemGraphs.insert(NamedSystemGraph(id, graph));
		return id;
	}

	void SystemPool::destroySystemGraph(Identifier const& system)
	{
		SystemGraphTable::iterator it = m_SystemGraphs.find(system);

		if (it == m_SystemGraphs.end())
		{
			std::ostringstream msg;
			msg << "system: " << system.name() << "does not exist";
			throw NotFoundException(msg.str());
		}

		SystemGraph *nirvana = it->second;
		nirvana->shutdown();
		delete nirvana;
		m_SystemGraphs.erase(it);
	}

	SystemGraph & SystemPool::getSystemGraph(Identifier const& system)
	{
		SystemGraphTable::iterator it = m_SystemGraphs.find(system);

		if (it == m_SystemGraphs.end())
		{
			std::ostringstream msg;
			msg << "system: " << system.name() << "does not exist";
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	SystemGraph const& SystemPool::getSystemGraph(Identifier const& system) const
	{
		SystemGraphTable::const_iterator it = m_SystemGraphs.find(system);

		if (it == m_SystemGraphs.end())
		{
			std::ostringstream msg;
			msg << "system: " << system.name() << "does not exist";
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

}