
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
#include "_2RealFactoryReference.h"
#include "_2RealServiceMetadata.h"
#include "_2RealPluginParameter.h"
#include "_2RealAbstractRef.h"

#include <iostream>

namespace _2Real
{

	Plugin::Plugin(std::string const& _dir, std::string const& _file, std::string const& _classname, Id *const _id) :
		Entity(_id),
		m_Metadata(_classname, _dir),
		m_Activator(NULL),
		m_File(_dir + _file),
		m_bIsInitialized(false)
	{
		if (_dir.empty())
		{
			throw Exception("could not create plugin - no install directory");
		}

		if (_file.empty())
		{
			throw Exception("could not create plugin - no dll filename");
		}

		if (_classname.empty())
		{
			throw Exception("could not create plugin - no classname");
		}
	}

	Plugin::Plugin(Plugin const& _src) :
		Entity(_src),
		m_Metadata("", "")
	{
		throw Exception("internal error: attempted to copy an entity");
	}

	Plugin& Plugin::operator=(Plugin const& _src)
	{
		throw Exception("internal error: attempted to assign an entity");
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
		catch (Exception &e)
		{
			std::cout << "destruction error: " << e.what() << std::endl;
		}
	}

	void Plugin::addService(FactoryReference *ref)
	{
		m_Services.push_back(ref);
	}

	Plugin::ServiceTemplates const& Plugin::services() const
	{
		return m_Services;
	}

	Plugin::ServiceTemplates const& Plugin::services()
	{
		return m_Services;
	}

	PluginMetadata const& Plugin::pluginMetadata() const
	{
		return m_Metadata;
	}

	ServiceMetadata const& Plugin::serviceMetadata(std::string const& _name) const
	{
		return m_Metadata.getServiceMetadata(_name);
	}

	bool const& Plugin::isInitialized() const
	{
		return m_bIsInitialized;
	}

	IDs Plugin::setupParamIDs() const
	{
		return m_SetupIds;
	}

	
	void Plugin::addSetupParameter(unsigned int const& id, PluginParameter *const _param)
	{
		if (_param == NULL)
		{
			throw Exception("setup parameter could not be added - null pointer");
		}

		m_SetupParams.insert(NamedParam(_param->name(), _param));
		m_SetupIds.push_back(id);
	}

	void Plugin::getParameterValue(std::string const& _name, AbstractRef *const _param)
	{
		ParamMap::iterator it = m_SetupParams.find(_name);
		if (it == m_SetupParams.end())
		{
			throw Exception("attempted to query non-existant setup parameter");
		}

		Poco::Any any = (it->second->value());
		_param->extractFrom(any);
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
				throw Exception("could not install plugin - could not load dll");
			}

			//3rd, read the metadata

			m_Activator->getMetadata(m_Metadata);
		}
		catch (Exception &e)
		{
			if (m_Activator)
			{
				m_PluginLoader.destroy(m_Metadata.getClassname(), m_Activator);
				m_Activator = NULL;
			}

			throw e;
		}
	}

	void Plugin::setup(ServiceFactory *const _factory)
	{
		try
		{
			if (!m_Activator)
			{
				throw Exception("internal error: attempted to set up uninstalled plugin");
			}

			PluginContext context(this, _factory);
			m_Activator->setup(context);

			m_bIsInitialized = true;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Plugin::uninstall()
	{
		try
		{
			m_PluginLoader.destroy(m_Metadata.getClassname(), m_Activator);

			if (m_PluginLoader.isLibraryLoaded(m_File))
			{
				m_PluginLoader.unloadLibrary(m_File);
			}

			//delete m_Activator;
			m_Activator = NULL;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}