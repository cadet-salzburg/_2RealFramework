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

#include "_2RealPluginMetadata.h"
#include "_2RealServiceMetadata.h"
#include "_2RealParameterMetadata.h"
#include "_2RealException.h"

#include <sstream>

namespace _2Real
{

	PluginMetadata::PluginMetadata(std::string const& classname, std::string const& path, StringMap const& allowedTypes) :
		m_Classname(classname),
		m_InstallDirectory(path),
		m_Description("this plugin is holy"),
		m_Author("the spaghetti monster"),
		m_Contact("through prayer"),
		m_Version(0, 0, 0),
		m_Services(),
		m_SetupParameters(),
		m_AllowedTypes(allowedTypes)
	{
	}

	PluginMetadata::~PluginMetadata()
	{
		for (ServiceDataMap::iterator it = m_Services.begin(); it != m_Services.end(); it++)
		{
			delete it->second;
		}

		m_Services.clear();
		m_SetupParameters.clear();
	}

	void PluginMetadata::checkKeyword(std::string const& keyword) const
	{
		if (m_AllowedTypes.find(keyword) == m_AllowedTypes.end())
		{
			std::ostringstream msg;
			msg << "keyword: " << keyword << " is not among accepted parameter keywords";
			throw XMLFormatException(msg.str());
		}
	}

	const std::string PluginMetadata::lookupKeyword(std::string const& type) const
	{
		std::string keyword = "";
		for (StringMap::const_iterator it = m_AllowedTypes.begin(); it != m_AllowedTypes.end(); it++)
		{
			if ((it->second) == type)
			{
				keyword = it->first;
				break;
			}
		}

		if (keyword == "")
		{
			std::ostringstream msg;
			msg << "type " << type << " is not accepted as parameter type";
			throw InvalidTypeException(msg.str());
		}

		return keyword;
	}

	void PluginMetadata::addServiceMetadata(std::string const& name)
	{
		ServiceDataMap::iterator it = m_Services.find(name);

		if (it != m_Services.end())
		{
			std::ostringstream msg;
			msg << "service " << name << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		ServiceMetadata *metadata = new ServiceMetadata(name, m_AllowedTypes);
		m_Services.insert(NamedServiceData(name, metadata));
	}

	void PluginMetadata::setDescription(std::string const& service, std::string const& description)
	{
		getServiceMetadata(service).setDescription(description);
	}

	void PluginMetadata::addSetupParameterByKey(std::string const& setupName, std::string const& keyword)
	{
		ParameterDataMap::iterator it = m_SetupParameters.find(setupName);

		if (it != m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "plugin setup parameter " << setupName << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		m_SetupParameters.insert(NamedParameterData(setupName, new ParameterMetadata(setupName, keyword)));
	}

	void PluginMetadata::addSetupParameterByKey(std::string const& serviceName, std::string const& setupName, std::string const& keyword)
	{
		getServiceMetadata(serviceName).addSetupParameter(setupName, keyword);
	}

	void PluginMetadata::addInputSlotByKey(std::string const& serviceName, std::string const& inputName, std::string const& keyword)
	{
		getServiceMetadata(serviceName).addInputSlot(inputName, keyword);
	}

	void PluginMetadata::addOutputSlotByKey(std::string const& serviceName, std::string const& outputName, std::string const& keyword)
	{
		getServiceMetadata(serviceName).addOutputSlot(outputName, keyword);
	}

	void PluginMetadata::addSetupParameterByType(std::string const& setupName, std::string const& type)
	{
		const std::string keyword = lookupKeyword(type);
		addSetupParameterByKey(setupName, keyword);
	}

	void PluginMetadata::addSetupParameterByType(std::string const& serviceName, std::string const& setupName, std::string const& type)
	{
		const std::string keyword = lookupKeyword(type);
		getServiceMetadata(serviceName).addSetupParameter(setupName, keyword);
	}

	void PluginMetadata::addInputSlotByType(std::string const& serviceName, std::string const& inputName, std::string const& type)
	{
		const std::string keyword = lookupKeyword(type);
		getServiceMetadata(serviceName).addInputSlot(inputName, keyword);
	}

	void PluginMetadata::addOutputSlotByType(std::string const& serviceName, std::string const& outputName, std::string const& type)
	{
		const std::string keyword = lookupKeyword(type);
		getServiceMetadata(serviceName).addOutputSlot(outputName, keyword);
	}

	const StringMap PluginMetadata::getSetupParameters() const
	{
		StringMap result;
		for (ParameterDataMap::const_iterator it = m_SetupParameters.begin(); it !=m_SetupParameters.end(); it++)
		{
			ParameterMetadata *data = it->second;
			result.insert(std::make_pair(data->getName(), data->getType()));
		}
		return result;
	}

	const StringMap PluginMetadata::getSetupParameters(std::string const& serviceName) const
	{
		return getServiceMetadata(serviceName).getSetupParameters();
	}

	const StringMap PluginMetadata::getOutputSlots(std::string const& serviceName) const
	{
		return getServiceMetadata(serviceName).getOutputParameters();
	}

	const StringMap PluginMetadata::getInputSlots(std::string const& serviceName) const
	{
		return getServiceMetadata(serviceName).getInputParameters();
	}

	bool PluginMetadata::containsParameterMetadata(std::string const& name) const
	{
		return m_SetupParameters.find(name) != m_SetupParameters.end();
	}

	bool PluginMetadata::containsServiceMetadata(std::string const& name) const
	{
		return m_Services.find(name) != m_Services.end();
	}

	ParameterMetadata & PluginMetadata::getParameterMetadata(std::string const& name)
	{
		ParameterDataMap::const_iterator it = m_SetupParameters.find(name);

		if (it == m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "plugin setup parameter " << name << " not found";
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	ServiceMetadata & PluginMetadata::getServiceMetadata(std::string const& name)
	{
		ServiceDataMap::const_iterator it = m_Services.find(name);

		if (it == m_Services.end())
		{
			std::ostringstream msg;
			msg << "service " << name << " not found";
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	ParameterMetadata const& PluginMetadata::getParameterMetadata(std::string const& name) const
	{
		ParameterDataMap::const_iterator it = m_SetupParameters.find(name);

		if (it == m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "plugin setup parameter " << name << " not found";
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	ServiceMetadata const& PluginMetadata::getServiceMetadata(std::string const& name) const
	{
		ServiceDataMap::const_iterator it = m_Services.find(name);

		if (it == m_Services.end())
		{
			std::ostringstream msg;
			msg << "service " << name << " not found";
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	const std::string PluginMetadata::getInfoString() const
	{
		std::ostringstream info;
		info << std::endl;
		info << "plugin:\t" << m_Classname << std::endl;
		info << "description:\t" << m_Description << std::endl;
		info << "installed at\t" << m_InstallDirectory << std::endl;
		info << "written by\t" << m_Author << std::endl;
		info << "contact\t\t" << m_Contact << std::endl;
		info << "version\t\t" << m_Version.asString() << std::endl;

		if (!m_SetupParameters.empty())
		{
			info << "this plugin has setup parameters:\t" << std::endl;
			for (ParameterDataMap::const_iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
			{
				info << it->first << ":\t" << it->second->getType() << std::endl;
			}
		}

		info << "this plugin exports the following services:" << std::endl;
		for (ServiceDataMap::const_iterator it = m_Services.begin(); it != m_Services.end(); it++)
		{
			info << "service\t" << it->first << std::endl;
			info << it->second->getInfoString();
		}

		return info.str();
	}

}