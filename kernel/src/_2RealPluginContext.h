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

#include <string>

namespace _2Real
{

	/**
	*	plugin's interface for communication with the framework
	*	allows export of services' factory functions
	*	as well as querying of setup parameters
	*/

	class Plugin;

	class IService;
	typedef Poco::SharedPtr< IService > (*ServiceCreator)(void);

//#define _2REAL_REGISTER_SERVICE(context, name, classname)

	class AbstractServiceObject
	{

	public:

		virtual IService * create() = 0;
		virtual ~AbstractServiceObject() {};

	};

	template< typename T >
	class ServiceObject : public AbstractServiceObject
	{

	private:

		IService *create()
		{
			return new T();
		}

	};

	class PluginContext
	{

	public:

		PluginContext(Plugin &plugin);

		/**
		*	registers a service in the framework
		*/
		template< typename T >
		void registerService(std::string const& serviceName)
		{
			registerServiceInternal(serviceName, new ServiceObject< T >());
		}

		/**
		*	returns the value of a setup parameter
		*/
		template< typename DataType >
		DataType const& getParameterValue(std::string const& name) const
		{
			EngineData const& data = getData(name);
			Poco::SharedPtr< DataType > const& ptr = Extract< DataType >(data);
			return *ptr.get();
		}

	private:

		void registerServiceInternal(std::string const& serviceName, AbstractServiceObject *obj);

		EngineData const&	getData(std::string const& name) const;

		Plugin				&m_Plugin;

	};

}