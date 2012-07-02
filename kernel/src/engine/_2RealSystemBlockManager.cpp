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

#include "engine/_2RealSystemBlockManager.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealAbstractUberBlock.h"

#include <iostream>
#include <sstream>
#include <assert.h>

using std::cout;
using std::endl;
using std::ostringstream;
using std::list;

namespace _2Real
{

	SystemBlockManager::~SystemBlockManager()
	{
		clearAll();
	}

	void SystemBlockManager::clearAll()
	{
		BlockList ready;

		for ( BlockList::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it )
		{
			(*it)->prepareForShutDown();
		}

		for ( BlockList::iterator it = m_ContextBlocks.begin(); it != m_ContextBlocks.end(); ++it )
		{
			(*it)->prepareForShutDown();
		}

		for ( BlockList::iterator it = m_Blocks.begin(); it != m_Blocks.end(); /**/ )
		{
			if ( (*it)->shutDown( 1000 ) )
			{
				ready.push_back( *it );
			}
			else
			{
#ifdef _DEBUG
				cout << "failed to shut down " << ( *it )->getName() << endl;
#endif
			}

			it = m_Blocks.erase( it );
		}

		for ( BlockList::iterator it = m_ContextBlocks.begin(); it != m_ContextBlocks.end(); /**/ )
		{
			if ( (*it)->shutDown( 1000 ) )
			{
				ready.push_back( *it );
			}
			else
			{
#ifdef _DEBUG
				cout << "failed to shut down " << ( *it )->getName() << endl;
#endif
			}

			it = m_ContextBlocks.erase( it );
		}

		for ( BlockList::iterator it = ready.begin(); it != ready.end(); /**/ )
		{
			delete *it;
			it = ready.erase( it );
		}
	}

	
	void SystemBlockManager::clearBlockInstances()
	{
		BlockList ready;

		for ( BlockList::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it )
		{
			(*it)->prepareForShutDown();
		}

		for ( BlockList::iterator it = m_Blocks.begin(); it != m_Blocks.end(); /**/ )
		{
			if ( (*it)->shutDown( 1000 ) )
			{
				ready.push_back( *it );
			}
			else
			{
#ifdef _DEBUG
				cout << "failed to shut down " << ( *it )->getName() << endl;
#endif
			}

			it = m_Blocks.erase( it );
		}

		for ( BlockList::iterator it = ready.begin(); it != ready.end(); /**/ )
		{
			delete *it;
			it = ready.erase( it );
		}
	}

	void SystemBlockManager::addBlock( AbstractUberBlock &block, const bool isContext )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_BlockAccess );

		if ( !isContext )
		{
			for ( BlockList::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it )
			{
				if ( ( *it ) == &block )
				{
	#ifdef _DEBUG
			assert( NULL );
	#endif
					return;
				}
			}
			m_Blocks.push_back( &block );
		}
		else
		{
			for ( BlockList::iterator it = m_ContextBlocks.begin(); it != m_ContextBlocks.end(); ++it )
			{
				if ( ( *it ) == &block )
				{
	#ifdef _DEBUG
			assert( NULL );
	#endif
					return;
				}
			}
			m_ContextBlocks.push_back( &block );
		}
	}

	void SystemBlockManager::destroyBlock( AbstractUberBlock &block )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_BlockAccess );
		for ( BlockList::iterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it )
		{
			if ( *it == &block )
			{
				( *it )->prepareForShutDown();
				if ( (*it)->shutDown( 5000 ) )
				{
					delete *it;
				}

				m_Blocks.erase( it );
				return;
			}
		}
#ifdef _DEBUG
		assert( NULL );
#endif
	}

}