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
#include "_2RealIEntity.h"
#include "_2RealIPluginActivator.h"
#include "_2RealPluginMetadata.h"

#include <list>

#include "Poco/ClassLoader.h"

namespace _2Real
{

	/**
	*
	*/

	class PluginMetadata;
	class ServiceFactory;

	class Plugin : public IEntity
	{

	public:

		enum ePluginState
		{
			INVALID		=	0x00000000,
			INSTALLED	=	0x00000001,
			LOADED		=	0x00000002,
			ACTIVE		=	0x00000004,
			UNINSTALLED =	0x00000008,
		};

		/**
		*	
		*/
		Plugin(std::string const& _dir, std::string const& _file, std::string const& _class, ServiceFactory *const _factory, IdentifierImpl *const _id) throw(...);

		/**
		*	
		*/
		Plugin(Plugin const& _src);

		/**
		*	
		*/
		Plugin& operator=(Plugin const& _src);

		/**
		*	
		*/
		~Plugin();

		/**
		*	
		*/
		ePluginState const& state() const;

		/**
		*	
		*/
		PluginMetadata const& metadata() const;

		/**
		*
		*/
		std::list< unsigned int > const& serviceIDs() const;

		/**
		*
		*/
		void install() throw(...);

		/**
		*
		*/
		void uninstall() throw(...);

		/**
		*	loads dll; state: installed->loaded
		*	error -> invalid
		*/
		void load() throw(...);

		/**
		*	unloads dll; state: loaded->installed
		*	error -> invalid
		*/
		void unload() throw(...);

		/**
		*	starts plugin activator; state: loaded->active
		*	error -> invalid
		*/
		void start(std::list< unsigned int > &_ids) throw(...);

		/**
		*	stops plugin activator; state: active->loaded
		*	error -> invalid
		*/
		void stop() throw(...);

		/**
		*	called by plugin context
		*/
		void registerService(std::string const& _name, ServiceCreator _creator);

	private:

		/**
		*	poit
		*/
		typedef Poco::ClassLoader< IPluginActivator >	PluginLoader;

		/**
		*	absolute filepath
		*/
		std::string						m_File;

		/**
		*	
		*/
		IPluginActivator				*m_Activator;

		/**
		*
		*/
		ServiceFactory					*m_Factory;

		/**
		*	
		*/
		PluginMetadata					m_Metadata;

		/**
		*	
		*/
		ePluginState					m_State;

		/**
		*
		*/
		PluginLoader					m_PluginLoader;

		/**
		*	
		*/
		std::list< unsigned int >		m_Services;

	};
}