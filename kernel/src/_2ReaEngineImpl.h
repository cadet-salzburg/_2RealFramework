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

#include "Poco/Mutex.h"

namespace _2Real
{

	/**
	*
	*/

	class PluginPool;
	class ServiceFactory;

	class EngineImpl
	{

		friend class Engine;

	public:

		/**
		*	function to install plugins
		*/
		const bool installPlugin(std::string const& _name, std::string const& _path);

		/**
		*	checks if plugin is installed
		*/
		const bool isInstalled(std::string const& _plugin);

		/**
		*	function to register a service
		*/
		void registerService(std::string const& _name, std::string const& _plugin, ServiceCreator _creator, bool const& _singleton);

	private:

		/**
		*	private constructor
		*/
		EngineImpl();

		/**
		*
		*/
		~EngineImpl();

		/**
		*
		*/
		EngineImpl(const EngineImpl &_src);
		
		/**
		*
		*/
		EngineImpl& operator=(const EngineImpl &_src);

		/**
		*	takes care of installed plugins
		*/
		PluginPool					*m_PluginsPtr;

		/**
		*	takes care of service creation
		*/
		ServiceFactory				*m_FactoryPtr;

		/**
		*
		*/
		Poco::Mutex					m_Mutex;

	};
}