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
#include "_2RealData.h"
#include "_2RealRunnableManager.h"
#include "_2RealSequence.h"
#include "_2RealSynchronization.h"

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
			std::cout << "nirvana: stopping child " << (*it)->getManagedId() << std::endl;
			(*it)->stop();
		}

		//kills all threads
		m_Threads.clearThreads();

		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			(*it)->shutdown();
		}

		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
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
		Runnable &runnable = getContained(serviceId).getManagedRunnable();
		Service &service = static_cast< Service & >(runnable);
		service.registerToNewData(outName, callback);
	}

	void SystemGraph::unregisterFromNewData(Identifier const& serviceId, std::string const& outName, DataCallback callback)
	{
		Runnable &runnable = getContained(serviceId).getManagedRunnable();
		Service &service = static_cast< Service & >(runnable);
		service.unregisterFromNewData(outName, callback);
	}

	void SystemGraph::registerToNewData(Identifier const& serviceId, std::string const& outName, IOutputListener &listener)
	{
		Runnable &runnable = getContained(serviceId).getManagedRunnable();
		Service &service = static_cast< Service & >(runnable);
		service.registerToNewData(outName, listener);
	}

	void SystemGraph::unregisterFromNewData(Identifier const& serviceId, std::string const& outName, IOutputListener &listener)
	{
		Runnable &runnable = getContained(serviceId).getManagedRunnable();
		Service &service = static_cast< Service & >(runnable);
		service.unregisterFromNewData(outName, listener);
	}

	void SystemGraph::add(Identifier const& runnable, Identifier const& parent, unsigned int index)
	{
		if (parent == identifier())
		{
			//system does not care about ordering anyway
			append(runnable, parent);
		}
		else
		{
			RunnableManager &runnableMgr = getContained(runnable);
			RunnableGraph &newParent = static_cast< RunnableGraph & >(getContained(parent).getManagedRunnable());
			
			Graph &oldParent = getContained(runnable).getManagedRunnable().father();

			if (&oldParent == this)
			{
				//if the old parent is the system, the child might run
				if (runnableMgr.isRunning())
				{
					runnableMgr.stop();
				}

				RunnableList::iterator it = iteratorId(runnable);
				m_Children.erase(it);
			}
			else
			{
				static_cast< RunnableGraph & >(oldParent).removeChild(runnable);
			}

			newParent.insertChild(runnableMgr, index);
		}
	}

	void SystemGraph::append(Identifier const& runnable, Identifier const& parent)
	{
		if (parent == identifier())
		{
			if (isChild(runnable))
			{
				return;
			}

			RunnableManager &runnableMgr = getContained(runnable);
			Graph &oldParent = runnableMgr.getManagedRunnable().father();
			//i can safely do this as the old parent is not the system itself
			static_cast< RunnableGraph & >(oldParent).removeChild(runnable);
			m_Children.push_back(&runnableMgr);
		}
		else
		{
			RunnableManager &runnableMgr = getContained(runnable);
			RunnableGraph &newParent = static_cast< RunnableGraph & >(getContained(parent).getManagedRunnable());
			
			Graph &oldParent = getContained(runnable).getManagedRunnable().father();

			if (&oldParent == this)
			{
				//if the old parent is the system, the child might run
				if (runnableMgr.isRunning())
				{
					runnableMgr.stop();
				}

				RunnableList::iterator it = iteratorId(runnable);
				m_Children.erase(it);
			}
			else
			{
				static_cast< RunnableGraph & >(oldParent).removeChild(runnable);
			}

			newParent.insertChild(runnableMgr, newParent.childCount());
		}
	}

	void SystemGraph::startAll()
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if (!(*it)->isRunning())
			{
				(*it)->start(m_Threads.getFreeThread());
			}
		}
	}

	void SystemGraph::stopAll()
	{
		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); it++)
		{
			if ((*it)->isRunning())
			{
				std::cout << "stopping: " << (*it)->getManagedId().name() << std::endl;
				(*it)->stop();
				//(*it)->wait();
				std::cout << "stopped: " << (*it)->getManagedId().name() << std::endl;
			}
		}
	}

	void SystemGraph::startChild(Identifier const& runnableId)
	{
		RunnableManager &manager = getChild(runnableId);
		if (!manager.isRunning())
		{
			manager.start(m_Threads.getFreeThread());
		}
	}

	void SystemGraph::stopChild(Identifier const& runnableId)
	{
		RunnableManager &manager = getChild(runnableId);
		if (manager.isRunning())
		{
			manager.stop();
		}
	}

	void SystemGraph::handleException(Runnable &runnable, Exception &exception)
	{
		stopChild(runnable.root().identifier());
		m_ExceptionHandler.handleException(exception, runnable.identifier());
	}

	const Identifier SystemGraph::loadPlugin(std::string const& name, std::string const& classname)
	{
		return m_Plugins.install(name, classname);
	}

	void SystemGraph::setUp(Identifier const& id)
	{
		if (id.isPlugin())
		{
			m_Plugins.setup(id);
		}
		else if (id.isService())
		{
			RunnableManager &mgr = getContained(id);
			mgr.setup();
		}
	}

	const Identifier SystemGraph::createService(std::string const& name, Identifier const& id, std::string const& service)
	{
		Runnable &runnable = m_Plugins.createService(name, id, service);
		RunnableManager *manager = new RunnableManager(runnable);
		m_Children.push_back(manager);
		return runnable.identifier();
	}

	const Identifier SystemGraph::createSequence(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB)
	{
		std::cout << "sequence creation" << std::endl;

		const Identifier id = Entity::createIdentifier(idName, "sequence");
		Sequence *seq = new Sequence(id, *this);
		RunnableManager &mgrA = getContained(runnableA);
		RunnableManager &mgrB = getContained(runnableB);

		//std::cout << "got contained runnables" << std::endl;

		Graph &parentA = mgrA.getManagedRunnable().father();
		if (this == &parentA)
		{
			//std::cout << "removing from: nirvana" << std::endl;

			if (mgrA.isRunning())
			{
				//std::cout << "stopping" << std::endl;
				mgrA.stop();
			}

			RunnableList::iterator it = iteratorId(runnableA);
			m_Children.erase(it);
		}
		else
		{
			//std::cout << "removing from: other" << std::endl;
			static_cast< RunnableGraph & >(parentA).removeChild(runnableA);
		}

		//std::cout << "removed first" << std::endl;

		Graph &parentB = mgrB.getManagedRunnable().father();
		if (this == &parentB)
		{
			//std::cout << "removing from: nirvana" << std::endl;

			if (mgrB.isRunning())
			{
				//std::cout << "stopping" << std::endl;
				mgrB.stop();
			}

			RunnableList::iterator it = iteratorId(runnableB);
			m_Children.erase(it);
		}
		else
		{
			//std::cout << "removing from: other" << std::endl;
			static_cast< RunnableGraph & >(parentB).removeChild(runnableB);
		}

		//std::cout << "removed second" << std::endl;

		seq->insertChild(mgrB, 0);
		seq->insertChild(mgrA, 1);

		//std::cout << "inserted both" << std::endl;

		RunnableManager *manager = new RunnableManager(*seq);
		m_Children.push_back(manager);
		manager->setup();

		//std::cout << "seq created & set up" << std::endl;

		return id;
	}

	const Identifier SystemGraph::createSynchronization(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB)
	{
		//std::cout << "synchronization creation" << std::endl;

		const Identifier id = Entity::createIdentifier(idName, "synchronization");
		Synchronization *sync = new Synchronization(id, *this);
		RunnableManager &mgrA = getContained(runnableA);
		RunnableManager &mgrB = getContained(runnableB);

		//std::cout << "got contained runnables" << std::endl;

		Graph &parentA = mgrA.getManagedRunnable().father();
		if (this == &parentA)
		{
			//std::cout << "removing from: nirvana" << std::endl;
			if (mgrA.isRunning())
			{
				//std::cout << "stopping" << std::endl;
				mgrA.stop();
			}

			RunnableList::iterator it = iteratorId(runnableA);
			m_Children.erase(it);
		}
		else
		{
			//std::cout << "removing from: other" << std::endl;
			static_cast< RunnableGraph & >(parentA).removeChild(runnableA);
		}

		//std::cout << "removed first" << std::endl;

		Graph &parentB = mgrB.getManagedRunnable().father();
		if (this == &parentB)
		{
			//std::cout << "removing from: nirvana" << std::endl;

			if (mgrB.isRunning())
			{
				//std::cout << "stopping" << std::endl;
				mgrB.stop();
			}

			RunnableList::iterator it = iteratorId(runnableB);
			m_Children.erase(it);
		}
		else
		{
			//std::cout << "removing from: other" << std::endl;
			static_cast< RunnableGraph & >(parentB).removeChild(runnableB);
		}

		//std::cout << "removed second" << std::endl;

		sync->insertChild(mgrB, 0);
		sync->insertChild(mgrA, 1);

		//std::cout << "inserted both" << std::endl;

		RunnableManager *manager = new RunnableManager(*sync);
		m_Children.push_back(manager);
		manager->setup();

		//std::cout << "sync created & set up" << std::endl;

		return id;
	}

	void SystemGraph::setValue(Identifier const& id, std::string const& paramName, EngineData const& value)
	{
		Data data(value, m_Engine.getTimestamp());
		if (id.isPlugin())
		{
			m_Plugins.setParameterValue(id, paramName, data);
		}
		else if (id.isService())
		{
			Runnable &runnable = getContained(id).getManagedRunnable();
			Service &service = static_cast< Service & >(runnable);
			service.setParameterValue(paramName, data);
		}
	}

	const EngineData SystemGraph::getValue(Identifier const& id, std::string const& paramName) const
	{
		if (id.isPlugin())
		{
			return m_Plugins.getParameterValue(id, paramName);
		}
		else
		{
			Runnable &runnable = getContained(id).getManagedRunnable();
			Service &service = static_cast< Service & >(runnable);
			return service.getParameterValue(paramName);
		}
	}

	void SystemGraph::setUpdateRate(Identifier const& id, float updatesPerSecond)
	{
		std::cout << "setting update rate of " << id << std::endl;
		Runnable &runnable = getContained(id).getManagedRunnable();
		runnable.setUpdateRate(updatesPerSecond);
	}

	void SystemGraph::linkSlots(Identifier const& serviceIn, std::string const& nameIn, Identifier const& serviceOut, std::string const& nameOut)
	{
		Runnable &runnableIn = getContained(serviceIn).getManagedRunnable();
		Service &in = static_cast< Service & >(runnableIn);

		Runnable &runnableOut = getContained(serviceOut).getManagedRunnable();
		Service &out = static_cast< Service & >(runnableOut);

		in.linkWith(nameIn, out, nameOut);
	}

}