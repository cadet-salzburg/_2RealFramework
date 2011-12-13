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
#include "_2RealHelpers.h"

#include <sstream>

namespace _2Real
{

	ServiceMetadata::ServiceMetadata(std::string const& name) :
		m_ServiceName(name),
		m_Description(""),
		m_SetupParameters(),
		m_InputSlots(),
		m_OutputSlots()
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

	void ServiceMetadata::addSetupParameter(std::string const& name, ParameterMetadata *data)
	{
		ParameterDataMap::iterator it = m_SetupParameters.find(name);

		if (it != m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "service setup parameter " << name << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		m_SetupParameters.insert(NamedParameterData(name, data));
	}

	void ServiceMetadata::addInputSlot(std::string const& name, ParameterMetadata *data)
	{
		ParameterDataMap::iterator it = m_InputSlots.find(name);

		if (it != m_InputSlots.end())
		{
			std::ostringstream msg;
			msg << "service input slot " << name << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		m_InputSlots.insert(NamedParameterData(name, data));
	}

	void ServiceMetadata::addOutputSlot(std::string const& name, ParameterMetadata *data)
	{
		ParameterDataMap::iterator it = m_OutputSlots.find(name);

		if (it != m_OutputSlots.end())
		{
			std::ostringstream msg;
			msg << "service output slot " << name << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		m_OutputSlots.insert(NamedParameterData(name, data));
	}

	std::ostream& operator<<(std::ostream &out, ServiceMetadata const& metadata)
	{
		out << metadata.getName() << std::endl;
		out << metadata.getDescription() << std::endl;
		out << "setup parameters:" << std::endl;
		out << metadata.getSetupParameters() << std::endl;
		out << "input slots:" << std::endl;
		out << metadata.getInputSlots() << std::endl;
		out << "output slots:" << std::endl;
		out << metadata.getOutputSlots() << std::endl;
		return out;
	}

}