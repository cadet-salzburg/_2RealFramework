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

#include "_2RealEngineData.h"

#include <map>
#include <string>

#include "Poco/SharedPtr.h"

namespace _2Real
{

	/**
	*	plugin's interface for communication with the framework
	*	allows export of services' factory functions
	*	as well as querying of setup parameters
	*/
	
	class IService;
	class ServiceFactory;

	typedef IService *const (*ServiceCreator)(void);

	class PluginContext
	{

	public:

		/**
		*	registers a service in the framework
		*	
		*	@param name:		name of service
		*	@param creator:		function used to create a service obj
		*	@throw:				InvalidServiceException
		*/
		void registerService(std::string const& name, ServiceCreator creator);

		/**
		*	returns the value of a setup parameter
		*	
		*	@param name:		name of a setup parameter
		*	@return:			constant reference to setup parameter
		*	@throw:				InvalidParameterException, DatatypeMismatchException
		*/
		template< typename Datatype >
		Datatype const& getParameterValue(std::string const& name)
		{
			Poco::SharedPtr< Datatype > ptr = Extract< Datatype >(getSetupParameter(name));
			return *ptr.get();
		}

	private:

		friend class Plugin;

		PluginContext(Plugin &plugin);
		PluginContext(PluginContext const& src);
		PluginContext& operator=(PluginContext const& src);

		EngineData			getSetupParameter(std::string const& name);

		Plugin				&m_Plugin;

	};

}