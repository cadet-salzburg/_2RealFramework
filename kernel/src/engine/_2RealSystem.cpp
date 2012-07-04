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
#include "helpers/_2RealException.h"

#include <assert.h>
#include <sstream>
#include <iostream>

namespace _2Real
{

	System::System( EngineImpl &engine ) :
		m_Engine( engine ),
		m_SubBlockManager( new SystemBlockManager() )
	{
	}

	System::~System()
	{
		clearAll();
		delete m_SubBlockManager;
	}

	void System::clearAll()
	{
		try
		{
			m_SubBlockManager->clearAll();
		}
		catch ( TimeOutException &e )
		{
			std::cout << e.message() << std::endl;
		}

		for ( IOLink::LinkIterator it = m_Links.begin(); it != m_Links.end(); /**/ )
		{
			delete *it;
			it = m_Links.erase( it );
		}

		m_ExceptionEvent.clear();
	}

	void System::clearBlockInstances()
	{
		try
		{
			m_SubBlockManager->clearBlockInstances();
		}
		catch ( TimeOutException &e )
		{
			std::cout << e.message() << std::endl;
		}

		for ( IOLink::LinkIterator it = m_Links.begin(); it != m_Links.end(); /**/ )
		{
			delete *it;
			it = m_Links.erase( it );
		}
	}

	void System::addUberBlock( AbstractUberBlock &block, const bool isContext )
	{
		m_SubBlockManager->addBlock( block, isContext );
	}

	void System::handleException( FunctionBlock &block, Exception const& exception )
	{
		m_ExceptionEvent.notify( std::make_pair( exception, app::BlockHandle( block ) ) );
	}

	void System::registerToException( app::ErrorCallback &callback )
	{
		m_ExceptionEvent.addListener( callback );
	}

	void System::unregisterFromException( app::ErrorCallback &callback )
	{
		m_ExceptionEvent.removeListener( callback );
	}

	void System::createLink( InletIO &inlet, OutletIO &outlet )
	{
		IOLink *link = new IOLink( inlet, outlet );
		IOLink::LinkIterator it = m_Links.find( link );
		if ( it == m_Links.end() )
		{
			link->activate();
			m_Links.insert( link );
		}
		else delete link;
	}

	void System::destroyLink( InletIO &inlet, OutletIO &outlet )
	{
		IOLink *link = new IOLink( inlet, outlet );
		IOLink::LinkIterator it = m_Links.find( link );
		if ( it != m_Links.end() )
		{
			link->deactivate();
			delete *it;
			m_Links.erase( it );
		}
		delete link;
	}
}