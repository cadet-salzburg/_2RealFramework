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

#include "_2RealFramework.h"
#include "_2RealIPluginActivator.h"

#include "Poco/ClassLoader.h"

namespace _2Real
{
	class _2RealPlugin
	{

	public:

		enum _2RealPluginState
		{
			UNINSTALLED =	0x00000001,
			INSTALLED	=	0x00000002,
			LOADED		=	0x00000004,
			ACTIVE		=	0x00000008,
		};

		~_2RealPlugin();

		const _2RealPluginState state() const;
		const std::string name() const;
		const std::string path() const;
		_2RealMetadataPtr metadata() const;
		
		void load();
		void unload();
		void start();
		void stop();
		void uninstall();
		
		_2RealServicePtr createService(_2RealConfigMetadataPtr _config);

	private:

		typedef Poco::ClassLoader<_2RealIPluginActivator>	_2RealPluginLoader;

		_2RealPlugin(const std::string& _name, const std::string& _dir, _2RealContextPrivate* _context);
		
		void activate();
		void deactivate();
		void loadLibrary();
		void unloadLibrary();

		_2RealContextPrivate*		m_ContextPtr;			//framework context
		_2RealPluginContextPtr		m_PluginContextPtr;		//execution context
		_2RealPluginState			m_PluginState;			//state of plugin
		_2RealPluginLoader			m_PluginLoader;			//poco classloader
		_2RealIPluginActivator*		m_PluginActivator;		//the activator interface as implemented by the plugin
		std::string					m_PluginName;			//name of plugin
		std::string					m_LibraryPath;			//absolute path to dll

		friend class _2RealContextPrivate;

	};
}