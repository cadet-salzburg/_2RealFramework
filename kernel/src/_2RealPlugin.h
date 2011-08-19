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

#include "_2RealIEntity.h"
#include "_2RealServiceTypedefs.h"
#include "_2RealIPluginActivator.h"
#include "_2RealIdentifierImpl.h"

#include <vector>

#include "Poco/ClassLoader.h"

namespace _2Real
{

	/**
	*
	*/

	class Metadata;
	class ServiceFactory;
	class IdentifierImpl;

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
		Plugin(std::string const& _path, std::string const& _class, ServiceFactory *const _factory) throw(...);

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
		std::string const& name() const;

		/**
		*	
		*/
		std::string const& path() const;

		/**
		*	
		*/
		const Metadata *const metadata() const;

		/**
		*
		*/
		std::vector< IdentifierImpl > const& serviceIDs() const;

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
		void start() throw(...);

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
		*	
		*/
		IPluginActivator				*m_Activator;

		/**
		*	
		*/
		Metadata						*m_Metadata;

		/**
		*
		*/
		ServiceFactory					*m_Factory;

		/**
		*	
		*/
		ePluginState					m_State;

		/**
		*	
		*/
		std::string						m_ClassName;

		/**
		*	
		*/
		std::string						m_LibraryPath;

		/**
		*
		*/
		PluginLoader					m_PluginLoader;

		/**
		*	
		*/
		std::vector< IdentifierImpl >	m_Services;

	};
}