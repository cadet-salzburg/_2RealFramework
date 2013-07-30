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

	System::System( EngineImpl *engine ) :
		mEngineImpl( engine )
	{
	}

	System::~System()
	{
		clear();
	}

	void System::clear()
	{
		for ( BlockIterator it = mBlockInstances.begin(); it != mBlockInstances.end(); ++it )
		{
			it->second->prepareForShutDown();
			std::cout << "prepared " << it->second->getName() << " for shutdown" << std::endl;
		}

		for ( BlockIterator it = mBlockInstances.begin(); it != mBlockInstances.end(); /**/ )
		{
			if ( it->second->shutDown( 1000 ) )
			{
				std::cout << "shut down " << it->second->getName() << std::endl;
				it = mBlockInstances.erase( it );
			}
			else
			{
				mEngineImpl->getLogger()->addLine( string( "failed to shut down ").append( it->second->getFullName() ) );
				++it;
			}
		}

		for ( ContextBlockIterator it = mContextBlocks.begin(); it != mContextBlocks.end(); ++it )
		{
			it->second->prepareForShutDown();
			std::cout << "prepared " << it->second->getName() << " for shutdown" << std::endl;
		}

		for ( ContextBlockIterator it = mContextBlocks.begin(); it != mContextBlocks.end(); /**/ )
		{
			if ( it->second->shutDown( 1000 ) )
			{
				std::cout << "shut down " << it->second->getName() << std::endl;
				it = mContextBlocks.erase( it );
			}
			else
			{
				mEngineImpl->getLogger()->addLine( string( "failed to shut down ").append( it->second->getFullName() ) );
				++it;
			}
		}
	}

	void System::addContextBlockInstance( Bundle *bundle, std::shared_ptr< FunctionBlock > block )
	{
		mContextBlocks.insert( std::make_pair( bundle->getAbsPath(), block ) );
	}

	void System::addRegularBlockInstance( Bundle *bundle, std::shared_ptr< FunctionBlock > block )
	{
		mBlockInstances.insert( std::make_pair( bundle->getAbsPath(), block ) );
	}

	void System::destroyBlocks( Bundle *bundle )
	{
		std::pair< BlockIterator, BlockIterator > range = mBlockInstances.equal_range( bundle->getAbsPath() );

		for ( BlockIterator it = range.first; it != range.second; ++it )
		{
			it->second->prepareForShutDown();
			std::cout << "prepared " << it->second->getName() << " for shutdown" << std::endl;
		}

		for ( BlockIterator it = range.first; it != range.second; )
		{
			if ( it->second->shutDown( 3000 ) )
				std::cout << "shut down " << it->second->getName() << std::endl;
			else
				mEngineImpl->getLogger()->addLine( string( "failed to shut down ").append( it->second->getFullName() ) );

			it = mBlockInstances.erase( it );
		}
	}

	unsigned int System::getBlockInstanceCount( Bundle const* bundle, std::string const& name )
	{
		std::pair< BlockIterator, BlockIterator > range = mBlockInstances.equal_range( bundle->getAbsPath() );

		unsigned int cnt = 0;

		for ( BlockIterator it = range.first; it != range.second; ++it )
		{
			if ( it->second->getName() == name )
				++cnt;
		}

		return cnt;
	}

	std::shared_ptr< FunctionBlock > System::getContextBlock( Bundle const* bundle )
	{
		ContextBlockIterator it = mContextBlocks.find( bundle->getAbsPath() );
		if ( it == mContextBlocks.end() )
			return std::shared_ptr< FunctionBlock >();
		else
			return std::dynamic_pointer_cast< FunctionBlock >( it->second );
	}

//	void System::removeRegularBlockInstance( std::shared_ptr< FunctionBlock > block, const long timeout )
//	{
//		for ( BlockIterator it = mBlockInstances.begin(); it != mBlockInstances.end(); ++it )
//		{
//			if ( *it == block )
//			{
//				( *it )->prepareForShutDown();
//				if ( ( *it )->shutDown( timeout ) )
//				{
//					mBlockInstances.erase( it );
//					return;
//				}
//				else
//				{
//					//mBlockInstances.erase( it );
//					ostringstream msg;
//					msg << " timeout reached on shutdown of " << block->getFullName();
//					throw TimeOutException( msg.str() );
//				}
//			}
//		}
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//	}
//
//	void System::removeContextBlockInstance( std::shared_ptr< FunctionBlock > block, const long timeout )
//	{
//		for ( BlockIterator it = mContextBlocks.begin(); it != mContextBlocks.end(); ++it )
//		{
//			if ( *it == block )
//			{
//				( *it )->prepareForShutDown();
//				if ( ( *it )->shutDown( timeout ) )
//				{
//					mContextBlocks.erase( it );
//					return;
//				}
//				else
//				{
//					//mContextBlocks.erase( it );
//					ostringstream msg;
//					msg << " timeout reached on shutdown of " << block->getFullName();
//					throw TimeOutException( msg.str() );
//				}
//			}
//		}
//#ifdef _DEBUG
//		assert( NULL );
//#endif
//	}
}