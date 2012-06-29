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
#include "_2RealSystemBlockManager.h"
#include "_2RealLink.h"
#include "app/_2RealCallbacks.h"
#include "app/_2RealCallbacksInternal.h"
#include "app/_2RealBlockHandle.h"

#include <assert.h>
#include <sstream>

namespace _2Real
{

	SystemBlock::SystemBlock( EngineImpl &engine, BlockIdentifier const& id ) :
		UberBlock< DisabledIOManager, SystemBlockStateManager >( id ),
		m_Engine( engine ),
		m_SubBlockManager( new SystemBlockManager() )
	{
	}

	void SystemBlock::clear()
	{
		try
		{
			m_SubBlockManager->clear();
		}
		catch (TimeOutException &e)
		{
			std::cout << e.message() << std::endl;
		}

		for ( app::ExceptionFunctionCallbacks::iterator it = m_ExceptionCallbacks.begin(); it != m_ExceptionCallbacks.end(); ++it )
		{
			delete *it;
		}

		for ( app::ExceptionCallbackHandlers::iterator it = m_ExceptionCallbackHandlers.begin(); it != m_ExceptionCallbackHandlers.end(); ++it )
		{
			delete *it;
		}
	}

	void SystemBlock::addUberBlock( AbstractUberBlock &block )
	{
		if ( this == &block )
		{
#ifdef _DEBUG
			assert( NULL );
#endif
			return;
		}

		m_SubBlockManager->addBlock( block );

		// TODO: in reality, this is probably unneccesary
		// however, i want to keep this for later
		TriggerLink *link = new TriggerLink( *this, BLOCK_READY, block, BLOCK_OK );
	}

	void SystemBlock::handleException( FunctionBlock &block, Exception const& exception )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		for ( app::ExceptionCallbackHandlers::iterator it = m_ExceptionCallbackHandlers.begin(); it != m_ExceptionCallbackHandlers.end(); ++it )
		{
			( *it )->invoke( exception, app::BlockHandle( block ) );
		}

		for ( app::ExceptionFunctionCallbacks::iterator it = m_ExceptionCallbacks.begin(); it != m_ExceptionCallbacks.end(); ++it )
		{
			( *it )->invoke( exception, app::BlockHandle( block ) );
		}
	}

	void SystemBlock::registerToException( app::ExceptionCallback callback, void *userData )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		app::ExceptionFunctionCallback *cb = new app::ExceptionFunctionCallback( callback, userData );
		app::ExceptionFunctionCallbacks::iterator it = m_ExceptionCallbacks.find( cb );
		if ( it == m_ExceptionCallbacks.end() )
		{
			m_ExceptionCallbacks.insert( cb );
		}
		else
		{
			delete cb;
		}
	}

	void SystemBlock::unregisterFromException( app::ExceptionCallback callback, void *userData )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		app::ExceptionFunctionCallback *cb = new app::ExceptionFunctionCallback( callback, userData );
		app::ExceptionFunctionCallbacks::iterator it = m_ExceptionCallbacks.find( cb );
		if ( it != m_ExceptionCallbacks.end() )
		{
			delete *it;
			m_ExceptionCallbacks.erase(it);
		}

		delete cb;
	}

	void SystemBlock::registerToException( app::AbstractExceptionCallbackHandler &handler )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );

		app::ExceptionCallbackHandlers::iterator it = m_ExceptionCallbackHandlers.find( &handler );
		if ( it == m_ExceptionCallbackHandlers.end() )
		{
			m_ExceptionCallbackHandlers.insert( &handler );
		}
		else
		{
			delete &handler;
		}
	}

	void SystemBlock::unregisterFromException( app::AbstractExceptionCallbackHandler &handler )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_ExceptionAccess );
		app::ExceptionCallbackHandlers::iterator it = m_ExceptionCallbackHandlers.find( &handler );
		if ( it != m_ExceptionCallbackHandlers.end() )
		{
			delete *it;
			m_ExceptionCallbackHandlers.erase(it);
		}

		delete &handler;
	}

	void SystemBlock::createLink( Inlet &inlet, Outlet &outlet )
	{
		AbstractLink *link = new IOLink( inlet, outlet );
		LinkSet::iterator it = m_Links.find( link );
		if ( it == m_Links.end() )
		{
			link->activate();
			m_Links.insert( link );
		}
		else delete link;
	}

	void SystemBlock::destroyLink( Inlet &inlet, Outlet &outlet )
	{
		AbstractLink *link = new IOLink( inlet, outlet );
		LinkSet::iterator it = m_Links.find( link );
		if ( it != m_Links.end() )
		{
			link->deactivate();
			delete *it;
			m_Links.erase( it );
		}
		delete link;
	}

}