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

#include "_2RealEngineTypedefs.h"
#include "_2RealEntity.h"
#include "_2RealIPluginActivator.h"
#include "_2RealPluginMetadata.h"

#include <list>

#include "Poco/ClassLoader.h"

namespace _2Real
{

	/**
	*	plugin class; responsible for loading dlls & storing plugin metadata
	*/

	class FactoryReference;
	class ServiceFactory;
	class PluginParameter;
	class AbstractRef;

	class Plugin : public Entity
	{

	public:

		/**
		*	dir: install directory, file: name of dll, classname: name of exported class
		*/
		Plugin(std::string const& dir, std::string const& file, std::string const& classname, Id *const id);
		Plugin(Plugin const& _src);
		Plugin& operator=(Plugin const& _src);
		~Plugin();

		typedef std::list< FactoryReference * >						ServiceTemplates;
		typedef Poco::ClassLoader< IPluginActivator >				PluginLoader;

		/**
		*	installation: inserts plugin into plugin pool, loads metadata
		*/
		void														install();

		/**
		*	kicks plugin out of framework
		*/
		void														uninstall();

		/**
		*	calls plugin activator's setup
		*/
		void														setup(ServiceFactory *const factory);

		/**
		*	called by plugin context during plugin activator's setup
		*/
		void														addService(FactoryReference *ref);

		/**
		*	function to get a setup param
		*/
		//void														getSetupParam(std::string const& name, int &param);

		/**
		*	get plugin metadata
		*/
		PluginMetadata const&										pluginMetadata() const;

		/**
		*	get service metadata
		*/
		ServiceMetadata const&										serviceMetadata(std::string const& name) const;

		/**
		*	list of services exported by plugin
		*/
		ServiceTemplates const&										services() const;

		/**
		*	list of services exported by plugin
		*/
		ServiceTemplates const&										services();

		/**
		*	returns true if plugin's setup has been called at least once
		*/
		bool const&													isInitialized() const;

		/**
		*	ids of setup params
		*/
		IDs															setupParamIDs() const;

		/**
		*	setup parameters
		*/
		void														addSetupParameter(unsigned int const& id, PluginParameter *const _param);

		/**
		*	retrieves value of a setup parameter
		*/
		void														getParameterValue(std::string const& _name, AbstractRef *const _param);

	private:

		typedef std::pair< const std::string, PluginParameter * >	NamedParam;
		typedef std::map< const std::string, PluginParameter * >	ParamMap;

		/**
		*	setup parameters
		*/
		ParamMap													m_SetupParams;

		/**
		*	setup parameter ids
		*/
		IDs															m_SetupIds;

		/**
		*	dll filepath
		*/
		std::string													m_File;

		/**
		*	plugin activator implemented by user plugin
		*/
		IPluginActivator											*m_Activator;

		/**
		*	metadata
		*/
		PluginMetadata												m_Metadata;

		/**
		*	poco classloader
		*/
		PluginLoader												m_PluginLoader;

		/**
		*	services
		*/
		ServiceTemplates											m_Services;

		/**
		*	true if plugin's setup has been called at least once
		*/
		bool														m_bIsInitialized;

	};

}