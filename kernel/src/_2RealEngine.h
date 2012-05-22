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

		/*
		*	sets the base directory, from where all plugins will be searched
		*/
		void setBaseDirectory(std::string const& directory);

		/*
		*	loads the dynamic lib from the given path
		*	if path is relative, it will be interpreted with respect to the base directory
		*	if lib is already loaded, returns identifier of bundle instance
		*/
		const Identifier load( std::string const& path );

		/**
		*	returns true if the library in question is already loaded
		*	if path is relative, it will be interpreted with respect to the base directory
		*/
		bool isLoaded(std::string const& libraryPath) const;

		/**
		*	returns plugin's metadata as string
		*/
		const std::string getInfoString( Identifier const& id );

	private:

		template< typename T >
		friend class SingletonHolder;

		Engine();
		Engine(Engine const& src);
		~Engine();

		EngineImpl								&m_EngineImpl;

	};

}