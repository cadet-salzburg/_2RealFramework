
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

#include <iostream>

namespace _2Real
{

	Plugin::Plugin(Identifier const& _id, PluginMetadata *const _info) :
		Entity(_id),
		m_Metadata(_info),
		m_Activator(NULL),
		m_IsInitialized(false)
	{
		m_File = m_Metadata->getInstallDirectory() + m_Metadata->getFilename();
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
			std::cout << "uninstall error" << std::endl;
		}

		delete m_Metadata;

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

		for (TemplateMap::iterator it = m_Services.begin(); it != m_Services.end(); it++)
		{
			std::string name;
			try
			{
				name = it->first;
				std::cout << "deleting service template: " << name << std::endl;
				it->second = NULL;
			}
			catch (...)
			{
				std::cout << "error on service template destruction: " << name << std::endl;
			}
		}
	}

	void Plugin::registerService(std::string const& _name, ServiceCreator _service)
	{
		TemplateMap::iterator it = m_Services.find(_name);
		if (it != m_Services.end())
		{
			//exception
		}

		m_Services.insert(NamedTemplate(_name, _service));
	}

	IService *const Plugin::createService(std::string const& _name)
	{
		TemplateMap::iterator it = m_Services.find(_name);
		if (it != m_Services.end())
		{
			//exception
		}

		return it->second();
	}

	void Plugin::addSetupParameter(SetupParameter *const _param)
	{
		m_SetupParameters.insert(NamedParameter(_param->name(), _param));
	}

	PluginMetadata const& Plugin::pluginMetadata() const
	{
		return *m_Metadata;
	}

	ServiceMetadata const& Plugin::serviceMetadata(std::string const& _name) const
	{
		return m_Metadata->getServiceMetadata(_name);
	}

	bool const& Plugin::isInitialized() const
	{
		return m_IsInitialized;
	}

	std::list< unsigned int > Plugin::setupParameterIDs() const
	{
		std::list< unsigned int > result;
		for (ParameterMap::const_iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
		{
			result.push_back(it->second->id());
		}

		return result;
	}

	//std::list< unsigned int > Plugin::serviceIDs() const
	//{
	//	std::list< unsigned int > result;
	//	for (TemplateMap::const_iterator it = m_Services.begin(); it != m_Services.end(); it++)
	//	{
	//		result.push_back(it->second->id());
	//	}

	//	return result;
	//}

	SetupParameter *const Plugin::getSetupParameter(std::string const& _name)
	{
		ParameterMap::iterator it = m_SetupParameters.find(_name);
		if (it == m_SetupParameters.end())
		{
			throw Exception("attempted to query non-existant setup parameter " + _name);
		}

		return it->second;
	}

	//ServiceTemplate *const Plugin::getServiceTemplate(std::string const& _name)
	//{
	//	TemplateMap::iterator it = m_Services.find(_name);
	//	if (it == m_Services.end())
	//	{
	//		throw Exception("attempted to query non-existant service template");
	//	}

	//	return it->second;
	//}

	SharedAny Plugin::getParameterValue(std::string const& _name)
	{
		ParameterMap::iterator it = m_SetupParameters.find(_name);
		if (it == m_SetupParameters.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		SharedAny any = (it->second->get());
		return any;
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
				//a poco error occured
				throw Exception(e.what());
			}

			//2nd, create plugin activator instance
			if (m_PluginLoader.canCreate(m_Metadata->getClassname()))
			{
				try
				{
					m_Activator = m_PluginLoader.create(m_Metadata->getClassname());
				}
				catch (Poco::NotFoundException &e)
				{
					//a poco error occured
					throw Exception(e.what());
				}
			}
			else
			{
				throw Exception("could not install plugin - could not load dll");
			}

			//3rd, read the metadata
			m_Activator->getMetadata(*m_Metadata);
		}
		catch (Exception &e)
		{
			if (m_Activator)
			{
				m_PluginLoader.destroy(m_Metadata->getClassname(), m_Activator);
				m_Activator = NULL;
			}

			throw e;
		}
	}

	void Plugin::setup()
	{
		if (!m_Activator)
		{
			throw Exception("internal error: attempted to set up uninstalled plugin");
		}

		PluginContext context(this);
		m_Activator->setup(context);

		m_IsInitialized = true;
	}

	void Plugin::uninstall()
	{
		delete m_Activator;
		m_Activator = NULL;

		if (m_PluginLoader.isLibraryLoaded(m_File))
		{
			m_PluginLoader.unloadLibrary(m_File);
		}
	}
}