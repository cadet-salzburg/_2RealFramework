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

		const std::list< std::string > load(std::string const& path);

		/**
		*	returns plugin metadata as string
		*/
		//const std::string getPluginInfo(Identifier const& plugin);

		//const Identifier loadPlugin();

		//template< typename DataType >
		//void setValue(Identifier const& id, std::string const& name, DataType const& value)
		//{
		//	EngineData data(value);
		//	setValueInternal(id, name, data);
		//}

		//template< typename DataType >
		//DataType const& getValue(Identifier const& id, std::string const& name) const
		//{
		//	Poco::SharedPtr< DataType > ptr = Extract< DataType >(getValueInternal(id, name));
		//	return *ptr.get();
		//}

	private:

		template< typename T >
		friend class SingletonHolder;

		Engine();
		Engine(Engine const& src);
		~Engine();

		/**
		*	internally used function for setting param values
		*/
		//void setValueInternal(Identifier const& id, std::string const& name, EngineData const& value);

		/**
		*	internally used function for setting param values
		*/
		//const EngineData getValueInternal(Identifier const& id, std::string const& name) const;

		//const bool isUnique(std::string const& name) const;

		//std::map< std::string, Identifier >		m_Names;

		EngineImpl								&m_Engine;

	};

}