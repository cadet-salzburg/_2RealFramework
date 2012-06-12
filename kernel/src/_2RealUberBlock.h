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

#include "_2RealAbstractUberBlock.h"
#include "_2RealAbstractUberBlockManager.h"
#include "_2RealAbstractIOManager.h"
#include "_2RealAbstractStateManager.h"

namespace _2Real
{

	template< typename IOMgr, typename UberBlockMgr, typename SubBlockMgr, typename StateMgr >
	class UberBlock : public AbstractBlock
	{

	public:

		UberBlock(std::string const& name, AbstractBlock *super) :
			AbstractBlock(name, super),
			m_IOManager(new IOMgr(*this)),
			m_SubBlockManager(new SubBlockMgr(*this)),
			m_UberBlockManager(new UberBlockMgr(*this)),
			m_StateManager(new StateMgr(*this))
		{
		}

		virtual ~UberBlock()
		{
			delete m_StateManager;
			delete m_IOManager;
			delete m_SubBlockManager;
			delete m_UberBlockManager;
		}

		void addUberBlock(AbstractBlock &uberBlock)
		{
			m_UberBlockManager->addBlock(uberBlock);
		}
		
		void removeUberBlock(AbstractBlock &uberBlock)
		{
			m_UberBlockManager->removeBlock(uberBlock);
		}

		void addSubBlock(AbstractBlock &subBlock)
		{
			m_SubBlockManager->addBlock(subBlock);
		}

		void removeSubBlock(AbstractBlock &subBlock)
		{
			m_SubBlockManager->removeBlock(subBlock);
		}

		void registerToNewData(std::string const& outlet, OutletCallback callback, void *userData)
		{
			m_IOManager->registerToNewData(outlet, callback, userData);
		}

		void unregisterFromNewData(std::string const& outlet, OutletCallback callback, void *userData)
		{
			m_IOManager->unregisterFromNewData(outlet, callback, userData);
		}

		void registerToNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler)
		{
			m_IOManager->registerToNewData(outlet, handler);
		}

		void unregisterFromNewData(std::string const& outlet, AbstractOutletCallbackHandler &handler)
		{
			m_IOManager->unregisterFromNewData(outlet, handler);
		}

		void registerToNewData(OutputCallback callback, void *userData)
		{
			m_IOManager->registerToNewData(callback, userData);
		}

		void unregisterFromNewData(OutputCallback callback, void *userData)
		{
			m_IOManager->unregisterFromNewData(callback, userData);
		}

		void registerToNewData(AbstractOutputCallbackHandler &handler)
		{
			m_IOManager->registerToNewData(handler);
		}

		void unregisterFromNewData(AbstractOutputCallbackHandler &handler)
		{
			m_IOManager->unregisterFromNewData(handler);
		}

		const EngineData getValue(std::string const& paramName) const
		{
			return m_IOManager->getValue(paramName);
		}

		std::string const& getKey(std::string const& paramName) const
		{
			return m_IOManager->getKey(paramName);
		}

		void setValue(std::string const& paramName, TimestampedData const& value)
		{
			m_IOManager->setValue(paramName, value);
		}

		void insertValue(std::string const& paramName, TimestampedData &value)
		{
			m_IOManager->insertValue(paramName, value);
		}

		void linkWith(std::string const& nameIn, AbstractBlock &out, std::string const& nameOut)
		{
			m_IOManager->linkWith(nameIn, out, nameOut);
		}

		void setUp()
		{
			m_StateManager->setUp();
		}

		void prepareForShutDown()
		{
			m_StateManager->prepareForShutDown();
		}

		const bool shutDown()
		{
			return m_StateManager->shutDown();
		}

		AbstractStateManager& getStateManager()
		{
			return *m_StateManager;
		}

		AbstractBlockManager& getSubBlockManager()
		{
			return *m_SubBlockManager;
		}

		AbstractBlockManager& getUberBlockManager()
		{
			return *m_UberBlockManager;
		}

		AbstractIOManager& getIOManager()
		{
			return *m_IOManager;
		}

		AbstractStateManager const& getStateManager() const
		{
			return *m_StateManager;
		}

		AbstractBlockManager const& getSubBlockManager() const
		{
			return *m_SubBlockManager;
		}

		AbstractBlockManager const& getUberBlockManager() const
		{
			return *m_UberBlockManager;
		}

		AbstractIOManager const& getIOManager() const
		{
			return *m_IOManager;
		}

	protected:

		AbstractIOManager				*m_IOManager;
		AbstractBlockManager			*m_SubBlockManager;
		AbstractBlockManager			*m_UberBlockManager;
		AbstractStateManager			*m_StateManager;

	};

}