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
#include "_2RealSharedAny.h"
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"
#include "_2RealIService.h"
#include "_2RealSystemGraph.h"

#include <iostream>

namespace _2Real
{

	Service::Service(IService *const _service, Identifier const& _id, SystemGraph *const _system) : 
		Runnable(_id, _system),
		m_Service(_service),
		m_Timer(),
		m_MaxDelay(16665)
	{
	}

	Service::~Service()
	{
		delete m_Service;

		for (InputMap::iterator it = m_InputSlots.begin(); it != m_InputSlots.end(); it++)
		{
			std::string name;
			try
			{
				name = it->second->name();
				std::cout << "deleting input slot: " << name << std::endl;
				delete it->second;
			}
			catch (...)
			{
				std::cout << "error on input slot destruction: " << name << std::endl;
			}
		}

		for (OutputMap::iterator it = m_OutputSlots.begin(); it != m_OutputSlots.end(); it++)
		{
			std::string name;
			try
			{
				name = it->second->name();
				std::cout << "deleting output slot: " << name << std::endl;
				delete it->second;
			}
			catch (...)
			{
				std::cout << "error on output slot destruction: " << name << std::endl;
			}
		}

		for (ParameterMap::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
		{
			std::string name;
			try
			{
				name = it->second->name();
				std::cout << "deleting setup parameter: " << name << std::endl;
				delete it->second;
			}
			catch (...)
			{
				std::cout << "error on setup parameter destruction: " << name << std::endl;
			}
		}
	}

	void Service::addInputSlot(InputSlot *const _slot)
	{
		m_InputSlots.insert(NamedInput(_slot->name(), _slot));
	}

	void Service::addOutputSlot(OutputSlot *const _slot)
	{
		m_OutputSlots.insert(NamedOutput(_slot->name(), _slot));
	}

	void Service::addSetupParameter(SetupParameter *const _parameter)
	{
		m_SetupParameters.insert(NamedParameter(_parameter->name(), _parameter));
	}

	void Service::checkConfiguration()
	{
		ServiceContext context(this);
		m_Service->setup(context);

			//for (InputMap::iterator it = m_InputSlots.begin(); it != m_InputParameters.end(); it++)
			//{
			//	if (!it->second->isLinked())
			//	{
			//		throw Exception("service setup error: input slot " + it->first + " is not linked");
			//	}
			//	if (!it->second->isInitialized())
			//	{
			//		throw Exception("service setup error: input slot " + it->first + " was not initialized by the service");
			//	}
			//}

			//for (OutputMap::iterator it = m_OutputSlots.begin(); it != m_OutputParameters.end(); it++)
			//{
			//	if (!it->second->isInitialized())
			//	{
			//		throw Exception("service setup error: output slot " + it->first + " was not initialized by the service");
			//	}
			//}

			//for (ParameterMap::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
			//{
			//	if (!it->second->isInitialized())
			//	{
			//		throw Exception("service setup error: value for setup parameter " + it->first + " was not provided");
			//	}
			//}

		//m_bIsConfigured = true;
	}

	void Service::run()
	{
		//if (!m_IsConfigured)
		//{
		//	throw Exception("attempted to update incorrectly configured service " + name());
		//}

		while (m_Run || m_RunOnce)
		{
			try
			{
				m_Timer.update();
				
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

					long elapsed = m_Timer.elapsed();

					long sleep = (m_MaxDelay - elapsed)/1000;
					//std::cout << name() << " " << sleep << " " << elapsed << std::endl;

					if (sleep > 0)
					{
						Poco::Thread::sleep(sleep);
					}

				}
			}
			catch (ServiceException &e)
			{
				m_Run = false;
				m_RunOnce = false;

				m_System->handleException(*this, e);
			}
			catch (Exception &e)
			{
				m_Run = false;
				m_RunOnce = false;

				m_System->handleException(*this, e);
			}
		}
	}

	void Service::update()
	{
		//if (!m_IsConfigured)
		//{
		//	throw Exception("attempted to update incorrectly configured service " + name());
		//}

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
		catch (ServiceException &e)
		{
			m_Run = false;
			m_RunOnce = false;

			m_System->handleException(*this, e);
		}
		catch (Exception &e)
		{
			m_Run = false;
			m_RunOnce = false;

			m_System->handleException(*this, e);
		}
	}

	void Service::shutdown()
	{
		m_Service->shutdown();
	}

	std::list< unsigned int > Service::inputSlotIDs() const
	{
		std::list< unsigned int > result;
		for (InputMap::const_iterator it = m_InputSlots.begin(); it != m_InputSlots.end(); it++)
		{
			result.push_back(it->second->id());
		}

		return result;
	}

	std::list< unsigned int > Service::outputSlotIDs() const
	{
		std::list< unsigned int > result;
		for (OutputMap::const_iterator it = m_OutputSlots.begin(); it != m_OutputSlots.end(); it++)
		{
			result.push_back(it->second->id());
		}

		return result;
	}

	std::list< unsigned int > Service::setupParameterIDs() const
	{
		std::list< unsigned int > result;
		for (ParameterMap::const_iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
		{
			result.push_back(it->second->id());
		}

		return result;
	}

	SharedAny Service::getParameterValue(std::string const& _name)
	{
		ParameterMap::iterator it = m_SetupParameters.find(_name);
		if (it == m_SetupParameters.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return it->second->get();
	}

	InputHandle Service::createInputHandle(std::string const& _name)
	{
		InputMap::iterator it = m_InputSlots.find(_name);
		if (it == m_InputSlots.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		InputHandle handle(it->second);
		return handle;
	}

	OutputHandle Service::createOutputHandle(std::string const& _name)
	{
		OutputMap::iterator it = m_OutputSlots.find(_name);
		if (it == m_OutputSlots.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		OutputHandle handle(it->second);
		return handle;
	}

	OutputSlot *const Service::getOutputSlot(std::string const& _name)
	{
		OutputMap::iterator it = m_OutputSlots.find(_name);
		if (it == m_OutputSlots.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return it->second;
	}

	InputSlot *const Service::getInputSlot(std::string const& _name)
	{
		InputMap::iterator it = m_InputSlots.find(_name);
		if (it == m_InputSlots.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return it->second;
	}

	SetupParameter *const Service::getSetupParameter(std::string const& _name)
	{
		ParameterMap::iterator it = m_SetupParameters.find(_name);
		if (it == m_SetupParameters.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		return it->second;
	}

}