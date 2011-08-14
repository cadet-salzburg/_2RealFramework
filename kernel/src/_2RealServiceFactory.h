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

#include <string>
#include <map>

namespace _2Real
{

	/**
	*
	*/

	class Plugin;

	class ServiceFactory
	{

	public:

		//ServiceFactory();

		/**
		*	registers factory function of a service
		*/
		void registerService(std::string const& _name, Plugin *const _pluginPtr, ServiceCreator _creator, bool const& _singleton);

		/**
		*	returns true if service in question can be created
		*/
		const bool canCreate(std::string const& _name, std::string const& _plugin) const;

		/**
		*	returns true if the service in question is a singleton
		*/
		const bool isSingleton(std::string const& _name, std::string const& _plugin) const;

		/**
		*	creates instance of service - or retirns singleton instance
		*/
		IService *const createService(std::string const& _name, std::string const& _plugin);

	private:

	};

}