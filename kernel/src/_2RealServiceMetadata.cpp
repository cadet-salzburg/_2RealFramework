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

#include "_2RealServiceMetadata.h"
#include "_2RealPluginMetadata.h"
#include "_2RealException.h"

#include <sstream>
#include <iostream>

namespace _2Real
{

	ServiceMetadata::ServiceMetadata(std::string const& name, StringMap const& types) :
		m_ServiceName(name),
		m_Description(""),
		m_SetupParameters(),
		m_InputParameters(),
		m_OutputParameters(),
		m_AllowedTypes(types)
	{
	}

	void ServiceMetadata::addSetupParameterByType(std::string const& name, std::string const& type)
	{
	}

	void ServiceMetadata::addInputSlotByType(std::string const& name, std::string const& type)
	{
	}

	void ServiceMetadata::addOutputSlotByType(std::string const& name, std::string const& type)
	{
	}

	void ServiceMetadata::setDescription(std::string const& description)
	{
		m_Description = description;
	}

	void ServiceMetadata::addSetupParameter(std::string const& _name, std::string const& _type)
	{
		ParameterDataMap::iterator it = m_SetupParameters.find(_name);

		if (it != m_SetupParameters.end())
		{
			throw Exception("setup parameter " + _name + " already exists");
		}

		m_SetupParameters.insert(NamedParameterData(_name, ParameterMetadata(_name, _type)));
	}

	void ServiceMetadata::addInputSlot(std::string const& _name, std::string const& _type)
	{
		ParameterDataMap::iterator it = m_InputParameters.find(_name);

		if (it != m_InputParameters.end())
		{
			throw Exception("input slot " + _name + " already exists");
		}

		m_InputParameters.insert(NamedParameterData(_name, ParameterMetadata(_name, _type)));
	}

	void ServiceMetadata::addOutputSlot(std::string const& _name, std::string const& _type)
	{
		ParameterDataMap::iterator it = m_OutputParameters.find(_name);

		if (it != m_OutputParameters.end())
		{
			throw Exception("output slot " + _name + " already exists");
		}

		m_OutputParameters.insert(NamedParameterData(_name, ParameterMetadata(_name, _type)));
	}

	const StringMap ServiceMetadata::getInputParameters() const
	{
		StringMap result;
		for (ParameterDataMap::const_iterator it = m_InputParameters.begin(); it !=m_InputParameters.end(); it++)
		{
			ParameterMetadata data = it->second;
			result.insert(std::make_pair(data.getName(), data.getType()));
		}
		return result;
	}

	const StringMap ServiceMetadata::getOutputParameters() const
	{
		StringMap result;
		for (ParameterDataMap::const_iterator it = m_OutputParameters.begin(); it !=m_OutputParameters.end(); it++)
		{
			ParameterMetadata data = it->second;
			result.insert(std::make_pair(data.getName(), data.getType()));
		}
		return result;
	}

	const StringMap ServiceMetadata::getSetupParameters() const
	{
		StringMap result;
		for (ParameterDataMap::const_iterator it = m_SetupParameters.begin(); it !=m_SetupParameters.end(); it++)
		{
			ParameterMetadata data = it->second;
			result.insert(std::make_pair(data.getName(), data.getType()));
		}
		return result;
	}

	std::string const& ServiceMetadata::getName() const
	{
		return m_ServiceName;
	}

	std::string const& ServiceMetadata::getDescription() const
	{
		return m_Description;
	}

	const bool ServiceMetadata::hasSetupParameter(std::string const& _name) const
	{
		ParameterDataMap::const_iterator it = m_SetupParameters.find(_name);

		if (it != m_SetupParameters.end())
		{
			return false;
		}

		return true;
	}

	const bool ServiceMetadata::hasInputParameter(std::string const& _name) const
	{
		ParameterDataMap::const_iterator it = m_InputParameters.find(_name);

		if (it != m_InputParameters.end())
		{
			return false;
		}

		return true;
	}

	const bool ServiceMetadata::hasOuputParameter(std::string const& _name) const
	{
		ParameterDataMap::const_iterator it = m_OutputParameters.find(_name);

		if (it != m_OutputParameters.end())
		{
			return false;
		}

		return true;
	}

	const std::string ServiceMetadata::info() const
	{
		std::stringstream info;
		info << std::endl;
		info << "service:\t" << m_ServiceName << std::endl;
		info << "description:\t" << m_Description << std::endl;

		if (!m_SetupParameters.empty())
		{
			info << "this service has setup parameters: " << std::endl;
			for (ParameterDataMap::const_iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
			{
				info << it->first << " " << it->second.getType() << std::endl;
			}
		}

		if (!m_InputParameters.empty())
		{
			info << "this service has input parameters: " << std::endl;
			for (ParameterDataMap::const_iterator it = m_InputParameters.begin(); it != m_InputParameters.end(); it++)
			{
				info << it->first << " " << it->second.getType() << std::endl;
			}
		}

		if (!m_OutputParameters.empty())
		{
			info << "this service has output parameters: " << std::endl;
			for (ParameterDataMap::const_iterator it = m_OutputParameters.begin(); it != m_OutputParameters.end(); it++)
			{
				info << it->first << " " << it->second.getType() << std::endl;
			}
		}

		return info.str();
	}

}