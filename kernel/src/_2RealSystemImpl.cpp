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

#include "_2RealSystemImpl.h"
#include "_2RealEngineImpl.h"
#include "_2RealBundleManager.h"
#include "_2RealThreadPool.h"
#include "_2RealExceptionHandler.h"
#include "_2RealException.h"
#include "_2RealServiceBlock.h"
#include "_2RealData.h"
#include "_2RealUpdatePolicy.h"
#include "_2RealSyncBlock.h"
#include "_2RealTriggerTypes.h"
#include "_2RealUpdatePolicyImpl.h"
#include "_2RealMetadata.h"

#include <sstream>

namespace _2Real
{

	SystemImpl::SystemImpl(std::string const& name) :
		UberBlock< DisabledIO, DisabledBlocks, OwnedAndUnordered, DisabledStates/*, SystemUpdates */>(name, NULL),
		m_Engine(EngineImpl::instance()),
		m_PluginPool(EngineImpl::instance().getPluginPool()),
		m_Timestamp(),
		m_ExceptionHandler()
	{
		m_Timestamp.update();
	}

	SystemImpl::~SystemImpl()
	{
		clear();
	}

	const long SystemImpl::getElapsedTime() const
	{
		return static_cast< long >(m_Timestamp.elapsed());
	}

	Poco::Timestamp const& SystemImpl::getTimestamp() const
	{
		return m_Timestamp;
	}

	void SystemImpl::clear()
	{
		try
		{
			m_SubBlockManager->clear();
		}
		catch (TimeOutException &e)
		{
			std::cout << e.message() << std::endl;
		}
	}

	void SystemImpl::registerToNewData( Identifier const& id, std::string const& outlet, DataCallback callback, void *userData )
	{
		if (id == Entity::getIdentifier())
		{
			m_IOManager->registerToNewData( outlet, callback, userData );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock( id );
			obj.registerToNewData( outlet, callback, userData );
		}
	}

	void SystemImpl::unregisterFromNewData( Identifier const& id, std::string const& outlet, DataCallback callback, void *userData )
	{
		if (id == Entity::getIdentifier())
		{
			m_IOManager->unregisterFromNewData( outlet, callback, userData );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.unregisterFromNewData( outlet, callback, userData );
		}
	}

	void SystemImpl::registerToNewData( Identifier const& id, std::string const& outlet, AbstractDataCallbackHandler &handler )
	{
		if (id == Entity::getIdentifier())
		{
			m_IOManager->registerToNewData( outlet, handler );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock( id );
			obj.registerToNewData( outlet, handler );
		}
	}

	void SystemImpl::unregisterFromNewData( Identifier const& id, std::string const& outlet, AbstractDataCallbackHandler &handler )
	{
		if (id == Entity::getIdentifier())
		{
			m_IOManager->unregisterFromNewData( outlet, handler );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.unregisterFromNewData( outlet, handler );
		}
	}

	void SystemImpl::setUp(Identifier const& id)
	{
		if (id == Entity::getIdentifier())
		{
			m_StateManager->setUp();
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.setUp();
		}
	}

	void SystemImpl::setValue(Identifier const& id, std::string const& paramName, EngineData const& value)
	{
		Data data(value, (long)m_Timestamp.elapsed());

		if (id == Entity::getIdentifier())
		{
			m_IOManager->setValue(paramName, data);
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.setValue(paramName, data);
		}
	}

	void SystemImpl::insertValue(Identifier const& id, std::string const& paramName, EngineData const& value)
	{
		Data data(value, (long)m_Timestamp.elapsed());

		if (id == Entity::getIdentifier())
		{
			m_IOManager->insertValue(paramName, data);
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.insertValue(paramName, data);
		}
	}

	const EngineData SystemImpl::getValue(Identifier const& id, std::string const& paramName) const
	{
		if (id == Entity::getIdentifier())
		{
			return m_IOManager->getValue(paramName);
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			return obj.getValue(paramName);
		}
	}

	std::string const& SystemImpl::getKey(Identifier const& id, std::string const& paramName) const
	{
		if (id == Entity::getIdentifier())
		{
			return m_IOManager->getKey(paramName);
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			return obj.getKey(paramName);
		}
	}

	void SystemImpl::link(Identifier const& in, std::string const& nameIn, Identifier const& out, std::string const& nameOut)
	{
		/**
		*	TODO
		*/
		AbstractBlock &objIn = m_SubBlockManager->getBlock(in);
		AbstractBlock &objOut = m_SubBlockManager->getBlock(out);
		objIn.linkWith(nameIn, objOut, nameOut);
	}

	const Identifier SystemImpl::createServiceBlock( Identifier const& pluginId, std::string const& blockName, UpdatePolicy const& triggers )
	{
		ServiceBlock &block = m_PluginPool.createServiceBlock( pluginId, blockName, *this, *triggers.m_Impl );
		addSubBlock( block );
		return block.getIdentifier();
	}

	const Identifier SystemImpl::createSyncBlock(std::list< Identifier > const& blockIds)
	{
		//BlockList syncSet, readySet, finishedSet;
		//for (std::list< Identifier >::const_iterator it = blockIds.begin(); it != blockIds.end(); ++it)
		//{
		//	AbstractBlock *block = &m_SubBlockManager->getBlock(*it);
		//	syncSet.push_back(block);
		//}

		SyncBlock *syncBlock = new SyncBlock("sync block", *this);
		//this->addSubBlock(*syncBlock);

		//AbstractStateManager &triggerMgr = syncBlock->getStateManager();
		//AbstractBlockManager &blockMgr = syncBlock->getSubBlockManager();
		//for (BlockList::iterator it = syncSet.begin(); it != syncSet.end(); ++it)
		//{
		//	AbstractBlock &b = **it;
		//	AbstractStateManager &tMgr = b.getStateManager();
		//	AbstractBlockManager &bMgr = b.getUberBlockManager();

		//	AbstractBlockBasedTrigger *readyTrigger = new BlockBasedTrigger< BLOCK_READY >(b.getName());
		//	AbstractBlockBasedTrigger *finishedTrigger = new BlockBasedTrigger< BLOCK_FINISHED >(b.getName());
		//	AbstractBlockBasedTrigger *uberTrigger = new BlockBasedTrigger< BLOCK_OK >("sync block");
		//	blockMgr.addBlock(b);
		//	bMgr.addBlock(*syncBlock);
		//	triggerMgr.subBlockAdded(b, *readyTrigger, BLOCK_READY);
		//	triggerMgr.subBlockAdded(b, *finishedTrigger, BLOCK_FINISHED);
		//	tMgr.uberBlockAdded(*syncBlock, *uberTrigger, BLOCK_OK);
		//}

		////causes the sync block to start waiting for ready messages
		//syncBlock->setUp();

		return syncBlock->getIdentifier();
	}

}