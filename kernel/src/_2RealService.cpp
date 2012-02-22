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
#include "_2RealServiceMetadata.h"
#include "_2RealParameterMetadata.h"

#include <iostream>
#include <sstream>

namespace _2Real
{

	Service::Service(Identifier const& id, IService &service, SystemGraph &system, ServiceMetadata const& metadata) :
		Runnable(id, system),
		m_Service(&service),
		m_SetupParameters(),
		m_InputSlots(),
		m_OutputSlots()
	{
		std::cout << "creating service " << id.name() << std::endl;

		ParameterDataMap const& setup = metadata.getSetupParameters();
		ParameterDataMap const& input = metadata.getInputSlots();
		ParameterDataMap const& output = metadata.getOutputSlots();

		std::cout << "creating setup" << std::endl;
		for (ParameterDataMap::const_iterator it = setup.begin(); it != setup.end(); ++it)
		{
			ParameterMetadata const& meta = *it->second;
			SetupParameter *setup = new SetupParameter(meta);
			std::cout << meta.getName() << std::endl;
			m_SetupParameters.insert(NamedParameter(meta.getName(), setup));
		}

		std::cout << "creating inputd" << std::endl;
		for (ParameterDataMap::const_iterator it = input.begin(); it != input.end(); ++it)
		{
			ParameterMetadata const& meta = *it->second;
			InputSlot *input = new InputSlot(meta);
			std::cout << meta.getName() << std::endl;
			m_InputSlots.insert(NamedInput(meta.getName(), input));
		}

		std::cout << "creating outputs" << std::endl;
		for (ParameterDataMap::const_iterator it = output.begin(); it != output.end(); ++it)
		{
			ParameterMetadata const& meta = *it->second;
			OutputSlot *output = new OutputSlot(meta);
			std::cout << meta.getName() << std::endl;
			m_OutputSlots.insert(NamedOutput(meta.getName(), output));
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

	void Service::registerToNewData(std::string const& outName, DataCallback callback)
	{
		getOutputSlot(outName).registerCallback(callback);
	}

	void Service::unregisterFromNewData(std::string const& outName, DataCallback callback)
	{
		getOutputSlot(outName).unregisterCallback(callback);
	}

	void Service::registerToNewData(std::string const& outName, IOutputListener &listener)
	{
		getOutputSlot(outName).addListener(listener);
	}

	void Service::unregisterFromNewData(std::string const& outName, IOutputListener &listener)
	{
		getOutputSlot(outName).addListener(listener);
	}

	void Service::linkWith(std::string const& inName, Service &serviceOut, std::string const& outName)
	{
		InputSlot &in = getInputSlot(inName);
		OutputSlot &out = serviceOut.getOutputSlot(outName);

		in.linkWith(out);
	}

	void Service::setup()
	{
		ServiceContext context(*this);
		m_Service->setup(context);
	}

	void Service::run()
	{
		try
		{
			bool ready = true;
			for (InputMap::iterator it = m_InputSlots.begin(); it != m_InputSlots.end(); it++)
			{
				ready &= it->second->updateCurrent();
			}

			if (ready)
			{
				m_Service->update();
				for (OutputMap::iterator it = m_OutputSlots.begin(); it != m_OutputSlots.end(); it++)
				{
					it->second->update();
				}
			}
		}
		catch (_2Real::Exception &e)
		{
			m_System.handleException(*this, e);
		}
	}

	void Service::shutdown()
	{
		m_Service->shutdown();
	}

	void Service::performStartCheck() const
	{
		bool success = true;
		std::ostringstream msg;
		for (ParameterMap::const_iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); ++it)
		{
			if (!it->second->isInitialized())
			{
				msg << "setup parameter " << it->second->getName() << " of service " << name() << " has not been set\n";
				success = false;
			}
		}

		//currently, i demand that all input slots of a service are either set to sth or linked before starting
		for (InputMap::const_iterator it = m_InputSlots.begin(); it != m_InputSlots.end(); ++it)
		{
			if (!it->second->isInitialized() && !it->second->isLinked())
			{
				msg << "input slot " << it->second->getName() << " of service " << name() << " has been neither set nor linked\n";
				success = false;
			}
		}

		if (!success)
		{
			throw StartException(msg.str());
		}
	}

	void Service::setParameterValue(std::string const& name, Data const& data)
	{
		if (hasSetupParameter(name))
		{
			getSetupParameter(name).setData(data.data());
		}
		else if (hasInputSlot(name))
		{
			getInputSlot(name).setData(TimestampedData(data.getTimestamp(), data.data()));
		}
		else
		{
			std::ostringstream msg;
			msg << "setup parameter / input slot " << name << " not found in " << this->name() << std::endl;
			throw NotFoundException(msg.str());
		}
	}

	const EngineData Service::getParameterValue(std::string const& name) const
	{
		if (hasSetupParameter(name))
		{
			return getSetupParameter(name).getData();
		}
		else if (hasInputSlot(name))
		{
			return getInputSlot(name).getOldest().second;
		}
		else
		{
			std::ostringstream msg;
			msg << "setup parameter / input slot " << name << " not found in " << this->name() << std::endl;
			throw NotFoundException(msg.str());
		}
	}

	std::string const& Service::getParameterKey(std::string const& name) const
	{
		if (hasSetupParameter(name))
		{
			return getSetupParameter(name).getKeyword();
		}
		else if (hasInputSlot(name))
		{
			return getInputSlot(name).getKeyword();
		}
		else
		{
			std::ostringstream msg;
			msg << "setup parameter / input slot " << name << " not found in " << this->name() << std::endl;
			throw NotFoundException(msg.str());
		}
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
			msg << "service input slot " << name << " not found in " << this->name() << std::endl;
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
			msg << "service setup parameter " << name << " not found in " << this->name() << std::endl;
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
			msg << "service input slot " << name << " not found in " << this->name() << std::endl;
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
			msg << "service setup parameter " << name << " not found in " << this->name() << std::endl;
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

}