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

#include "_2RealTypedefs.h"

#include "_2RealFramework.h"
#include "_2RealIPluginActivator.h"
#include "_2RealMetadata.h"

#include "Poco/ClassLoader.h"

namespace _2Real
{
	class Plugin
	{

	public:

		enum PluginState
		{
			INVALID		=	0x00000000,
			INSTALLED	=	0x00000001,
			LOADED		=	0x00000002,
			ACTIVE		=	0x00000004,
		};

		/**
		*
		*/
		Plugin(std::string const& _name, std::string const& _path, Framework &_context);

		/**
		*
		*/
		PluginState const& state() const;
		
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
		MetadataPtr const& metadata() const;
		
		/**
		*	loads dll; state: installed->loaded
		*	error -> invalid
		*/
		void load();

		/**
		*	unloads dll; state: loaded->installed
		*	error -> invalid
		*/
		void unload();
		
		/**
		*	starts plugin activator; state: loaded->active
		*	error -> invalid
		*/
		void start();
		
		/**
		*	stops plugin activator; state: active->loaded
		*	error -> invalid
		*/
		void stop();

		/**
		*
		*/
		void registerService(std::string const& _name, UserServiceCreator _creator, bool const& _singleton) const;

	private:

		typedef Poco::ClassLoader< IPluginActivator >	PluginLoader;

		/**
		*
		*/
		Framework				&m_Framework;
		
		/**
		*
		*/
		PluginContext			*m_ContextPtr;
		
		/**
		*
		*/
		IPluginActivator		*m_ActivatorPtr;
		
		/**
		*
		*/
		PluginLoader			m_PluginLoader;
		
		/**
		*
		*/
		PluginState				m_PluginState;
		
		/**
		*
		*/
		std::string				m_PluginName;
		
		/**
		*
		*/
		std::string				m_LibraryPath;
		
		/**
		*
		*/
		MetadataPtr				m_MetadataPtr;

	};
}