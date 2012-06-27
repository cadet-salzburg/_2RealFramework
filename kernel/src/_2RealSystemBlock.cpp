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

#include "_2RealSystemBlock.h"
#include "_2RealEngineImpl.h"
#include "_2RealBundleManager.h"
#include "_2RealThreadPool.h"
#include "_2RealException.h"
#include "_2RealFunctionBlock.h"
#include "_2RealTimestampedData.h"
#include "_2RealUberBlockBasedTrigger.h"
#include "_2RealInletBasedTrigger.h"
#include "_2RealTimeBasedTrigger.h"
#include "_2RealBlockError.h"
#include "_2RealLink.h"
#include "_2RealSystemBlockStateManager.h"

#include <sstream>

namespace _2Real
{

	SystemBlock::SystemBlock( BlockIdentifier const& id ) :
		UberBlock< DisabledIOManager, DisabledBlockManager, SystemBlockManager, SystemBlockStateManager >( id ),
		m_Engine( EngineImpl::instance() ),
		m_BundleManager( EngineImpl::instance().getPluginPool() ),
		m_StateManager( dynamic_cast< SystemBlockStateManager * >( UberBlock::m_StateManager ) )
	{
	}

	SystemBlock::~SystemBlock()
	{
		clear();
	}

	void SystemBlock::clear()
	{
		try
		{
			m_SubBlockManager->clear();

			std::cout << "all sub blocks cleared!" << std::endl;
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

	void SystemBlock::registerToNewData( BlockIdentifier const& id, std::string const& outlet, OutletCallback callback, void *userData )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_IOManager->registerToNewData( outlet, callback, userData );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock( id );
			obj.registerToNewData( outlet, callback, userData );
		}
	}

