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
#include "engine/_2RealLogger.h"
#include "engine/_2RealFunctionBlock.h"
#include "helpers/_2RealException.h"

#include <string>
#include <assert.h>
#include <sstream>

using std::ostringstream;
using std::string;

namespace _2Real
{

	System::System( Logger &logger ) :
		m_Logger( logger )
	{
	}

	System::~System()
	{
		clearAll();
	}

	void System::clearAll()
	{
		Blocks readyBlocks;

		for ( BlockIterator it = m_BlockInstances.begin(); it != m_BlockInstances.end(); ++it )
		{
			(*it)->prepareForShutDown();
		}

		for ( BlockIterator it = m_ContextBlocks.begin(); it != m_ContextBlocks.end(); ++it )
		{
			(*it)->prepareForShutDown();
		}

		for ( BlockIterator it = m_BlockInstances.begin(); it != m_BlockInstances.end(); /**/ )
		{
			if ( (*it)->shutDown( 1000 ) )
			{
				readyBlocks.insert( *it );
			}
			else
			{
				m_Logger.addLine( string( "failed to shut down ").append( ( *it )->getFullName() ) );
			}

			it = m_BlockInstances.erase( it );
		}

		for ( BlockIterator it = m_ContextBlocks.begin(); it != m_ContextBlocks.end(); /**/ )
		{
			if ( (*it)->shutDown( 1000 ) )
			{
				readyBlocks.insert( *it );
			}
			else
			{
				m_Logger.addLine( string( "failed to shut down ").append( ( *it )->getFullName() ) );
			}

			it = m_ContextBlocks.erase( it );
		}

		for ( BlockIterator it = readyBlocks.begin(); it != readyBlocks.end(); /**/ )
		{
			delete *it;
			it = readyBlocks.erase( it );
		}
	}

	void System::clearBlockInstances()
	{
		Blocks ready;

		for ( BlockIterator it = m_BlockInstances.begin(); it != m_BlockInstances.end(); ++it )
		{
			(*it)->prepareForShutDown();
		}

		for ( BlockIterator it = m_BlockInstances.begin(); it != m_BlockInstances.end(); /**/ )
		{
			if ( (*it)->shutDown( 1000 ) )
			{
				ready.insert( *it );
			}
			else
			{
				m_Logger.addLine( string( "failed to shut down ").append( ( *it )->getFullName() ) );
			}

			it = m_BlockInstances.erase( it );
		}

		for ( BlockIterator it = ready.begin(); it != ready.end(); /**/ )
		{
			delete *it;
			it = ready.erase( it );
		}
	}

	void System::addBlock( FunctionBlock< app::ContextBlockHandle > &block )
	{
		m_ContextBlocks.insert( &block );
	}

	void System::addBlock( FunctionBlock< app::BlockHandle > &block )
	{
		m_BlockInstances.insert( &block );
	}

	void System::removeBlock( FunctionBlock< app::BlockHandle > &block, const long timeout )
	{
		for ( BlockIterator it = m_BlockInstances.begin(); it != m_BlockInstances.end(); ++it )
		{
			if ( *it == &block )
			{
				( *it )->prepareForShutDown();
				if ( (*it)->shutDown( timeout ) )
				{
					delete *it;
					m_BlockInstances.erase( it );
					return;
				}
				else
				{
					m_BlockInstances.erase( it );
					ostringstream msg;
					msg << " timeout reached on shutdown of " << block.getFullName();
					throw TimeOutException( msg.str() );
				}
			}
		}
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void System::removeBlock( FunctionBlock< app::ContextBlockHandle > &block, const long timeout )
	{
		for ( BlockIterator it = m_ContextBlocks.begin(); it != m_ContextBlocks.end(); ++it )
		{
			if ( *it == &block )
			{
				( *it )->prepareForShutDown();
				if ( (*it)->shutDown( timeout ) )
				{
					delete *it;
					m_ContextBlocks.erase( it );
					return;
				}
				else
				{
					m_ContextBlocks.erase( it );
					ostringstream msg;
					throw TimeOutException( msg.str() );
				}
			}
		}
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	System::Blocks const& System::getBlockInstances() const
	{
		return m_BlockInstances;
	}

	System::Blocks const& System::getBundleContexts() const
	{
		return m_ContextBlocks;
	}

}