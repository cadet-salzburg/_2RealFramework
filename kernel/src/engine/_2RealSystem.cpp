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
#include "engine/_2RealAbstractUberBlock.h"
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
		clearFully();
	}

	void System::clearFully()
	{
		Blocks ready;

		for ( BlockIterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it )
		{
			(*it)->prepareForShutDown();
		}

		for ( BlockIterator it = m_ContextBlocks.begin(); it != m_ContextBlocks.end(); ++it )
		{
			(*it)->prepareForShutDown();
		}

		for ( BlockIterator it = m_Blocks.begin(); it != m_Blocks.end(); /**/ )
		{
			if ( (*it)->shutDown( 1000 ) )
			{
				ready.push_back( *it );
			}
			else
			{
				m_Logger.addLine( string( "failed to shut down ").append( ( *it )->getName() ) );
			}

			it = m_Blocks.erase( it );
		}

		for ( BlockIterator it = m_ContextBlocks.begin(); it != m_ContextBlocks.end(); /**/ )
		{
			if ( (*it)->shutDown( 1000 ) )
			{
				ready.push_back( *it );
			}
			else
			{
				m_Logger.addLine( string( "failed to shut down ").append( ( *it )->getName() ) );
			}

			it = m_ContextBlocks.erase( it );
		}

		for ( BlockIterator it = ready.begin(); it != ready.end(); /**/ )
		{
			delete *it;
			it = ready.erase( it );
		}
	}

	void System::clearBlocksOnly()
	{
		Blocks ready;

		for ( BlockIterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it )
		{
			(*it)->prepareForShutDown();
		}

		for ( BlockIterator it = m_Blocks.begin(); it != m_Blocks.end(); /**/ )
		{
			if ( (*it)->shutDown( 1000 ) )
			{
				ready.push_back( *it );
			}
			else
			{
				m_Logger.addLine( string( "failed to shut down ").append( ( *it )->getName() ) );
			}

			it = m_Blocks.erase( it );
		}

		for ( BlockIterator it = ready.begin(); it != ready.end(); /**/ )
		{
			delete *it;
			it = ready.erase( it );
		}
	}

	void System::addContextBlock( AbstractUberBlock &context )
	{
#ifdef _DEBUG
		for ( BlockIterator it = m_ContextBlocks.begin(); it != m_ContextBlocks.end(); ++it )
		{
			if ( ( *it ) == &context ) assert( NULL );
		}
#endif
		m_ContextBlocks.push_back( &context );
	}

	void System::addBlockInstance( AbstractUberBlock &block )
	{
#ifdef _DEBUG
		for ( BlockIterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it )
		{
			if ( ( *it ) == &block ) assert( NULL );
		}
#endif
		m_Blocks.push_back( &block );
	}

	void System::removeBlock( AbstractUberBlock &block, const long timeout )
	{
		for ( BlockIterator it = m_Blocks.begin(); it != m_Blocks.end(); ++it )
		{
			if ( *it == &block )
			{
				( *it )->prepareForShutDown();
				if ( (*it)->shutDown( timeout ) )
				{
					delete *it;
					m_Blocks.erase( it );
					return;
				}
				else
				{
					m_Blocks.erase( it );
					ostringstream msg;
					msg << " timeout reached on shutdown of " << block.getName();
					throw TimeOutException( msg.str() );
				}
			}
		}
#ifdef _DEBUG
		assert( NULL );
#endif
	}

}