
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

#include <sstream>

namespace _2Real
{

	Plugin::Plugin(Identifier const& id, std::string const& directory, std::string const& file, std::string const& classname, SystemGraph &system) :
		Entity(id),
		m_System(system),
		m_Metadata(classname, directory, system.getAllowedTypes()),
		m_Activator(NULL),
		m_IsInitialized(false),
		m_ServiceTemplates(),
		m_SetupParameters(),
		m_File(directory+file),
		m_PluginLoader()
	{
	}

	Plugin::~Plugin()
	{
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

		for (ParameterMap::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); ++it)
		{
			delete it->second;
		}

		m_SetupParameters.clear();
		m_ServiceTemplates.clear();
		m_Services.clear();
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

	void Plugin::setParameterValue(std::string const& name, EngineData &data)
	{
		getSetupParameter(name).setData(data);
	}

	EngineData Plugin::getParameterValue(std::string const& name) const
	{
		return getSetupParameter(name).getData();
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

	const Identifier Plugin::createService(std::string const& idName, std::string const& serviceName)
	{
		IService &service = createService(serviceName);

		const StringMap setup = m_Metadata.getSetupParameters(serviceName);
		const StringMap input = m_Metadata.getInputSlots(serviceName);
		const StringMap output = m_Metadata.getOutputSlots(serviceName);

		const Identifier id = Entity::createIdentifier(idName, "service");
		Service *runnable = new Service(id, service, m_System, setup, input, output);

		unsigned int index = m_System.childCount();
		m_System.insertChild(*runnable, index);

		return id;
	}

	bool Plugin::canExportService(std::string const& name) const
	{
		return m_Metadata.containsServiceMetadata(name);
	}

	void Plugin::install()
	{
		try
		{
			//1st, load dll
			try
			{
				m_PluginLoader.loadLibrary(m_File);
			}
			catch (Poco::LibraryLoadException &e)
			{
				throw _2Real::Exception(e.what());
			}

			//2nd, create plugin activator instance
			if (m_PluginLoader.canCreate(m_Metadata.getClassname()))
			{
				try
				{
					m_Activator = m_PluginLoader.create(m_Metadata.getClassname());
				}
				catch (Poco::NotFoundException &e)
				{
					throw Exception(e.what());
				}
			}
			else
			{
				std::ostringstream msg;
				msg << "invalid library: does not contain IPluginActivator";
				throw Exception(msg.str());
			}

			//3rd, read the metadata
			Metadata metadata(m_Metadata);
			m_Activator->getMetadata(metadata);

			//4th, create setup parameters from the metadata
			StringMap setupParameters = m_Metadata.getSetupParameters();
			for (StringMap::iterator it = setupParameters.begin(); it != setupParameters.end(); ++it)
			{
				std::string name = it->first;
				std::string keyword = it->second;
				std::string type = m_System.getAllowedTypes().find(keyword)->second;

				SetupParameter *parameter = new SetupParameter(name, type, keyword);
				m_SetupParameters.insert(NamedParameter(name, parameter));
			}
		}
		catch (_2Real::Exception &e)
		{
			if (m_Activator)
			{
				m_PluginLoader.destroy(m_Metadata.getClassname(), m_Activator);
				m_Activator = NULL;
			}

			if (m_PluginLoader.isLibraryLoaded(m_File))
			{
				m_PluginLoader.unloadLibrary(m_File);
			}

			e.rethrow();
		}
	}

	void Plugin::setup()
	{
		PluginContext context(*this);
		m_Activator->setup(context);
		m_IsInitialized = true;
	}

	void Plugin::uninstall()
	{
		if (m_Activator)
		{
			delete m_Activator;
			m_Activator = NULL;
		}

		if (m_PluginLoader.isLibraryLoaded(m_File))
		{
			m_PluginLoader.unloadLibrary(m_File);
		}
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