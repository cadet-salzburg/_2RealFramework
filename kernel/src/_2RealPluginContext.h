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

namespace _2Real
{

	class PluginContext
	{

		friend class Plugin;

	public:

		/**
		*	registers a service in the framework
		*/
		void registerService(std::string const& _name, ServiceCreator _creator);
		
		/**
		*	registers a singleton in the framework
		*/
		void registerSingleton(std::string const& _name, ServiceCreator _creator);
	
	private:

		/**
		*
		*/
		~PluginContext();

		/**
		*
		*/
		PluginContext(Plugin *const _plugin);
		
		/**
		*
		*/
		PluginContext(PluginContext const& _src);
		
		/**
		*
		*/
		PluginContext& operator=(PluginContext const& _src);

		/**
		*
		*/
		Plugin			*m_PluginPtr;

	};

}