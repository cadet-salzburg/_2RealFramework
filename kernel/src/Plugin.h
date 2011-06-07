#pragma once

#include "Common.h"
#include "IPluginActivator.h"


namespace _2Real
{
	class Plugin
	{
	
	public:
		
		enum PluginState
		{
			UNINSTALLED = 0x00000001,
			INSTALLED = 0x00000002,
			RESOLVED = 0x00000004,
			STARTING = 0x00000008,
			STOPPING = 0x00000010,
			ACTIVE = 0x00000020
		};

		enum PluginActivationPolicy
		{
			ACTIVATION_LAZY = 0x00000001,
			ACTIVATION_EAGER = 0x00000002
		};

		~Plugin();

		void start();				//starts an installed plugin
		void stop();				//stops an installed plugin
		void uninstall();			//uninstalls a plugin from the framework

		const PluginState getState();

	private:

		Plugin(const std::string& name, PluginFrameworkContext* fwContext);
		
		void activate();
		void deactivate(const bool activated);
		
		void startDependencies();
		void stopDependencies();
		
		void loadLibrary();
		void unloadLibrary();

		PluginContext* getContext();
		PluginFrameworkContext* getFrameworkContext();

		PluginFrameworkContext*		m_FrameworkContext;
		PluginContext*				m_Context;
		
		PluginState					m_State;
		PluginLoader				m_PluginLoader;

		bool						m_isActivating;
		bool						m_isDeactivating;

		std::string					m_PluginName;
		std::string					m_LibraryPath;

		IPluginActivator*			m_Activator;

		PluginActivationPolicy		m_ActivationPolicy;

		friend class PluginFrameworkContext;
		friend class PluginContext;
	};
}