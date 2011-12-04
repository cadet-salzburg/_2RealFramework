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

#include "_2RealService.h"
#include "_2RealServiceContext.h"
#include "_2RealInputSlot.h"
#include "_2RealOutputSlot.h"
#include "_2RealSetupParameter.h"
#include "_2RealEngineData.h"
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"
#include "_2RealIService.h"
#include "_2RealSystemGraph.h"
//#include "_2RealEngine.h"

#include <iostream>
#include <sstream>

namespace _2Real
{

	Service::Service(Identifier const& id, IService *const service, SystemGraph &system) : 
		Runnable(id, system),
		m_Service(service),
		m_MaxDelay(long(1000.0f/30.0f)),
		m_UpdatesPerSecond(30.0f)
	{
	}

	Service::~Service()
	{
		delete m_Service;

		for (InputMap::iterator it = m_InputSlots.begin(); it != m_InputSlots.end(); it++)
		{
			delete it->second;
		}

		for (OutputMap::iterator it = m_OutputSlots.begin(); it != m_OutputSlots.end(); it++)
		{
			delete it->second;
		}

		for (ParameterMap::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
		{
			delete it->second;
		}
	}

	void Service::addInputSlot(InputSlot &slot)
	{
		m_InputSlots.insert(NamedInput(slot.name(), &slot));
	}

	void Service::addOutputSlot(OutputSlot &slot)
	{
		m_OutputSlots.insert(NamedOutput(slot.name(), &slot));
	}

	void Service::addSetupParameter(SetupParameter &parameter)
	{
		m_SetupParameters.insert(NamedParameter(parameter.name(), &parameter));
	}

	const bool Service::hasInputSlot(std::string const& name) const
	{
		InputMap::const_iterator it = m_InputSlots.find(name);
		return (it != m_InputSlots.end());
	}

	const bool Service::hasOutputSlot(std::string const& name) const
	{
		OutputMap::const_iterator it = m_OutputSlots.find(name);
		return (it != m_OutputSlots.end());
	}

	const bool Service::hasSetupParameter(std::string const& name) const
	{
		ParameterMap::const_iterator it = m_SetupParameters.find(name);
		return (it != m_SetupParameters.end());
	}

	void Service::setUpdateRate(float const& updatesPerSecond)
	{
		m_MaxDelay = long(1000.0f/updatesPerSecond);
		m_UpdatesPerSecond = updatesPerSecond;
	}

	void Service::checkConfiguration()
	{
		ServiceContext context(*this);
		m_Service->setup(context);

		//TODO
	}

	void Service::run()
	{
		while (m_Run || m_RunOnce)
		{
			try
			{
				//Engine::instance()->
				
				for (InputMap::iterator it = m_InputSlots.begin(); it != m_InputSlots.end(); it++)
				{
					it->second->updateCurrent();
				}

				m_Service->update();
				
				for (OutputMap::iterator it = m_OutputSlots.begin(); it != m_OutputSlots.end(); it++)
				{
					it->second->update();
				}

				if (m_RunOnce)
				{
					m_RunOnce = false;
				}
				else
				{
					long elapsed = 0;//m_Timer.elapsed()/1000;
					long sleep = m_MaxDelay - elapsed;
					if (sleep > 0)
					{
						Poco::Thread::sleep(sleep);
					}
				}
			}
			catch (_2Real::Exception &e)
			{
				m_Run = false;
				m_RunOnce = false;

				m_System.handleException(*this, e);
			}
		}
	}

	void Service::update()
	{
		try
		{
			for (InputMap::iterator it = m_InputSlots.begin(); it != m_InputSlots.end(); it++)
			{
				it->second->updateCurrent();
			}

			m_Service->update();

			for (OutputMap::iterator it = m_OutputSlots.begin(); it != m_OutputSlots.end(); it++)
			{
				it->second->update();
			}
		}
		catch (_2Real::Exception &e)
		{
			m_Run = false;
			m_RunOnce = false;

			m_System.handleException(*this, e);
		}
	}

	void Service::shutdown()
	{
		m_Service->shutdown();
	}

	std::list< Identifier > Service::inputSlotIDs() const
	{
		std::list< Identifier > result;
		for (InputMap::const_iterator it = m_InputSlots.begin(); it != m_InputSlots.end(); it++)
		{
			result.push_back(it->second->identifier());
		}

		return result;
	}

	std::list< Identifier > Service::outputSlotIDs() const
	{
		std::list< Identifier > result;
		for (OutputMap::const_iterator it = m_OutputSlots.begin(); it != m_OutputSlots.end(); it++)
		{
			result.push_back(it->second->identifier());
		}

		return result;
	}

	std::list< Identifier > Service::setupParameterIDs() const
	{
		std::list< Identifier > result;
		for (ParameterMap::const_iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
		{
			result.push_back(it->second->identifier());
		}

		return result;
	}

	EngineData Service::getParameterValue(std::string const& name) const
	{
		ParameterMap::const_iterator it = m_SetupParameters.find(name);
		if (it == m_SetupParameters.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return it->second->get();
	}

	InputHandle Service::createInputHandle(std::string const& name) const
	{
		InputMap::const_iterator it = m_InputSlots.find(name);
		if (it == m_InputSlots.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		InputHandle handle(it->second);
		return handle;
	}

	OutputHandle Service::createOutputHandle(std::string const& name) const
	{
		OutputMap::const_iterator it = m_OutputSlots.find(name);
		if (it == m_OutputSlots.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		OutputHandle handle(it->second);
		return handle;
	}

	OutputSlot & Service::getOutputSlot(std::string const& name)
	{
		OutputMap::iterator it = m_OutputSlots.find(name);
		if (it == m_OutputSlots.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return *(it->second);
	}

	InputSlot & Service::getInputSlot(std::string const& name)
	{
		InputMap::iterator it = m_InputSlots.find(name);
		if (it == m_InputSlots.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return *(it->second);
	}

	SetupParameter & Service::getSetupParameter(std::string const& name)
	{
		ParameterMap::iterator it = m_SetupParameters.find(name);
		if (it == m_SetupParameters.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return *(it->second);
	}

	OutputSlot const& Service::getOutputSlot(std::string const& name) const
	{
		OutputMap::const_iterator it = m_OutputSlots.find(name);
		if (it == m_OutputSlots.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return *(it->second);
	}

	InputSlot const& Service::getInputSlot(std::string const& name) const
	{
		InputMap::const_iterator it = m_InputSlots.find(name);
		if (it == m_InputSlots.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return *(it->second);
	}

	SetupParameter const& Service::getSetupParameter(std::string const& name) const
	{
		ParameterMap::const_iterator it = m_SetupParameters.find(name);
		if (it == m_SetupParameters.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return *(it->second);
	}

}