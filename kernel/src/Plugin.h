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

		const PluginState state() const;
		const std::string& name() const;
		const std::string& attribute(std::string attrib) const;

	private:

		Plugin(const std::string& name, PluginFrameworkContext* fwContext);
		
		void readMetadata();								//read the xml file containing the metadata

		void activate();
		void deactivate();
		void startDependencies();
		void stopDependencies();
		void resolveDependencies();
		void removeDependencies();
		void loadLibrary();
		void unloadLibrary();

		PluginContext*				contextPtr();
		PluginFrameworkContext*		frameworkContextPtr();

		PluginFrameworkContext*		m_FrameworkContext;		//framework context
		PluginContext*				m_Context;				//execution context
		PluginState					m_State;
		PluginLoader				m_PluginLoader;			//poco classloader
		std::string					m_PluginName;
		std::string					m_LibraryPath;
		IPluginActivator*			m_Activator;			//the activator interface, that every plugin needs to implement
		PluginActivationPolicy		m_ActivationPolicy;		//lazy or eager activation
		Plugins						m_Dependencies;			//the other plugins on which this one depends
		PluginManifest*				m_Metadata;				//plugin metadata

		friend class PluginFrameworkContext;
		friend class PluginContext;
	};
}