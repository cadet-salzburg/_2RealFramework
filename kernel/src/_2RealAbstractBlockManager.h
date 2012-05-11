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

#include <string>
#include <list>

namespace _2Real
{

	class Identifier;
	class AbstractBlock;

	class AbstractBlockManager
	{

	public:

		AbstractBlockManager(AbstractBlock &owner);
		virtual ~AbstractBlockManager();

		std::string const& getName() const;
		const unsigned int getId() const;

		virtual void						clear() = 0;
		virtual AbstractBlock &				getBlock(Identifier const& blockId) = 0;
		virtual AbstractBlock const&		getBlock(Identifier const& blockId) const = 0;
		virtual void						addBlock(AbstractBlock &block) = 0;
		virtual	void						removeBlock(AbstractBlock &block) = 0;
		virtual std::list< Identifier >		getCurrentBlockIds() const = 0;

	protected:

		AbstractBlock						&m_Owner;

	};

}