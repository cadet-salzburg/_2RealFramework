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

#include "_2RealUnownedBlockManager.h"
#include "_2RealException.h"
#include "_2RealAbstractUberBlock.h"

#include <sstream>

namespace _2Real
{

	UnownedBlockManager::UnownedBlockManager(AbstractUberBlock &owner) :
		AbstractBlockManager(owner)
	{
	}

	UnownedBlockManager::~UnownedBlockManager()
	{
	}

	void UnownedBlockManager::clear()
	{
		m_Blocks.clear();
	}

	AbstractUberBlock & UnownedBlockManager::getBlock( BlockIdentifier const& id )
	{
		for (std::list< AbstractUberBlock * >::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it)
		{
			if ( (*it)->getIdentifier() == id )
			{
				return **it;
			}
		}

		std::ostringstream msg;
		msg << "sub block " << id.getName() << " not found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	AbstractUberBlock const& UnownedBlockManager::getBlock(BlockIdentifier const& id) const
	{
		for (std::list< AbstractUberBlock * >::const_iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it)
		{
			if ((*it)->getIdentifier() == id)
			{
				return **it;
			}
		}

		std::ostringstream msg;
		msg << "sub block " << id.getName() << " not found in " << m_Owner.getName();
		throw NotFoundException(msg.str());
	}

	void UnownedBlockManager::addBlock(AbstractUberBlock &block)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Access);
		for (std::list< AbstractUberBlock * >::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it)
		{
			if ((*it) == &block)
			{
				//in case the block already is a sub block, fail silently
				return;
			}
		}
		
		m_Blocks.push_back(&block);
	}

	void UnownedBlockManager::removeBlock(AbstractUberBlock &block)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_Access);
		for (std::list< AbstractUberBlock * >::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it)
		{
			if (*it == &block)
			{
				it = m_Blocks.erase(it);
				break;
			}
		}

		//fail silently if not found
	}

}