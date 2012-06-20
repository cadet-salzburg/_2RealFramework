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

#include "_2RealAbstractUberBlock.h"

namespace _2Real
{

	AbstractBlock::AbstractBlock( BlockIdentifier const& id, AbstractBlock *superBlock ) :
		m_Identifier( id ),
		m_SuperBlock( superBlock )
	{
	}

	AbstractBlock::~AbstractBlock()
	{
	}

	void AbstractBlock::lockSuperBlock()
	{
		m_SuperBlockAccess.lock();
	}

	void AbstractBlock::unlockSuperBlock()
	{
		m_SuperBlockAccess.unlock();
	}

	void AbstractBlock::setSuperBlock(AbstractBlock &superBlock)
	{
		m_SuperBlock = &superBlock;
	}

	AbstractBlock & AbstractBlock::getSuperBlock()
	{
		if (m_SuperBlock == nullptr)
		{
			return *this;
		}

		return *m_SuperBlock;
	}

	AbstractBlock const& AbstractBlock::getSuperBlock() const
	{
		if (m_SuperBlock == nullptr)
		{
			return *this;
		}

		return *m_SuperBlock;
	}

	BlockIdentifier const& AbstractBlock::getIdentifier() const
	{
		return m_Identifier;
	}

	std::string const& AbstractBlock::getName() const
	{
		return m_Identifier.getName();
	}

	unsigned int AbstractBlock::getId() const
	{
		return m_Identifier.getId();
	}

}