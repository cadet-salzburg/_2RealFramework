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

	class Plugin : public Entity
	{

	public:

		/**
		*	
		*/
		Plugin(std::string const& _dir, std::string const& _file, std::string const& _class, Id *const _id) throw(...);

		/**
		*	
		*/
		Plugin(Plugin const& _src) throw(...);

		/**
		*	
		*/
		Plugin& operator=(Plugin const& _src) throw (...);

		/**
		*	
		*/
		~Plugin();

		/**
		*	
		*/
		PluginMetadata const& pluginMetadata() const;

		/**
		*	
		*/
		ServiceMetadata const& serviceMetadata(std::string const& _name) const;

		/**
		*
		*/
		std::list< FactoryReference *> const& services() const;

		/**
		*
		*/
		std::list< FactoryReference *> const& services();

		/**
		*
		*/
		void addService(FactoryReference *ref);

		/**
		*
		*/
		void install(ServiceFactory *const _factory) throw(...);

		/**
		*
		*/
		void uninstall() throw(...);

	private:

		/**
		*	poit
		*/
		typedef Poco::ClassLoader< IPluginActivator >	PluginLoader;

		/**
		*	dll filepath
		*/
		std::string						m_File;

		/**
		*	plugin activator implemented by user plugin
		*/
		IPluginActivator				*m_Activator;

		/**
		*	metadata
		*/
		PluginMetadata					m_Metadata;

		/**
		*	poco classloader
		*/
		PluginLoader					m_PluginLoader;

		/**
		*	services
		*/
		std::list< FactoryReference *>	m_Services;

	};
}