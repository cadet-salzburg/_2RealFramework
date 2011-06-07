#include "Plugin.h"
#include "PluginContext.h"
#include "PluginFrameworkContext.h"

#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"

namespace _2Real
{
	Plugin::Plugin(const std::string& name, PluginFrameworkContext* fwContext) :	m_FrameworkContext(fwContext), m_State(Plugin::UNINSTALLED), m_isActivating(false), 
																					m_isDeactivating(false), m_ActivationPolicy(Plugin::ACTIVATION_EAGER), m_Activator(NULL)
	{
		m_PluginName = name;
		m_LibraryPath  = std::string(Poco::Path::current() + "..\\..\\plugin\\" + m_PluginName);

		int sz = m_PluginName.size();
		if(m_PluginName[sz-1] == 'd' && m_PluginName[sz-2] == '_')
		{
			m_PluginName = m_PluginName.substr(0, sz-2);
		}
		 
		m_Context = new PluginContext(this);
		m_State = Plugin::INSTALLED;
	}

	Plugin::~Plugin()
	{
		delete m_Context;
		delete m_Activator;
		m_FrameworkContext = NULL;
	}

	void Plugin::start()
	{
		if (m_State == Plugin::UNINSTALLED)
		{
			throw std::logic_error("Plugin::start trying to start uninstalled plugin");
		}
		else if (m_State == Plugin::ACTIVE)
		{
		}

		else if (m_State == Plugin::INSTALLED)
		{
			try
			{
				//m_FrameworkContext->resolvePlugin();
			}
			catch (std::logic_error e)
			{
				throw;
			}

			m_State = Plugin::RESOLVED;
		}

		if (m_ActivationPolicy == Plugin::ACTIVATION_LAZY)
		{
			if (m_State == Plugin::STARTING) 
			{
			}
			
			m_State = STARTING;
		}
		else
		{
			activate();
		}
	}

	void Plugin::stop()
	{
		if (m_State == Plugin::UNINSTALLED)
		{
			throw std::logic_error("Plugin::start trying to stop uninstalled plugin");
		}
		bool started = false;

		switch (m_State)
		{
			case INSTALLED:
			case RESOLVED:
			case STOPPING:
			case UNINSTALLED:
				return;

			case ACTIVE:
				started = true;
			//lazy activation
			case STARTING:
				try
				{
					//unloadLibrary(started);
				}
				catch (...)
				{
				}
				break;
		}
	}

	void Plugin::uninstall()
	{
		bool resolved = false;

		switch (m_State)
		{
		case UNINSTALLED:
			//throw std::logic_error("Plugin::uninstall trying to uninstall uninstalled plugin");
		case STARTING:
		case ACTIVE:
		case STOPPING:
		try
		{
			if ((m_State == ACTIVE) | (m_State == STARTING))
			{
				try
				{
					//unloadLibrary(m_State == ACTIVE);
				}
				catch (...)
				{
				}
			}
		}
		catch (...)
		{
			m_isDeactivating = false;
		}
		case RESOLVED:
			resolved = true;
		case INSTALLED:
			if (resolved)
			{
				//event
			}

			m_State = UNINSTALLED;
			break;
		}
	}

	void Plugin::activate()
	{
		if (m_State == Plugin::INSTALLED)
		{
			try
			{
				//m_FrameworkContext->resolvePlugin(m_Ptr);
			}
			catch (...)
			{
				throw;
			}
			
			m_State = Plugin::RESOLVED;
		}

		switch (m_State)
		{
		case Plugin::INSTALLED:
			std::logic_error("Plugin::activate internal error");
		case Plugin::STARTING:
			if (m_isActivating) 
			{
				return;
			}
		case Plugin::RESOLVED:
			m_State = Plugin::STARTING;
			m_isActivating = true;
			try
			{
				//startDependencies();
				loadLibrary();
			}
			catch (...)
			{
				m_State = Plugin::STOPPING;
				//removeDependencies();
				m_State = Plugin::RESOLVED;
				m_isActivating = false;
				throw;
			}
			m_isActivating = false;
			break;
		case Plugin::ACTIVE:
			break;
		case Plugin::STOPPING:
			throw std::logic_error("Plugin::activate start called from stop");
		case Plugin::UNINSTALLED:
			throw std::logic_error("Plugin::activate trying to start uninstalled plugin");
		}
	}

	void Plugin::deactivate(const bool started)
	{
		m_State = Plugin::STOPPING;
		m_isDeactivating = true;
  
		try
		{
			if (started && m_Activator)
			{
				try
				{
					unloadLibrary();
				}
				catch (...)
				{
				}
			}

			//stopDependencies();
		}
		catch (...)
		{
		}

		if (m_State != Plugin::UNINSTALLED)
		{
			m_State = Plugin::RESOLVED;
			m_isDeactivating = false;
		}
	}

	void Plugin::loadLibrary()
	{
		try {
			
			m_PluginLoader.loadLibrary(m_LibraryPath);
			if (!m_PluginLoader.isLibraryLoaded(m_LibraryPath))
			{
				throw std::logic_error("Plugin::loadLibrary could not load library");
			}

			if(m_PluginLoader.canCreate(m_PluginName))
			{
				m_Activator = m_PluginLoader.create(m_PluginName);
				if (!m_Activator)
				{
					throw std::logic_error("Plugin::loadLibrary could create plugin activator");
				}

				m_Activator->start(m_Context);
				m_State = Plugin::ACTIVE;
			}
			else
			{
				throw std::logic_error("Plugin::loadLibrary cannot create plugin activator");
			}
		}
		catch(std::logic_error e)
		{
			if (!m_PluginLoader.isLibraryLoaded(m_LibraryPath))
			{
				m_PluginLoader.unloadLibrary(m_LibraryPath);
			}

			//error handling
		}
	}

	void Plugin::unloadLibrary()
	{
		try
		{
			if(m_PluginLoader.isLibraryLoaded(m_LibraryPath))
			{
				m_Activator->stop(m_Context);
				m_PluginLoader.unloadLibrary(m_LibraryPath);
				
				delete [] m_Activator;
				m_Activator = NULL;
			}
			else
			{
				throw std::logic_error("Plugin::loadLibrary cannot unload library");
			}
		}
		catch(std::logic_error e)
		{
			//error handling
		}
	}

	PluginFrameworkContext* Plugin::getFrameworkContext()
	{
		return m_FrameworkContext;
	}

	PluginContext* Plugin::getContext()
	{
		return m_Context;
	}
}