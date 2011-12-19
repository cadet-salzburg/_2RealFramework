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

#include <map>
#include <string>

namespace _2Real
{

	class Identifier;
	class SystemGraph;

	typedef std::pair< Identifier, SystemGraph * >	NamedSystemGraph;
	typedef std::map< Identifier, SystemGraph * >	SystemGraphTable;

	class SystemPool
	{

	public:

		SystemPool();
		~SystemPool();

		void					clearSystemGraphs();
		const Identifier		createSystemGraph(std::string const& name);
		void					destroySystemGraph(Identifier const& system);
		SystemGraph &			getSystemGraph(Identifier const& system);
		SystemGraph const&		getSystemGraph(Identifier const& system) const;

	private:

		SystemGraphTable::iterator			iteratorId(Identifier const& system);
		SystemGraphTable::const_iterator	iteratorId(Identifier const& system) const;

		SystemGraphTable		m_SystemGraphs;

	};

}