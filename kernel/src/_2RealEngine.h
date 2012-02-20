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

	class Identifier;
	class EngineImpl;

	class Engine
	{

	public:

		static Engine& instance();

		void setBaseDirectory(std::string const& directory);

		/*
		*	loads the dynamic from the given path, returns a list of strings with all the exported classes
		*	if path is relative, it will be interpreted with respect to the base directory
		*	TODO:
		*	if path points to a directory, all dynamic libs inside that directory will be loaded
		*	exceptions: invalid path, lib not found
		*/
		const std::list< std::string > load(std::string const& path);

		/**
		*	returns true if the library in question is already loaded
		*/
		const bool isLoaded(std::string const& libraryPath) const;

		/**
		*	returns plugin metadata as string
		*/
		const std::string getInfoString(std::string const& classname, std::string const& libraryPath = "");

		/**
		*	returns true if instances of the class can be created
		*	(false if no class has been loaded OR it's a singleton)
		*	path is used to optionally define the library
		*/
		const bool canCreate(std::string const& className, std::string const& libraryPath = "") const;

		/**
		*	returns true if className has been loaded & it's a singleton
		*/
		const bool isSingleton(std::string const& className, std::string const& libraryPath = "") const;

		/**
		*	creates instance of a plugin
		*/
		const Identifier create(std::string const& idName, std::string const& className, std::string const& libraryPath = "");

		/*
		*	returns instance of singleton plugin
		*	idName is used only at the first call, after that the existing identifier is returned
		*/
		const Identifier instance(std::string const& idName, std::string const& className, std::string const& libraryPath = "");

		const bool isSetUp(Identifier const& pluginId) const;

		/**
		*	sets the value of a plugin's setup parameter
		*	does nothing if the plugin in question is already set up!
		*/
		template< typename DataType >
		void setValue(Identifier const& pluginId, std::string const& paramName, DataType const& value)
		{
			EngineData data(value);
			setValueInternal(pluginId, paramName, data);
		}

		template< typename DataType >
		DataType const& getValue(Identifier const& id, std::string const& name) const
		{
			Poco::SharedPtr< DataType > ptr = Extract< DataType >(getValueInternal(id, name));
			return *ptr.get();
		}

	private:

		template< typename T >
		friend class SingletonHolder;

		Engine();
		Engine(Engine const& src);
		~Engine();

		/**
		*	internally used function for setting param values
		*/
		void setValueInternal(Identifier const& id, std::string const& name, EngineData const& value);

		/**
		*	internally used function for setting param values
		*/
		const EngineData getValueInternal(Identifier const& id, std::string const& name) const;

		//const bool isUnique(std::string const& name) const;

		//std::map< std::string, Identifier >		m_Names;

		EngineImpl								&m_Engine;

	};

}