	void SystemBlock::unregisterFromNewData( BlockIdentifier const& id, std::string const& outlet, OutletCallback callback, void *userData )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_IOManager->unregisterFromNewData( outlet, callback, userData );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			obj.unregisterFromNewData( outlet, callback, userData );
		}
	}

	void SystemBlock::registerToNewData( BlockIdentifier const& id, std::string const& outlet, AbstractOutletCallbackHandler &handler )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_IOManager->registerToNewData( outlet, handler );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock( id );
			obj.registerToNewData( outlet, handler );
		}
	}

	void SystemBlock::unregisterFromNewData( BlockIdentifier const& id, std::string const& outlet, AbstractOutletCallbackHandler &handler )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_IOManager->unregisterFromNewData( outlet, handler );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			obj.unregisterFromNewData( outlet, handler );
		}
	}

	void SystemBlock::registerToNewData( BlockIdentifier const& id, OutputCallback callback, void *userData )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_IOManager->registerToNewData( callback, userData );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock( id );
			obj.registerToNewData( callback, userData );
		}
	}

	void SystemBlock::unregisterFromNewData( BlockIdentifier const& id, OutputCallback callback, void *userData )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_IOManager->unregisterFromNewData( callback, userData );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			obj.unregisterFromNewData( callback, userData );
		}
	}

	void SystemBlock::registerToNewData( BlockIdentifier const& id, AbstractOutputCallbackHandler &handler )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_IOManager->registerToNewData( handler );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock( id );
			obj.registerToNewData( handler );
		}
	}

	void SystemBlock::unregisterFromNewData( BlockIdentifier const& id, AbstractOutputCallbackHandler &handler )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_IOManager->unregisterFromNewData( handler );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			obj.unregisterFromNewData( handler );
		}
	}

	void SystemBlock::registerToException( ExceptionCallback callback, void *userData )
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

	void SystemBlock::unregisterFromException( ExceptionCallback callback, void *userData )
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

	void SystemBlock::registerToException( AbstractExceptionCallbackHandler &handler )
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

	void SystemBlock::unregisterFromException( AbstractExceptionCallbackHandler &handler )
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

	void SystemBlock::handleException( AbstractUberBlock &subBlock, Exception &exception )
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

	void SystemBlock::setUp( BlockIdentifier const& id )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system can not be set up" );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			obj.setUp();
		}
	}

	void SystemBlock::start( BlockIdentifier const& id )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_StateManager->start();
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			obj.start();
		}
	}

	void SystemBlock::singleStep( BlockIdentifier const& id )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_StateManager->stop();
			m_StateManager->start();
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			obj.stop( true, LONG_MAX );	// blocks until the end of the world, basically
			obj.start();
		}
	}

	void SystemBlock::stop( BlockIdentifier const& id, const long timeout )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			m_StateManager->stop();
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock( id );
			obj.stop( true, timeout );
		}
	}

	void SystemBlock::destroy( BlockIdentifier const& id )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system can not be destroied" );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock( id );
			m_SubBlockManager->removeBlock( obj );
		}
	}

	void SystemBlock::setValue( BlockIdentifier const& id, std::string const& paramName, EngineData const& value )
	{
		TimestampedData data( value, m_Engine.getElapsedTime() );

		if ( id == AbstractUberBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system has no i/o" );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			obj.setValue(paramName, data);
		}
	}

	EngineData const& SystemBlock::getValue(BlockIdentifier const& id, std::string const& paramName) const
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system has no i/o" );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			return obj.getValue(paramName);
		}
	}

	std::string const& SystemBlock::getTypename(BlockIdentifier const& id, std::string const& paramName) const
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system has no i/o" );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			return obj.getTypename(paramName);
		}
	}

	std::string const& SystemBlock::getLongTypename(BlockIdentifier const& id, std::string const& paramName) const
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system has no i/o" );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			return obj.getLongTypename(paramName);
		}
	}

	UpdatePolicyHandle SystemBlock::getUpdatePolicy( BlockIdentifier const& id )
	{
		if ( id == AbstractUberBlock::getIdentifier() )
		{
			throw InvalidOperationException( "system has no update policy" );
		}
		else
		{
			AbstractUberBlock &obj = m_SubBlockManager->getBlock(id);
			return obj.getUpdatePolicyHandle();
		}
	}

	void SystemBlock::link( BlockIdentifier const& in, std::string const& nameIn, BlockIdentifier const& out, std::string const& nameOut )
	{
		AbstractUberBlock &blockIn = m_SubBlockManager->getBlock( in );
		AbstractUberBlock &blockOut = m_SubBlockManager->getBlock( out );

		Inlet &inlet = blockIn.getInlet( nameIn );
		Outlet &outlet = blockOut.getOutlet( nameOut );

		AbstractLink *link = new IOLink( inlet, outlet );		// but, who takes care of the links?
		link->activate();
		m_Links.insert( link );
	}

	void SystemBlock::unlink( BlockIdentifier const& in, std::string const& nameIn, BlockIdentifier const& out, std::string const& nameOut )
	{
		AbstractUberBlock &blockIn = m_SubBlockManager->getBlock( in );
		AbstractUberBlock &blockOut = m_SubBlockManager->getBlock( out );

		Inlet &inlet = blockIn.getInlet( nameIn );
		Outlet &outlet = blockOut.getOutlet( nameOut );

		AbstractLink *link = new IOLink( inlet, outlet );
		LinkSet::iterator it = m_Links.find( link );
		if ( it != m_Links.end() )
		{
			( *it )->deactivate();
			delete *it;
			m_Links.erase( *it );
		}
	}

	const BlockIdentifier SystemBlock::createFunctionBlock( BundleIdentifier const& pluginId, std::string const& blockName )
	{
		// this currently works b/c i know exactely what triggers to create oO

		FunctionBlock &block = m_BundleManager.createServiceBlock( pluginId, blockName, *this );

		AbstractUberBlockBasedTrigger *s = this->createSubBlockTrigger();
		AbstractUberBlockBasedTrigger *f = block.createSuperBlockTrigger();

		s->setOther( *f );
		f->setOther( *s );

		this->addSubBlock( block, f );
		block.addSuperBlock( *this, s );

		return block.getIdentifier();
	}

	AbstractUberBlockBasedTrigger * SystemBlock::createSubBlockTrigger()
	{
		return new UberBlockBasedTrigger< SystemBlockStateManager >( *m_StateManager, &SystemBlockStateManager::tryTriggerUberBlock, BLOCK_READY );
	}

	AbstractUberBlockBasedTrigger * SystemBlock::createSuperBlockTrigger()
	{
		return nullptr;
	}

}