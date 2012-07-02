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

#include "engine/_2RealSystem.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealSystemBlockManager.h"
#include "engine/_2RealLink.h"
#include "app/_2RealBlockHandle.h"
#include "engine/_2RealUberBlockBasedTrigger.h"

#include <assert.h>
#include <sstream>

namespace _2Real
{

	System::System( EngineImpl &engine ) :
		m_Engine( engine ),
		m_SubBlockManager( new SystemBlockManager() )
	{
	}

	void System::clearAll()
	{
		try
		{
			m_SubBlockManager->clearAll();
		}
		catch (TimeOutException &e)
		{
			std::cout << e.message() << std::endl;
		}

		//for ( app::ExceptionFunctionCallbacks::iterator it = m_ExceptionCallbacks.begin(); it != m_ExceptionCallbacks.end(); /**/ )
		//{
		//	delete *it;
		//	it = m_ExceptionCallbacks.erase( it );
		//}

		//for ( app::ExceptionCallbackHandlers::iterator it = m_ExceptionCallbackHandlers.begin(); it != m_ExceptionCallbackHandlers.end(); /**/ )
		//{
		//	delete *it;
		//	it = m_ExceptionCallbackHandlers.erase( it );
		//}
	}

	void System::clearBlockInstances()
	{
		try
		{
			m_SubBlockManager->clearBlockInstances();
		}
		catch (TimeOutException &e)
		{
			std::cout << e.message() << std::endl;
		}

		//for ( app::ExceptionFunctionCallbacks::iterator it = m_ExceptionCallbacks.begin(); it != m_ExceptionCallbacks.end(); ++it )
		//{
		//	delete *it;
		//}

		//for ( app::ExceptionCallbackHandlers::iterator it = m_ExceptionCallbackHandlers.begin(); it != m_ExceptionCallbackHandlers.end(); ++it )
		//{
		//	delete *it;
		//}
	}

	void System::addUberBlock( AbstractUberBlock &block, const bool isContext )
	{
		m_SubBlockManager->addBlock( block, isContext );
	}

	void System::handleException( FunctionBlock &block, Exception const& exception )
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

	void System::registerToException( app::ExceptionCallback callback, void *userData )
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

	void System::unregisterFromException( app::ExceptionCallback callback, void *userData )
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

	void System::registerToException( app::AbstractExceptionCallbackHandler &handler )
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

	void System::unregisterFromException( app::AbstractExceptionCallbackHandler &handler )
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

	void System::createLink( Inlet &inlet, Outlet &outlet )
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

	void System::destroyLink( Inlet &inlet, Outlet &outlet )
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