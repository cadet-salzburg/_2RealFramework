
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
#include "_2RealSystemGraph.h"
#include "_2RealException.h"
#include "_2RealSetupParameter.h"
#include "_2RealService.h"
#include "_2RealParameterMetadata.h"
#include "_2RealData.h"

#include <sstream>

namespace _2Real
{

	Plugin::Plugin(Identifier const& id, std::string const& classname, Poco::Path const& dir, SystemGraph &system, IPluginActivator *activator) :
		Entity(id),
		m_ServiceTemplates(),
		m_SetupParameters(),
		m_Services(),
		m_Activator(activator),
		m_Classname(classname),
		m_IsSetUp(false),
		m_System(system),
		m_Metadata(classname, dir.toString())
	{
	}

	Plugin::~Plugin()
	{
		for (ParameterMap::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); ++it)
		{
			delete it->second;
		}

		m_SetupParameters.clear();
		m_ServiceTemplates.clear();
		m_Services.clear();

		try
		{
			if (m_Activator)
			{
				uninstall();
			}
		}
		catch (std::exception &e)
		{
		}
	}

	const std::string Plugin::getInfoString() const
	{
		std::ostringstream info;
		info << m_Metadata;
		return info.str();
	}

	bool Plugin::isSetUp() const
	{
		return m_IsSetUp;
	}

	void Plugin::registerService(std::string const& name, ServiceCreator service)
	{
		if (canExportService(name))
		{
			TemplateMap::iterator it = m_ServiceTemplates.find(name);
			if (it != m_ServiceTemplates.end())
			{
				std::ostringstream msg;
				msg << "service " << name << " already exists";
				throw AlreadyExistsException(msg.str());
			}

			m_ServiceTemplates.insert(NamedTemplate(name, service));
		}
		else
		{
			std::ostringstream msg;
			msg << "service " << name << " not found in plugin metadata";
			throw NotFoundException(msg.str());
		}
	}

	const std::string Plugin::getClassname() const
	{
		return m_Classname;
	}

	void Plugin::setParameterValue(std::string const& paramName, Data const& data)
	{
		getSetupParameter(paramName).setData(data.data());
	}

	std::string const& Plugin::getParameterKey(std::string const& paramName) const
	{
		return getSetupParameter(paramName).getKeyword();
	}

	EngineData const& Plugin::getParameterValue(std::string const& paramName) const
	{
		return getSetupParameter(paramName).getData();
	}

	IService & Plugin::createService(std::string const& serviceName) const
	{
		TemplateMap::const_iterator it = m_ServiceTemplates.find(serviceName);
		if (it == m_ServiceTemplates.end())
		{
			std::ostringstream msg;
			msg << "internal error: plugin " << this->name() << " does not export service " << serviceName;
			throw Exception(msg.str());
		}

		return *(it->second());
	}

	Runnable & Plugin::createService(std::string const& idName, std::string const& serviceName)
	{
		IService &service = createService(serviceName);
		const Identifier id = Entity::createIdentifier(idName, "service");
		Service *runnable = new Service(id, service, m_System, m_Metadata.getServiceMetadata(serviceName));
		return *runnable;
	}

	bool Plugin::canExportService(std::string const& name) const
	{
		return m_Metadata.containsServiceMetadata(name);
	}

	void Plugin::setup()
	{
		PluginContext context(*this);
		m_Activator->setup(context);
		m_IsSetUp = true;
	}

	void Plugin::install()
	{
		//try
		//{
		//	//1st, load dll
		//	try
		//	{
		//		m_PluginLoader.loadLibrary(m_File);
		//	}
		//	catch (Poco::LibraryLoadException &e)
		//	{
		//		throw _2Real::Exception(e.what());
		//	}

		//	//2nd, create plugin activator instance
		//	if (m_PluginLoader.canCreate(m_Metadata.getClassname()))
		//	{
		//		try
		//		{
		//			m_Activator = m_PluginLoader.create(m_Metadata.getClassname());
		//		}
		//		catch (Poco::NotFoundException &e)
		//		{
		//			throw Exception(e.what());
		//		}
		//	}
		//	else
		//	{
		//		std::ostringstream msg;
		//		msg << "invalid library: does not contain IPluginActivator";
		//		throw Exception(msg.str());
		//	}

		//	//3rd, read the metadata
		//	Metadata metadata(m_Metadata);
		//	m_Activator->getMetadata(metadata);

		//	//build setup params
		//	ParameterDataMap const& setup = m_Metadata.getSetupParameters();
		//	for (ParameterDataMap::const_iterator it = setup.begin(); it != setup.end(); ++it)
		//	{
		//		SetupParameter *setup = new SetupParameter(*it->second);
		//		m_SetupParameters.insert(NamedParameter(it->second->getName(), setup));
		//	}
		//}
		//catch (_2Real::Exception &e)
		//{
		//	if (m_Activator)
		//	{
		//		m_PluginLoader.destroy(m_Metadata.getClassname(), m_Activator);
		//		m_Activator = NULL;
		//	}

		//	if (m_PluginLoader.isLibraryLoaded(m_File))
		//	{
		//		m_PluginLoader.unloadLibrary(m_File);
		//	}

		//	e.rethrow();
		//}
	}

	void Plugin::uninstall()
	{
		//m_Metadata.clear();

		//if (m_Activator)
		//{
		//	m_Activator->shutdown();
		//	delete m_Activator;
		//	m_Activator = NULL;
		//}

		//if (m_PluginLoader.isLibraryLoaded(m_File))
		//{
		//	m_PluginLoader.unloadLibrary(m_File);
		//}
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