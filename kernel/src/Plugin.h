#pragma once

#include "Common.h"
#include "IPluginActivator.h"
#include "PluginManifest.h"

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
			ACTIVE = 0x00000010,
		};

		enum PluginActivationPolicy
		{
			ACTIVATION_LAZY = 0x00000001,
			ACTIVATION_EAGER = 0x00000002
		};

		~Plugin();

		void start(bool overrideStartingPolicy);	//starts an installed plugin
		void stop();								//stops an installed plugin
		void uninstall();							//uninstalls a plugin from the framework

		const PluginState getState();

	private:

		Plugin(const std::string& name, PluginFrameworkContext* fwContext);
		
		void activate();
		void deactivate();
		void startDependencies();
		void stopDependencies();
		void resolveDependencies();
		void removeDependencies();
		void loadLibrary();
		void unloadLibrary();

		PluginContext* getContext();
		PluginFrameworkContext* getFrameworkContext();

		PluginFrameworkContext*		m_FrameworkContext;		//framework context
		PluginContext*				m_Context;				//execution context
		PluginState					m_State;
		PluginLoader				m_PluginLoader;			//poco classloader
		std::string					m_PluginName;
		std::string					m_LibraryPath;
		IPluginActivator*			m_Activator;			//the activator interface, that every plugin needs to implement
		PluginActivationPolicy		m_ActivationPolicy;		//lazy or eager activation
		Plugins						m_Dependencies;			//the other plugins on which this one depends
		PluginManifest				m_Metadata;				//plugin metadata

		friend class PluginFrameworkContext;
		friend class PluginContext;
	};
}