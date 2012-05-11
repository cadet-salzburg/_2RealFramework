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

#include "_2RealUnOwnedBlocks.h"
#include "_2RealException.h"
#include "_2RealAbstractBlock.h"

#include <sstream>

namespace _2Real
{

	NotOwnedAndUnordered::NotOwnedAndUnordered(AbstractBlock &owner) :
		AbstractBlockManager(owner)
	{
	}

	NotOwnedAndUnordered::~NotOwnedAndUnordered()
	{
		clear();
	}

	void NotOwnedAndUnordered::clear()
	{
		m_Blocks.clear();
	}

	AbstractBlock & NotOwnedAndUnordered::getBlock(Identifier const& id)
	{
		for (std::list< AbstractBlock * >::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it)
		{
			if ((*it)->getIdentifier() == id)
			{
				return **it;
			}
		}

		std::ostringstream msg;
		msg << "sub block " << id << " not found in " << m_Owner.getIdentifier();
		throw NotFoundException(msg.str());
	}

	AbstractBlock const& NotOwnedAndUnordered::getBlock(Identifier const& id) const
	{
		for (std::list< AbstractBlock * >::const_iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it)
		{
			if ((*it)->getIdentifier() == id)
			{
				return **it;
			}
		}

		std::ostringstream msg;
		msg << "sub block " << id << " not found in " << m_Owner.getIdentifier();
		throw NotFoundException(msg.str());
	}

	void NotOwnedAndUnordered::addBlock(AbstractBlock &block)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Access);
		for (std::list< AbstractBlock * >::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it)
		{
			if ((*it) == &block)
			{
				//in case the block already is a sub block, fail silently
				return;
			}
		}
		
		m_Blocks.push_back(&block);
	}

	void NotOwnedAndUnordered::removeBlock(AbstractBlock &block)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Access);
		for (std::list< AbstractBlock * >::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it)
		{
			if (*it == &block)
			{
				it = m_Blocks.erase(it);
				break;
			}
		}

		//fail silently if not found
	}

	std::list< Identifier > NotOwnedAndUnordered::getCurrentBlockIds() const
	{
		std::list< Identifier > result;
		Poco::ScopedLock< Poco::FastMutex > lock(m_Access);
		for (std::list< AbstractBlock * >::const_iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it)
		{
			result.push_back((*it)->getIdentifier());
		}

		return result;
	}

}