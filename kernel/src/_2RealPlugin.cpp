
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
#include "_2RealException.h"
#include "_2RealServiceMetadata.h"
#include "_2RealSetupParameter.h"
#include "_2RealSystemGraph.h"

#include <iostream>
#include <sstream>

namespace _2Real
{

	Plugin::Plugin(Identifier const& id, std::string const& directory, std::string const& file, std::string const& classname, SystemGraph const& system) :
		Entity(id),
		m_System(system),
		m_Metadata(classname, directory, m_System.getAllowedTypes()),
		m_Activator(NULL),
		m_IsInitialized(false),
		m_Services(),
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
		catch (...)
		{
			m_System.getLogstream() << "error on plugin uninstallation, " << name() << "\n";
		}

		for (ParameterMap::iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); ++it)
		{
			delete it->second;
		}

		m_SetupParameters.clear();
		m_Services.clear();
		m_File.clear();
	}

	void Plugin::registerService(std::string const& name, ServiceCreator service)
	{
		TemplateMap::iterator it = m_Services.find(name);
		if (it != m_Services.end())
		{
			std::ostringstream msg;
			msg << "error: service " << name << " was already exported";
			throw _2Real::Exception(msg.str());
		}

		m_Services.insert(NamedTemplate(name, service));
	}

	IService *const Plugin::createService(std::string const& name) const
	{
		TemplateMap::const_iterator it = m_Services.find(name);
		if (it == m_Services.end())
		{
			std::ostringstream msg;
			msg << "internal error: plugin " << this->name() << " does not export service " << name;
			throw _2Real::Exception(msg.str());
		}

		return it->second();
		return NULL;
	}

	void Plugin::addSetupParameter(SetupParameter *const parameter)
	{
		ParameterMap::iterator it = m_SetupParameters.find(parameter->name());
		if (it != m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "internal error: parameter" << parameter->name() << " already exists";
			throw _2Real::Exception(msg.str());
		}

		m_SetupParameters.insert(NamedParameter(parameter->name(), parameter));
	}

	const bool Plugin::canExportService(std::string const& name) const
	{
		return m_Metadata.containsServiceMetadata(name);
	}

	const bool Plugin::exportsService(std::string const& name) const
	{
		return (m_Services.find(name) != m_Services.end());
	}

	const bool Plugin::hasSetupParameter(std::string const& name) const
	{
		return (m_SetupParameters.find(name) != m_SetupParameters.end());
	}

	PluginMetadata const& Plugin::getMetadata() const
	{
		return m_Metadata;
	}

	ServiceMetadata const& Plugin::getMetadata(std::string const& name) const
	{
		return m_Metadata.getServiceMetadata(name);
	}

	bool const& Plugin::isInitialized() const
	{
		return m_IsInitialized;
	}

	SetupParameter & Plugin::getSetupParameter(std::string const& name)
	{
		ParameterMap::iterator it = m_SetupParameters.find(name);
		if (it == m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "error: parameter" << name << "does not exist";
			throw _2Real::Exception(msg.str());
		}

		return *(it->second);
	}

	SetupParameter const& Plugin::getSetupParameter(std::string const& name) const
	{
		ParameterMap::const_iterator it = m_SetupParameters.find(name);
		if (it == m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "error: parameter" << name << "does not exist";
			throw _2Real::Exception(msg.str());
		}

		return *(it->second);
	}

	//SharedAny Plugin::getParameterValue(std::string const& name)
	//{
	//	ParameterMap::iterator it = m_SetupParameters.find(name);
	//	if (it == m_SetupParameters.end())
	//	{
	//		std::ostringstream msg;
	//		msg << "error: parameter" << name << "does not exist";
	//		throw _2Real::Exception(msg.str());
	//	}

	//	SharedAny any = (it->second->get());
	//	return any;
	//}

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
				//a poco error occured
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
					//a poco error occured
					throw Exception(e.what());
				}
			}
			else
			{
				std::ostringstream msg;
				msg << "error: could not load dll";
				throw _2Real::Exception(msg.str());
			}

			//3rd, read the metadata
			m_Activator->getMetadata(m_Metadata);
		}
		catch (_2Real::Exception &e)
		{
			if (m_Activator)
			{
				m_PluginLoader.destroy(m_Metadata.getClassname(), m_Activator);
				m_Activator = NULL;
			}

			throw e;
		}
	}

	void Plugin::setup()
	{
		if (!m_Activator)
		{
			std::ostringstream msg;
			msg << "internal error: setup failed";
			throw _2Real::Exception(msg.str());
		}

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

}