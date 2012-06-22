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
#include "_2RealException.h"
#include "_2RealServiceBlock.h"
#include "_2RealTimestampedData.h"
#include "_2RealUpdatePolicy.h"
#include "_2RealSyncBlock.h"
#include "_2RealUberBlockBasedTrigger.h"
#include "_2RealInletBasedTrigger.h"
#include "_2RealTimeBasedTrigger.h"
#include "_2RealUpdatePolicyImpl.h"
#include "_2RealBlockError.h"

#include <sstream>

namespace _2Real
{

	SystemImpl::SystemImpl( BlockIdentifier const& id ) :
		UberBlock< DisabledIO, DisabledBlocks, OwnedAndUnordered, SystemStates >( id, nullptr ),
		m_Engine( EngineImpl::instance() ),
		m_BundleManager( EngineImpl::instance().getPluginPool() )
	{
	}

	SystemImpl::~SystemImpl()
	{
		clear();
	}

	void SystemImpl::clear()
	{
		try
		{
			m_SubBlockManager->clear();
		}
		catch (TimeOutException &e)
		{
#ifdef _DEBUG
			std::cout << e.message() << std::endl;
#endif
		}

		for ( ExceptionFunctionCallbacks::iterator it = m_ExceptionCallbacks.begin(); it != m_ExceptionCallbacks.end(); /**/ )
		{
			delete *it;
			it = m_ExceptionCallbacks.erase( it );
		}

		for ( ExceptionCallbackHandlers::iterator it = m_ExceptionCallbackHandlers.begin(); it != m_ExceptionCallbackHandlers.end(); /**/ )
		{
			delete *it;
			it = m_ExceptionCallbackHandlers.erase( it );
		}
	}

