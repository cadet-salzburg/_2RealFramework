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
#include "_2RealParameterMetadata.h"
#include "_2RealException.h"

#include <sstream>

namespace _2Real
{

	ServiceMetadata::ServiceMetadata(std::string const& name, StringMap const& allowedTypes) :
		m_ServiceName(name),
		m_Description(""),
		m_SetupParameters(),
		m_InputSlots(),
		m_OutputSlots(),
		m_AllowedTypes(allowedTypes)
	{
	}

	ServiceMetadata::~ServiceMetadata()
	{
		for (ParameterDataMap::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); ++it)
		{
			delete it->second;
		}

		for (ParameterDataMap::iterator it = m_InputSlots.begin(); it != m_InputSlots.end(); ++it)
		{
			delete it->second;
		}

		for (ParameterDataMap::iterator it = m_OutputSlots.begin(); it != m_OutputSlots.end(); ++it)
		{
			delete it->second;
		}

		m_SetupParameters.clear();
		m_InputSlots.clear();
		m_OutputSlots.clear();
	}

	void ServiceMetadata::addSetupParameter(std::string const& name, std::string const& type)
	{
		ParameterDataMap::iterator it = m_SetupParameters.find(name);

		if (it != m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "service setup parameter " << name << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		m_SetupParameters.insert(NamedParameterData(name, new ParameterMetadata(name, type)));
	}

	void ServiceMetadata::addInputSlot(std::string const& name, std::string const& type)
	{
		ParameterDataMap::iterator it = m_InputSlots.find(name);

		if (it != m_InputSlots.end())
		{
			std::ostringstream msg;
			msg << "service input slot " << name << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		m_InputSlots.insert(NamedParameterData(name, new ParameterMetadata(name, type)));
	}

	void ServiceMetadata::addOutputSlot(std::string const& name, std::string const& type)
	{
		ParameterDataMap::iterator it = m_OutputSlots.find(name);

		if (it != m_OutputSlots.end())
		{
			std::ostringstream msg;
			msg << "service output slot " << name << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		m_OutputSlots.insert(NamedParameterData(name, new ParameterMetadata(name, type)));
	}

	const StringMap ServiceMetadata::getInputParameters() const
	{
		StringMap result;
		for (ParameterDataMap::const_iterator it = m_InputSlots.begin(); it !=m_InputSlots.end(); it++)
		{
			ParameterMetadata *data = it->second;
			result.insert(std::make_pair(data->getName(), data->getType()));
		}
		return result;
	}

	const StringMap ServiceMetadata::getOutputParameters() const
	{
		StringMap result;
		for (ParameterDataMap::const_iterator it = m_OutputSlots.begin(); it !=m_OutputSlots.end(); it++)
		{
			ParameterMetadata *data = it->second;
			result.insert(std::make_pair(data->getName(), data->getType()));
		}
		return result;
	}

	const StringMap ServiceMetadata::getSetupParameters() const
	{
		StringMap result;
		for (ParameterDataMap::const_iterator it = m_SetupParameters.begin(); it !=m_SetupParameters.end(); it++)
		{
			ParameterMetadata *data = it->second;
			result.insert(std::make_pair(data->getName(), data->getType()));
		}
		return result;
	}

	const bool ServiceMetadata::containsSetupParameter(std::string const& name) const
	{
		return m_SetupParameters.find(name) != m_SetupParameters.end();
	}

	const bool ServiceMetadata::containsInputParameter(std::string const& name) const
	{
		return m_InputSlots.find(name) != m_InputSlots.end();
	}

	const bool ServiceMetadata::containsOutputParameter(std::string const& name) const
	{
		return m_OutputSlots.find(name) != m_OutputSlots.end();
	}

	const std::string ServiceMetadata::getInfoString() const
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
				info << it->first << " " << it->second->getType() << std::endl;
			}
		}

		if (!m_InputSlots.empty())
		{
			info << "this service has input parameters: " << std::endl;
			for (ParameterDataMap::const_iterator it = m_InputSlots.begin(); it != m_InputSlots.end(); it++)
			{
				info << it->first << " " << it->second->getType() << std::endl;
			}
		}

		if (!m_OutputSlots.empty())
		{
			info << "this service has output parameters: " << std::endl;
			for (ParameterDataMap::const_iterator it = m_OutputSlots.begin(); it != m_OutputSlots.end(); it++)
			{
				info << it->first << " " << it->second->getType() << std::endl;
			}
		}

		return info.str();
	}

}