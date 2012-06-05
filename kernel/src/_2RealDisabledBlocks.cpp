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

#include "_2RealDisabledBlocks.h"
#include "_2RealException.h"
#include "_2RealAbstractUberBlock.h"

#include <sstream>

namespace _2Real
{

	DisabledBlocks::DisabledBlocks(AbstractBlock &owner) :
		AbstractBlockManager(owner)
	{
	}

	DisabledBlocks::~DisabledBlocks()
	{
		clear();
	}

	void DisabledBlocks::clear()
	{
	}

	AbstractBlock & DisabledBlocks::getBlock(BlockIdentifier const& id)
	{
		std::ostringstream msg;
		msg << "block " << id.getName() << " not found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	AbstractBlock const& DisabledBlocks::getBlock(BlockIdentifier const& id) const
	{
		std::ostringstream msg;
		msg << "block " << id.getName() << " not found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	void DisabledBlocks::addBlock(AbstractBlock &block)
	{
	}

	void DisabledBlocks::removeBlock(AbstractBlock &block)
	{
	}

	std::list< BlockIdentifier > DisabledBlocks::getCurrentBlockIds() const
	{
		return std::list< BlockIdentifier >();
	}

}