	void SystemImpl::registerToNewData( BlockIdentifier const& id, std::string const& outlet, OutletCallback callback, void *userData )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_IOManager->registerToNewData( outlet, callback, userData );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock( id );
			obj.registerToNewData( outlet, callback, userData );
		}
	}

	void SystemImpl::unregisterFromNewData( BlockIdentifier const& id, std::string const& outlet, OutletCallback callback, void *userData )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_IOManager->unregisterFromNewData( outlet, callback, userData );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.unregisterFromNewData( outlet, callback, userData );
		}
	}

	void SystemImpl::registerToNewData( BlockIdentifier const& id, std::string const& outlet, AbstractOutletCallbackHandler &handler )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_IOManager->registerToNewData( outlet, handler );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock( id );
			obj.registerToNewData( outlet, handler );
		}
	}

	void SystemImpl::unregisterFromNewData( BlockIdentifier const& id, std::string const& outlet, AbstractOutletCallbackHandler &handler )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_IOManager->unregisterFromNewData( outlet, handler );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.unregisterFromNewData( outlet, handler );
		}
	}

	void SystemImpl::registerToNewData( BlockIdentifier const& id, OutputCallback callback, void *userData )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_IOManager->registerToNewData( callback, userData );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock( id );
			obj.registerToNewData( callback, userData );
		}
	}

	void SystemImpl::unregisterFromNewData( BlockIdentifier const& id, OutputCallback callback, void *userData )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_IOManager->unregisterFromNewData( callback, userData );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.unregisterFromNewData( callback, userData );
		}
	}

	void SystemImpl::registerToNewData( BlockIdentifier const& id, AbstractOutputCallbackHandler &handler )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_IOManager->registerToNewData( handler );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock( id );
			obj.registerToNewData( handler );
		}
	}

	void SystemImpl::unregisterFromNewData( BlockIdentifier const& id, AbstractOutputCallbackHandler &handler )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_IOManager->unregisterFromNewData( handler );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.unregisterFromNewData( handler );
		}
	}

	void SystemImpl::registerToException( ExceptionCallback callback, void *userData )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		ExceptionFunctionCallback *cb = new ExceptionFunctionCallback( callback, userData );
		ExceptionFunctionCallbacks::iterator it = m_ExceptionCallbacks.find( cb );
		if ( it == m_ExceptionCallbacks.end() )
		{
			m_ExceptionCallbacks.insert( cb );
		}
		else
		{
			delete cb;
		}
	}

	void SystemImpl::unregisterFromException( ExceptionCallback callback, void *userData )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		ExceptionFunctionCallback *cb = new ExceptionFunctionCallback( callback, userData );
		ExceptionFunctionCallbacks::iterator it = m_ExceptionCallbacks.find( cb );
		if ( it != m_ExceptionCallbacks.end() )
		{
			delete *it;
			m_ExceptionCallbacks.erase(it);
		}

		delete cb;
	}

	void SystemImpl::registerToException( AbstractExceptionCallbackHandler &handler )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		ExceptionCallbackHandlers::iterator it = m_ExceptionCallbackHandlers.find( &handler );
		if ( it == m_ExceptionCallbackHandlers.end() )
		{
			m_ExceptionCallbackHandlers.insert( &handler );
		}
		else
		{
			delete &handler;
		}
	}

	void SystemImpl::unregisterFromException( AbstractExceptionCallbackHandler &handler )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		ExceptionCallbackHandlers::iterator it = m_ExceptionCallbackHandlers.find( &handler );
		if ( it != m_ExceptionCallbackHandlers.end() )
		{
			delete *it;
			m_ExceptionCallbackHandlers.erase(it);
		}

		delete &handler;
	}

	void SystemImpl::handleException( AbstractBlock &subBlock, Exception &exception )
	{
		BlockError e( exception, subBlock.getIdentifier() );

		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		for ( ExceptionCallbackHandlers::iterator it = m_ExceptionCallbackHandlers.begin(); it != m_ExceptionCallbackHandlers.end(); ++it )
		{
			( *it )->invoke( e );
		}

		for ( ExceptionFunctionCallbacks::iterator it = m_ExceptionCallbacks.begin(); it != m_ExceptionCallbacks.end(); ++it )
		{
			( *it )->invoke( e );
		}
	}

	void SystemImpl::setUp( BlockIdentifier const& id )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system can not be set up" );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.setUp();
		}
	}

	void SystemImpl::start( BlockIdentifier const& id )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_StateManager->start();
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			dynamic_cast< SystemStates * >( m_StateManager )->setAllowedUpdates( obj, LONG_MAX );
			obj.start();
		}
	}

	void SystemImpl::singleStep( BlockIdentifier const& id )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_StateManager->stop();
			m_StateManager->start();
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.stop( true, LONG_MAX );	// blocks until the end of the world, basically
			dynamic_cast< SystemStates * >( m_StateManager )->setAllowedUpdates( obj, 1 );
			obj.start();
		}
	}

	void SystemImpl::stop( BlockIdentifier const& id, const long timeout )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			m_StateManager->stop();
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock( id );
			dynamic_cast< SystemStates * >( m_StateManager )->setAllowedUpdates( obj, 0 );
			obj.stop( true, timeout );
		}
	}

	void SystemImpl::destroy( BlockIdentifier const& id )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system can not be destroied" );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock( id );
			dynamic_cast< SystemStates * >( m_StateManager )->setAllowedUpdates( obj, 0 );
			m_SubBlockManager->removeBlock( obj );
		}
	}

	void SystemImpl::setValue( BlockIdentifier const& id, std::string const& paramName, EngineData const& value )
	{
		TimestampedData data( value, m_Engine.getElapsedTime() );

		if ( id == AbstractBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system has no i/o" );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.setValue(paramName, data);
		}
	}

	EngineData const& SystemImpl::getValue(BlockIdentifier const& id, std::string const& paramName) const
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system has no i/o" );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			return obj.getValue(paramName);
		}
	}

	std::string const& SystemImpl::getTypename(BlockIdentifier const& id, std::string const& paramName) const
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system has no i/o" );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			return obj.getTypename(paramName);
		}
	}

	std::string const& SystemImpl::getLongTypename(BlockIdentifier const& id, std::string const& paramName) const
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system has no i/o" );
		}
		else
		{
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			return obj.getLongTypename(paramName);
		}
	}

	void SystemImpl::link(BlockIdentifier const& in, std::string const& nameIn, BlockIdentifier const& out, std::string const& nameOut)
	{
		/**
		*	TODO
		*/
		AbstractBlock &objIn = m_SubBlockManager->getBlock(in);
		AbstractBlock &objOut = m_SubBlockManager->getBlock(out);
		objIn.linkWith(nameIn, objOut, nameOut);
	}

	const BlockIdentifier SystemImpl::createFunctionBlock( BundleIdentifier const& pluginId, std::string const& blockName )
	{
		FunctionBlock &block = m_BundleManager.createServiceBlock( pluginId, blockName, *this );
		addSubBlock( block );
		dynamic_cast< SystemStates * >( m_StateManager )->setAllowedUpdates( block, 0 );

		UpdatePolicy policy;
		policy.triggerByUpdateRate( 30.0f );
		UpdatePolicyImpl *p = new UpdatePolicyImpl( *policy.m_Impl );
		block.setUpdatePolicy( *p );

		AbstractStateManager &mgr = block.getStateManager();
		AbstractUberBlockBasedTrigger *trigger = new UberBlockBasedTrigger< BLOCK_OK >( this->getName() );
		mgr.uberBlockAdded( *this, *trigger );
		return block.getIdentifier();
	}

	void SystemImpl::setUpdatePolicy( BlockIdentifier const& id, UpdatePolicy const& policy )
	{
		if ( id == AbstractBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system has no update policy" );
		}
		else
		{
			UpdatePolicyImpl *p = new UpdatePolicyImpl( *policy.m_Impl );
			AbstractBlock &obj = m_SubBlockManager->getBlock(id);
			obj.setUpdatePolicy( *p );
		}
	}

	//const Identifier SystemImpl::createSyncBlock(std::list< Identifier > const& blockIds)
	//{
		//BlockList syncSet, readySet, finishedSet;
		//for (std::list< Identifier >::const_iterator it = blockIds.begin(); it != blockIds.end(); ++it)
		//{
		//	AbstractBlock *block = &m_SubBlockManager->getBlock(*it);
		//	syncSet.push_back(block);
		//}

		//SyncBlock *syncBlock = new SyncBlock("sync block", *this);
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

	//	return syncBlock->getIdentifier();
	//}

}