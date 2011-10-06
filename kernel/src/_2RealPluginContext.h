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

#include "_2RealParamRef.h"

#include <string>
#include <typeinfo>

namespace _2Real
{

	/**
	*	plugin's interface for communication with the framework
	*	allows export of services' factory functions
	*	as well as querying of setup parameters
	*/
	
	class IService;
	class ServiceFactory;
	class MetadataReader;

	class PluginContext
	{

	public:

		/**
		*	service factory function
		*/
		typedef IService *const (*const ServiceCreator)(void);

		/**
		*	registers a service in the framework
		*/
		void registerService(std::string const& name, ServiceCreator creator);

		/**
		*	retrieves the value of a setup parameter from the framework
		*	name & type must match some setup parameter defined in plugin metadata
		*/
		template< typename T >
		void getSetupParameter(std::string const& name, T &param)
		{
			try
			{
				AbstractRef *ref = new ParamRef< T >(param);
				getSetupParameter(name, ref);
			}
			catch (Exception &e)
			{
				throw e;
			}
		}

	private:

		friend class Plugin;

		PluginContext(Plugin *const plugin, ServiceFactory *const factory);
		PluginContext(PluginContext const& src);
		PluginContext& operator=(PluginContext const& src);
		~PluginContext();

		void				getSetupParameter(std::string const& name, AbstractRef *const ref);

		Plugin				*m_Plugin;
		ServiceFactory		*m_Factory;

	};

}