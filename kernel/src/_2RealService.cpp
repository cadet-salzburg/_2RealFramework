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
#include "_2RealEngine.h"

#include <sstream>

namespace _2Real
{

	Service::Service(Identifier const& id, IService &service, SystemGraph &system, StringMap const& setup, StringMap const& input, StringMap const& output) : 
		Runnable(id, system),
		m_Service(&service),
		m_MaxDelay(long(1000.0f/30.0f)),
		m_UpdatesPerSecond(30.0f),
		m_SetupParameters(),
		m_InputSlots(),
		m_OutputSlots()
	{
		StringMap const& allowedTypes = Engine::instance().getAllowedTypes();
		Typetable const& lookupTable = Engine::instance().types();

		for (StringMap::const_iterator it = setup.begin(); it != setup.end(); ++it)
		{
			std::string name = it->first;
			std::string keyword = it->second;
			std::string type =  allowedTypes.find(keyword)->second;
			SetupParameter *setup = new SetupParameter(name, type, keyword);
			m_SetupParameters.insert(NamedParameter(name, setup));
		}

		for (StringMap::const_iterator it = input.begin(); it != input.end(); ++it)
		{
			std::string name = it->first;
			std::string keyword = it->second;
			std::string type =  allowedTypes.find(keyword)->second;
			InputSlot *in = new InputSlot(*this, name, type, keyword);
			m_InputSlots.insert(NamedInput(name, in));
		}

		for (StringMap::const_iterator it = output.begin(); it != output.end(); ++it)
		{
			std::string name = it->first;
			std::string keyword = it->second;
			std::string type =  allowedTypes.find(keyword)->second;
			EngineData initialData;
			lookupTable.createEngineData(keyword, initialData);
			OutputSlot *out = new OutputSlot(*this, name, type, keyword, initialData);
			m_OutputSlots.insert(NamedOutput(name, out));
		}
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

	void Service::setUpdateRate(float updatesPerSecond)
	{
		m_MaxDelay = long(1000.0f/updatesPerSecond);
		m_UpdatesPerSecond = updatesPerSecond;
	}

	void Service::checkConfiguration()
	{
		//TODO
	}

	void Service::setup()
	{
		ServiceContext context(*this);
		m_Service->setup(context);
	}

	void Service::run()
	{
		while (m_Run || m_RunOnce)
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

	void Service::setParameterValue(std::string const& name, EngineData const& data)
	{
		getSetupParameter(name).setData(data);
	}

	EngineData Service::getParameterValue(std::string const& name) const
	{
		return getSetupParameter(name).getData();
	}

	InputHandle Service::createInputHandle(std::string const& name) const
	{
		return InputHandle(getInputSlot(name));
	}

	OutputHandle Service::createOutputHandle(std::string const& name)
	{
		return OutputHandle(getOutputSlot(name));
	}

	OutputSlot & Service::getOutputSlot(std::string const& name)
	{
		OutputMap::iterator it = m_OutputSlots.find(name);
		if (it == m_OutputSlots.end())
		{
			std::ostringstream msg;
			msg << "service output slot " << name << " not found in " << this->name() << std::endl;
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	InputSlot & Service::getInputSlot(std::string const& name)
	{
		InputMap::iterator it = m_InputSlots.find(name);
		if (it == m_InputSlots.end())
		{
			std::ostringstream msg;
			msg << "service output slot " << name << " not found in " << this->name() << std::endl;
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	SetupParameter & Service::getSetupParameter(std::string const& name)
	{
		ParameterMap::iterator it = m_SetupParameters.find(name);
		if (it == m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "service output slot " << name << " not found in " << this->name() << std::endl;
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	OutputSlot const& Service::getOutputSlot(std::string const& name) const
	{
		OutputMap::const_iterator it = m_OutputSlots.find(name);
		if (it == m_OutputSlots.end())
		{
			std::ostringstream msg;
			msg << "service output slot " << name << " not found in " << this->name() << std::endl;
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	InputSlot const& Service::getInputSlot(std::string const& name) const
	{
		InputMap::const_iterator it = m_InputSlots.find(name);
		if (it == m_InputSlots.end())
		{
			std::ostringstream msg;
			msg << "service output slot " << name << " not found in " << this->name() << std::endl;
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	SetupParameter const& Service::getSetupParameter(std::string const& name) const
	{
		ParameterMap::const_iterator it = m_SetupParameters.find(name);
		if (it == m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "service output slot " << name << " not found in " << this->name() << std::endl;
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

}