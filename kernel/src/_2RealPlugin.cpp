
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

#include "_2RealPlugin.h"
#include "_2RealPluginContext.h"
#include "_2RealMetadata.h"
#include "_2RealException.h"
#include "_2RealSetupParameter.h"
#include "_2RealService.h"
#include "_2RealParameterMetadata.h"
#include "_2RealData.h"

#include <sstream>

namespace _2Real
{

	Plugin::Plugin(Identifier const& id, IPluginActivator &activator, PluginMetadata &metadata) :
		Entity(id),
		m_ServiceTemplates(),
		m_SetupParameters(),
		m_Services(),
		m_Activator(activator),
		m_Metadata(metadata),
		m_IsSetUp(false)
	{
			//build setup params
			ParameterDataMap const& setup = m_Metadata.getSetupParameters();
			for (ParameterDataMap::const_iterator it = setup.begin(); it != setup.end(); ++it)
			{
				SetupParameter *setup = new SetupParameter(*it->second);
				m_SetupParameters.insert(NamedParameter(it->second->getName(), setup));
			}
	}

	Plugin::~Plugin()
	{
		for (ParameterMap::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); /**/)
		{
			delete it->second;
			it = m_SetupParameters.erase(it);
		}

		for (TemplateMap::iterator it = m_ServiceTemplates.begin(); it != m_ServiceTemplates.end(); /**/)
		{
			delete it->second;
			it = m_ServiceTemplates.erase(it);
		}

		m_Services.clear();
	}

	std::string const& Plugin::getClassName() const
	{
		return m_Metadata.getClassname();
	}

	std::string const& Plugin::getLibraryPath() const
	{
		return m_Metadata.getInstallDirectory();
	}

	IPluginActivator & Plugin::getActivator()
	{
		return m_Activator;
	}

	void Plugin::shutDown()
	{
		m_Activator.shutdown();
	}

	bool Plugin::isSetUp() const
	{
		return m_IsSetUp;
	}

	void Plugin::registerService(std::string const& name, AbstractServiceObject *obj)
	{
		if (definesService(name))
		{
			TemplateMap::iterator it = m_ServiceTemplates.find(name);
			if (it != m_ServiceTemplates.end())
			{
				std::ostringstream msg;
				msg << "service " << name << " already exists";
				throw AlreadyExistsException(msg.str());
			}

			m_ServiceTemplates.insert(NamedTemplate(name, obj));
		}
		else
		{
			std::ostringstream msg;
			msg << "service " << name << " not found in plugin metadata";
			throw NotFoundException(msg.str());
		}
	}

	void Plugin::setParameterValue(std::string const& paramName, EngineData const& data)
	{
		getSetupParameter(paramName).setData(data);
	}

	std::string const& Plugin::getParameterKey(std::string const& paramName) const
	{
		return getSetupParameter(paramName).getKeyword();
	}

	EngineData const& Plugin::getParameterValue(std::string const& paramName) const
	{
		return getSetupParameter(paramName).getData();
	}

	const std::string Plugin::getInfoString() const
	{
		std::ostringstream s;
		s << m_Metadata;
		return s.str();
	}

	Poco::SharedPtr< IService > Plugin::createService(std::string const& serviceName) const
	{
		TemplateMap::const_iterator it = m_ServiceTemplates.find(serviceName);
		if (it == m_ServiceTemplates.end())
		{
			std::ostringstream msg;
			msg << "internal error: plugin " << this->name() << " does not export service " << serviceName;
			throw Exception(msg.str());
		}

		return Poco::SharedPtr< IService >(it->second->create());
	}

	Runnable & Plugin::createService(std::string const& serviceName, SystemImpl &graph)
	{
		unsigned int counter = 0;
		//this could be done more efficiently, currently i just count the instances of a particular plugin
		for (ServiceMap::iterator it = m_Services.begin(); it != m_Services.end(); ++it)
		{
			Service *s = it->second;
			if (s->getServiceName() == serviceName)
			{
				counter++;
			}
		}

		std::ostringstream s;
		s << counter;

		std::string idName = this->name() + "." + toLower(serviceName) + " nr. " + s.str();
		return createService(idName, serviceName, graph);
	}

	Runnable & Plugin::createService(std::string const& idName, std::string const& serviceName, SystemImpl &graph)
	{
		Poco::SharedPtr< IService > service = createService(serviceName);
		const Identifier id = Entity::createIdentifier(idName, "service");
		Service *runnable = new Service(id, service, graph, m_Metadata.getServiceMetadata(serviceName));
		m_Services.insert(NamedService(id, runnable));
		return *runnable;
	}

	const bool Plugin::definesService(std::string const& serviceName) const
	{
		return m_Metadata.containsServiceMetadata(serviceName);
	}

	const bool Plugin::exportsService(std::string const& serviceName) const
	{
		return m_ServiceTemplates.find(serviceName) != m_ServiceTemplates.end();
	}

	void Plugin::setup()
	{
		PluginContext context(*this);
		m_Activator.setup(context);
		m_IsSetUp = true;
	}

	SetupParameter & Plugin::getSetupParameter(std::string const& name)
	{
		ParameterMap::iterator it = m_SetupParameters.find(name);
		if (it == m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "plugin setup parameter" << name << "not found";
			throw NotFoundException(msg.str());
		}
		return *(it->second);
	}

	SetupParameter const& Plugin::getSetupParameter(std::string const& name) const
	{
		ParameterMap::const_iterator it = m_SetupParameters.find(name);
		if (it == m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "plugin setup parameter" << name << "not found";
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

}