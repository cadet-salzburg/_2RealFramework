
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
#include "_2RealServiceFactory.h"
#include "_2RealServiceMetadata.h"

#include <iostream>

namespace _2Real
{

	Plugin::Plugin(std::string const& _dir, std::string const& _file, std::string const& _class, ServiceFactory *const _factory, IdentifierImpl *const _id) :
		IEntity(_id),
		m_Factory(_factory),
		m_Metadata(_class, _dir),
		m_Activator(NULL),
		m_State(Plugin::UNINSTALLED),
		m_File(_dir + _file)
	{
	}

	Plugin::Plugin(Plugin const& _src) :
		IEntity(_src),
		m_Metadata(_src.m_Metadata)
	{
		throw Exception::noCopy();
	}

	Plugin& Plugin::operator=(Plugin const& _src)
	{
		throw Exception::noCopy();
	}

	Plugin::~Plugin()
	{
		if (m_State != Plugin::UNINSTALLED)
		{
			try
			{
				stop();
				unload();
				uninstall();
			}
			catch (...)
			{
				std::cout << "plugin destruction error" << std::endl;
			}
		}
	}

	Plugin::ePluginState const& Plugin::state() const
	{
		return m_State;
	}

	PluginMetadata const& Plugin::metadata() const
	{
		return m_Metadata;
	}

	std::list< unsigned int > const& Plugin::serviceIDs() const
	{
		return m_Services;
	}

	void Plugin::registerService(std::string const& _name, ServiceCreator _creator)
	{
		try
		{
			ServiceMetadata data = m_Metadata.getServiceMetadata(_name);
			const unsigned int id = m_Factory->registerService(_name, this, data, _creator);
			m_Services.push_back(id);
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	void Plugin::install()
	{
		if (m_State == Plugin::UNINSTALLED)
		{
			if (m_Metadata.getInstallDirectory().empty())
			{
				throw Exception::failure();
			}

			if (m_Metadata.getClassname().empty())
			{
				throw Exception::failure();
			}

			if (m_Factory == NULL)
			{
				throw Exception::failure();
			}

			m_State = Plugin::INSTALLED;
		}
		else
		{
			m_State = Plugin::INVALID;
			throw Exception::failure();
		}
	}

	void Plugin::uninstall() throw(...)
	{
		try
		{
			if (m_State == Plugin::ACTIVE)
			{
				stop();
			}

			if (m_State == Plugin::LOADED)
			{
				unload();
			}

			m_State = Plugin::UNINSTALLED;
		}
		catch (...)
		{
			m_State = Plugin::INVALID;
			throw Exception::failure();
		}
	}

	void Plugin::load()
	{
		if (m_State == Plugin::INSTALLED)
		{
			try
			{
				m_PluginLoader.loadLibrary(m_File);
			}
			catch (...)
			{
				m_State = Plugin::INVALID;
				throw Exception::failure();
			}

			m_State = Plugin::LOADED;
		}
		else
		{
			throw Exception::failure();
		}
	}

	void Plugin::start(std::list< unsigned int > &_ids)
	{
		if (m_State == Plugin::LOADED)
		{
			try
			{
				if (m_PluginLoader.canCreate(m_Metadata.getClassname()))
				{
					m_Activator = m_PluginLoader.create(m_Metadata.getClassname());
					if (!m_Activator)
					{
						m_State = Plugin::INVALID;
						throw Exception::failure();
					}

					PluginContext context(this);
					//get metadata
					m_Activator->getMetadata(m_Metadata);

					//start
					m_Activator->start(context);

					_ids.clear();
					_ids = m_Services;
				}
				else
				{
					m_State = Plugin::INVALID;
					throw Exception::failure();
				}
			}
			catch (...)
			{
				if (m_Activator)
				{
					delete m_Activator;
					m_Activator = NULL;
				}

				if (!m_Services.empty())
				{
					//clean up services
				}

				m_State = Plugin::INVALID;
				throw Exception::failure();
			}

			m_State = Plugin::ACTIVE;
		}
		else
		{
			throw Exception::failure();
		}
	}

	void Plugin::stop()
	{
		try
		{
			if (m_State == Plugin::ACTIVE)
			{
				m_PluginLoader.destroy(m_Metadata.getClassname(), m_Activator);
				delete m_Activator;
				m_State = Plugin::LOADED;
			}
		}
		catch (...)
		{
			m_State = Plugin::INVALID;
			throw Exception::failure();
		}
	}

	void Plugin::unload()
	{
		try
		{
			if (m_State == Plugin::LOADED)
			{
				if (m_PluginLoader.isLibraryLoaded(m_File))
				{
					m_PluginLoader.unloadLibrary(m_File);
				}
			}

			m_State = Plugin::INSTALLED;
		}
		catch (...)
		{
			m_State = Plugin::INVALID;
			throw Exception::failure();
		}
	}
}