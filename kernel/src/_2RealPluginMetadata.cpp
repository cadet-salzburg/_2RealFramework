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
#include "_2RealEngine.h"
#include "_2RealEngineData.h"

#include <sstream>

namespace _2Real
{

	PluginMetadata::PluginMetadata(std::string const& classname, std::string const& path) :
		m_Classname(classname),
		m_InstallDirectory(path),
		m_Description("this plugin is holy"),
		m_Author("the spaghetti monster"),
		m_Contact("through prayer"),
		m_Version(0, 0, 0),
		m_Services(),
		m_SetupParameters(),
		m_Typetable(Engine::instance().types())
	{
	}

	PluginMetadata::~PluginMetadata()
	{
	}

	void PluginMetadata::clear()
	{
		for (ServiceDataMap::iterator it = m_Services.begin(); it != m_Services.end(); it++)
		{
			delete it->second;
		}
		for (ParameterDataMap::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
		{
			delete it->second;
		}
		m_Services.clear();
		m_SetupParameters.clear();

		m_Test.assign(NULL);
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

		ServiceMetadata *metadata = new ServiceMetadata(name);
		m_Services.insert(NamedServiceData(name, metadata));
	}

	void PluginMetadata::setDescription(std::string const& service, std::string const& description)
	{
		getServiceMetadata(service).setDescription(description);
	}

	ParameterMetadata * PluginMetadata::createParameterFromKey(std::string const& name, std::string const& keyword)
	{
		const std::string type = m_Typetable.lookupType(keyword);
		ParameterMetadata *data = new ParameterMetadata(name, type, keyword);
		return data;
	}

	ParameterMetadata * PluginMetadata::createParameterFromType(std::string const& name, std::string const& type)
	{
		const std::string keyword = m_Typetable.lookupKey(type);
		ParameterMetadata *data = new ParameterMetadata(name, type, keyword);
		return data;
	}

	ParameterMetadata * PluginMetadata::createParameterFromData(std::string const& name, EngineData defaultValue)
	{
		const std::string type = defaultValue.typeinfo().name();
		const std::string keyword = m_Typetable.lookupKey(type);
		ParameterMetadata *data = new ParameterMetadata(name, type, keyword);
		data->setDefaultValue(defaultValue);
		return data;
	}

	void PluginMetadata::addSetupParameter(std::string const& setupName, ParameterMetadata *data)
	{
		ParameterDataMap::iterator it = m_SetupParameters.find(setupName);

		if (it != m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "plugin setup parameter " << setupName << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		m_SetupParameters.insert(NamedParameterData(setupName, data));
	}

	void PluginMetadata::addSetupParameterByKey(std::string const& setupName, std::string const& keyword)
	{
		ParameterMetadata *data = createParameterFromKey(setupName, keyword);
		addSetupParameter(setupName, data);
	}

	void PluginMetadata::addSetupParameterByKey(std::string const& serviceName, std::string const& setupName, std::string const& keyword)
	{
		ParameterMetadata *data = createParameterFromKey(setupName, keyword);
		getServiceMetadata(serviceName).addSetupParameter(setupName, data);
	}

	void PluginMetadata::addInputSlotByKey(std::string const& serviceName, std::string const& inputName, std::string const& keyword)
	{
		ParameterMetadata *data = createParameterFromKey(inputName, keyword);
		getServiceMetadata(serviceName).addInputSlot(inputName, data);
	}

	void PluginMetadata::addOutputSlotByKey(std::string const& serviceName, std::string const& outputName, std::string const& keyword)
	{
		ParameterMetadata *data = createParameterFromKey(outputName, keyword);
		getServiceMetadata(serviceName).addOutputSlot(outputName, data);
	}

	void PluginMetadata::addSetupParameterByType(std::string const& setupName, std::string const& type)
	{
		ParameterMetadata *data = createParameterFromType(setupName, type);
		addSetupParameter(setupName, data);
	}

	void PluginMetadata::addSetupParameterByType(std::string const& serviceName, std::string const& setupName, std::string const& type)
	{
		ParameterMetadata *data = createParameterFromType(setupName, type);
		getServiceMetadata(serviceName).addSetupParameter(setupName, data);
	}

	void PluginMetadata::addInputSlotByType(std::string const& serviceName, std::string const& inputName, std::string const& type)
	{
		ParameterMetadata *data = createParameterFromType(inputName, type);
		getServiceMetadata(serviceName).addInputSlot(inputName, data);
	}

	void PluginMetadata::addOutputSlotByType(std::string const& serviceName, std::string const& outputName, std::string const& type)
	{
		ParameterMetadata *data = createParameterFromType(outputName, type);
		getServiceMetadata(serviceName).addOutputSlot(outputName, data);
	}

	void PluginMetadata::addSetupParameterByData(std::string const& setupName, EngineData const& defaultValue)
	{
		ParameterMetadata *data = createParameterFromData(setupName, defaultValue);
		addSetupParameter(setupName, data);
	}

	void PluginMetadata::addSetupParameterByData(std::string const& serviceName, std::string const& setupName, EngineData const& defaultValue)
	{
		ParameterMetadata *data = createParameterFromData(setupName, defaultValue);
		getServiceMetadata(serviceName).addSetupParameter(setupName, data);
	}

	void PluginMetadata::addInputSlotByData(std::string const& serviceName, std::string const& inputName, EngineData const& defaultValue)
	{
		ParameterMetadata *data = createParameterFromData(inputName, defaultValue);
		getServiceMetadata(serviceName).addInputSlot(inputName, data);
	}

	bool PluginMetadata::containsParameterMetadata(std::string const& name) const
	{
		return m_SetupParameters.find(name) != m_SetupParameters.end();
	}

	bool PluginMetadata::containsServiceMetadata(std::string const& name) const
	{
		return m_Services.find(name) != m_Services.end();
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

	ServiceMetadata & PluginMetadata::getServiceMetadata(std::string const& name)
	{
		ServiceDataMap::iterator it = m_Services.find(name);

		if (it == m_Services.end())
		{
			std::ostringstream msg;
			msg << "service " << name << " not found";
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	std::ostream& operator<<(std::ostream &out, PluginMetadata const& metadata)
	{
		out << metadata.getClassname() << std::endl;
		out << "description:\t" << metadata.getDescription() << std::endl;
		out << "installed at\t" << metadata.getInstallDirectory() << std::endl;
		out << "written by\t" << metadata.getAuthor() << std::endl;
		out << "contact\t\t" << metadata.getContact() << std::endl;
		out << "version\t\t" << metadata.getVersion() << std::endl;
		out << "setup parameters:" << std::endl;
		out << metadata.getSetupParameters() << std::endl;
		out << "available services:" << std::endl;
		out << metadata.getServices() << std::endl;
		return out;
	}

}