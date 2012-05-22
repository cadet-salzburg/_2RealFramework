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

#include "_2RealOwnedBlocks.h"
#include "_2RealException.h"
#include "_2RealAbstractUberBlock.h"

#include <sstream>

namespace _2Real
{

	OwnedAndUnordered::OwnedAndUnordered(AbstractBlock &owner) :
		AbstractBlockManager(owner)
	{
	}

	OwnedAndUnordered::~OwnedAndUnordered()
	{
	}

	void OwnedAndUnordered::clear()
	{
		BlockList ready;

		for (BlockList::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it)
		{
			(*it)->prepareForShutDown();
		}

		for (BlockList::iterator it = m_Blocks.begin(); it != m_Blocks.end(); /**/)
		{
			if ((*it)->shutDown())
			{
				ready.push_back(*it);
			}

			it = m_Blocks.erase(it);
		}

		for (BlockList::iterator it = ready.begin(); it != ready.end(); /**/)
		{
			delete *it;
			it = ready.erase(it);
		}
	}

	AbstractBlock & OwnedAndUnordered::getBlock(Identifier const& id)
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

	AbstractBlock const& OwnedAndUnordered::getBlock(Identifier const& id) const
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

	void OwnedAndUnordered::addBlock(AbstractBlock &block)
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

	void OwnedAndUnordered::removeBlock(AbstractBlock &block)
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

	std::list< Identifier > OwnedAndUnordered::getCurrentBlockIds() const
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