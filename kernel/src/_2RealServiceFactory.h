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

#include "_2RealServiceReference.h"

namespace _2Real
{
	class ServiceFactory
	{

	public:

		ServiceFactory();

		/**
		*	registers factory function of a service
		*	_name = human readable name
		*	_pluginPtr = pointer to plugin that does the registering
		*	_creator = factory function
		*	_singleton = if service in question is a singleton
		*/
		void registerService(std::string const& _name, PluginPtr _plugin, UserServiceCreator _creator, bool const& _singleton);

		void registerFrameworkService(std::string const& _name, ServiceCreator _creator);

		/**
		*	returns true if service in question can be created
		*	_name = human readable name of service
		*	_plugin = human readable name of plugin
		*/
		const bool canCreate(std::string const& _name, std::string const& _plugin) const;

		/**
		*	returns true if the service in question is a singleton
		*	_name = human readable name of service
		*	_plugin = human readable name of plugin
		*/
		const bool isSingleton(std::string const& _name, std::string const& _plugin) const;

		/**
		*	creates instance of service
		*	_name = human readable name of service
		*	_plugin = human readable name of plugin
		*/
		NamedServicePtr createService(std::string const& _name, std::string const& _plugin);

		NamedServicePtr createServiceContainer(std::string const& _name, std::string const& _plugin);

	private:

		typedef std::pair< std::string, ServiceRefPtr >		NamedServiceRefPtr;
		typedef std::map< std::string, ServiceRefPtr >		NamedReferenceMap;
		typedef std::map< ServiceName, ServicePtr >			NamedServiceMap;;

		template< class T >
		static ServicePtr createContainer()
		{
			return ServicePtr(new T());
		}

		const ServiceName generateName(NamedServiceRefPtr const& _ref) const;

		NamedReferenceMap									m_ReferenceMap;
		NamedServiceMap										m_ServiceMap;

		unsigned int										m_iCreationCount;
	};
}