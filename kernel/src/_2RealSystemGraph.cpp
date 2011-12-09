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

#include "_2RealSystemGraph.h"
#include "_2RealRunnable.h"
#include "_2RealRunnableGraph.h"
#include "_2RealExceptionHandler.h"
#include "_2RealEngine.h"
#include "_2RealException.h"
#include "_2RealService.h"

#include <sstream>

namespace _2Real
{
	SystemGraph::SystemGraph(Identifier const& id) :
		Graph(),
		Entity(id),
		m_Engine(Engine::instance()),
		m_Threads(5, 1000, 0, id.name()),
		m_Plugins(*this),
		m_ExceptionHandler(id),
		m_AllowedTypes(m_Engine.getAllowedTypes()),
		m_Logfile(""),
		m_Logstream()
	{
	}

	void SystemGraph::setLogfile(std::string const& file)
	{
		if (m_Logstream.is_open())
		{
			m_Logstream.close();
		}

		m_Logfile = file;
		startLogging();
	}

	void SystemGraph::startLogging()
	{
		if (!m_Logfile.empty())
		{
			m_Logstream.open(m_Logfile);
		}

		if (m_Logstream.is_open())
		{
			m_Logstream << m_Engine.getTimestamp() << " " << identifier() << std::endl;
		}
	}

	SystemGraph::~SystemGraph()
	{
		if (childCount() > 0)
		{
			shutdown();
		}
	}

	void SystemGraph::shutdown()
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			//for sequences & synchronizations, this calls stop on the children
			(*it)->stop();
		}

		//kills all threads
		m_Threads.clearThreads();

		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			//performs shutdown recursively
			(*it)->shutdown();
			delete *it;
		}

		m_Children.clear();

		//uninstalls all plugins
		m_Plugins.clearPlugins();

		if (m_Logstream.is_open())
		{
			m_Logstream.close();
		}
	}

	void SystemGraph::registerExceptionCallback(ExceptionCallback callback)
	{
		m_ExceptionHandler.registerExceptionCallback(callback);
	}

	void SystemGraph::unregisterExceptionCallback(ExceptionCallback callback)
	{
		m_ExceptionHandler.unregisterExceptionCallback(callback);
	}

	void SystemGraph::registerExceptionListener(IExceptionListener &listener)
	{
		m_ExceptionHandler.registerExceptionListener(listener);
	}

	void SystemGraph::unregisterExceptionListener(IExceptionListener &listener)
	{
		m_ExceptionHandler.unregisterExceptionListener(listener);
	}

	void SystemGraph::registerToNewData(Identifier const& serviceId, std::string const& outName, DataCallback callback)
	{
		Service &service = static_cast< Service & >(getContained(serviceId));
		service.registerToNewData(outName, callback);
	}

	void SystemGraph::unregisterFromNewData(Identifier const& serviceId, std::string const& outName, DataCallback callback)
	{
		Service &service = static_cast< Service & >(getContained(serviceId));
		service.unregisterFromNewData(outName, callback);
	}

	void SystemGraph::registerToNewData(Identifier const& serviceId, std::string const& outName, IOutputListener &listener)
	{
		Service &service = static_cast< Service & >(getContained(serviceId));
		service.registerToNewData(outName, listener);
	}

	void SystemGraph::unregisterFromNewData(Identifier const& serviceId, std::string const& outName, IOutputListener &listener)
	{
		Service &service = static_cast< Service & >(getContained(serviceId));
		service.unregisterFromNewData(outName, listener);
	}

	void SystemGraph::insertChild(Runnable &child, unsigned int index)
	{
		m_Children.push_back(&child);
		child.setFather(*this);
	}

	void SystemGraph::removeChild(Identifier const& id)
	{
		RunnableList::iterator it = iteratorId(id);
		m_Children.erase(it);
	}

	void SystemGraph::startAll()
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			m_Threads.start(**it, false);
		}
	}

	void SystemGraph::stopAll()
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			m_Threads.stop((*it)->identifier());
		}
	}

	void SystemGraph::handleException(Runnable &runnable, Exception &exception)
	{
		stopChild(runnable.root().identifier());
		m_ExceptionHandler.handleException(exception, runnable.identifier());
	}

	void SystemGraph::startChild(Identifier const& runnableId)
	{
		Runnable &child = getChild(runnableId);

		//if (child.checkForUpdate())
		//{
			m_Threads.start(child, false);
		//}
		//else if (!child.isSetUp())
		//{
		//	child.setup();
		//	startChild(runnableId);
		//}
		//else
		//{
		//	std::ostringstream msg;
		//	msg << "runnable could not be started";
		//}
	}

	void SystemGraph::stopChild(Identifier const& runnableId)
	{
		m_Threads.stop(runnableId);
	}

	const Identifier SystemGraph::install(std::string const& name, std::string const& classname)
	{
		return m_Plugins.install(name, classname);
	}

	bool SystemGraph::contains(Identifier const& id) const
	{
		return (m_Plugins.contains(id) || Graph::isContained(id));
	}

	void SystemGraph::setup(Identifier const& id)
	{
		if (id.isPlugin())
		{
			m_Plugins.setup(id);
		}
		else if (id.isService())
		{
			Service &service = static_cast< Service & >(getChild(id));
			service.setup();
		}
	}

	const Identifier SystemGraph::createService(std::string const& name, Identifier const& id, std::string const& service)
	{
		return m_Plugins.createService(name, id, service);
	}

	void SystemGraph::setValue(Identifier const& id, std::string const& paramName, EngineData const& value)
	{
		if (id.isPlugin())
		{
			m_Plugins.setParameterValue(id, paramName, value);
		}
		else if (id.isService())
		{
			Service &service = static_cast< Service & >(getChild(id));
			service.setParameterValue(paramName, value);
		}
	}

	void SystemGraph::setUpdateRate(Identifier const& id, float updatesPerSecond)
	{
		Service &service = static_cast< Service & >(getChild(id));
		service.setUpdateRate(updatesPerSecond);
	}

	void SystemGraph::linkSlots(Identifier const& serviceIn, std::string const& nameIn, Identifier const& serviceOut, std::string const& nameOut)
	{
		Service &in = static_cast< Service & >(getChild(serviceIn));
		Service &out = static_cast< Service & >(getChild(serviceOut));

		in.linkWith(nameIn, out, nameOut);
	}

}