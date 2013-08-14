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
#include "engine/_2RealBundle.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealConstants.h"

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
		for ( std::multimap< std::string, std::shared_ptr< FunctionBlock > >::iterator it = mBlockInstances.begin(); it != mBlockInstances.end(); ++it )
		{
			if ( it->second->isContext() ) continue;
			it->second->prepareForShutDown();
		}

		for ( std::multimap< std::string, std::shared_ptr< FunctionBlock > >::iterator it = mBlockInstances.begin(); it != mBlockInstances.end(); /**/ )
		{
			if ( it->second->isContext() )
			{
				++it;
			}
			else
			{
				if ( it->second->shutDown( 1000 ) )
				{
					std::cout << "successfully shut down " << it->second->getHumanReadableName() << std::endl;
					mEngineImpl->getLogger()->addLine( string( "successfully shut down ").append( it->second->getHumanReadableName() ) );
					it = mBlockInstances.erase( it );
				}
				else
				{
					std::cout << "failed to shut down " << it->second->getHumanReadableName() << std::endl;
					mEngineImpl->getLogger()->addLine( string( "failed to shut down ").append( it->second->getHumanReadableName() ) );
					mFailedBlockInstances.push_back( it->second );
					it = mBlockInstances.erase( it );
				}
			}
		}

		for ( std::multimap< std::string, std::shared_ptr< FunctionBlock > >::iterator it = mBlockInstances.begin(); it != mBlockInstances.end(); ++it )
			it->second->prepareForShutDown();

		for ( std::multimap< std::string, std::shared_ptr< FunctionBlock > >::iterator it = mBlockInstances.begin(); it != mBlockInstances.end(); /**/ )
		{
			if ( it->second->shutDown( 1000 ) )
			{
				std::cout << "successfully shut down " << it->second->getHumanReadableName() << std::endl;
				mEngineImpl->getLogger()->addLine( string( "successfully shut down ").append( it->second->getHumanReadableName() ) );
				it = mBlockInstances.erase( it );
			}
			else
			{
				std::cout << "failed to shut down " << it->second->getHumanReadableName() << std::endl;
				mEngineImpl->getLogger()->addLine( string( "failed to shut down ").append( it->second->getHumanReadableName() ) );
				mFailedBlockInstances.push_back( it->second );
				it = mBlockInstances.erase( it );
			}
		}
	}

	void System::destroyBlocks( std::shared_ptr< const Bundle > bundle )
	{
		typedef std::multimap< std::string, std::shared_ptr< FunctionBlock > >::iterator iter;
		
		iter context;
		std::pair< iter, iter > range = mBlockInstances.equal_range( bundle->getBundleMetadata()->getInstallDirectory() );
		for ( iter it = range.first; it != range.second; ++it )
		{
			if ( it->second->isContext() ) context = it;
			else it->second->prepareForShutDown();
		}

		for ( iter it = range.first; it != range.second; ++it )
		{
			if ( it->second->isContext() ) continue;
			else
			{
				if ( it->second->shutDown( 1000 ) )
				{
					std::cout << "shut down " << it->second->getHumanReadableName() << std::endl;
					mBlockInstances.erase( it );
				}
				else
				{
					mEngineImpl->getLogger()->addLine( string( "failed to shut down ").append( it->second->getHumanReadableName() ) );
					mFailedBlockInstances.push_back( it->second );
					it = mBlockInstances.erase( it );
				}
			}
		}

		context->second->prepareForShutDown();
		if ( context->second->shutDown( 1000 ) )
		{
			std::cout << "shut down " << context->second->getHumanReadableName() << std::endl;
			mBlockInstances.erase( context );
		}
		else
		{
			mEngineImpl->getLogger()->addLine( string( "failed to shut down ").append( context->second->getHumanReadableName() ) );
			mFailedBlockInstances.push_back( context->second );
			mBlockInstances.erase( context );
		}
	}

	void System::addBlockInstance( std::shared_ptr< const Bundle > bundle, std::shared_ptr< FunctionBlock > block )
	{
		mBlockInstances.insert( std::make_pair( bundle->getBundleMetadata()->getInstallDirectory(), block ) );
	}

	std::shared_ptr< FunctionBlock > System::findContextBlockInstance( std::shared_ptr< const Bundle > bundle )
	{
		typedef std::multimap< std::string, std::shared_ptr< FunctionBlock > >::iterator iter;
		std::pair< iter, iter > range = mBlockInstances.equal_range( bundle->getBundleMetadata()->getInstallDirectory() );
		for ( iter it = range.first; it != range.second; ++it )
		{
			if ( it->second->isContext() ) return it->second;
		}

		return std::shared_ptr< FunctionBlock >();
	}

	void System::removeBlockInstance( std::shared_ptr< const Bundle > bundle, std::shared_ptr< FunctionBlock > block, const long timeout )
	{
		typedef std::multimap< std::string, std::shared_ptr< FunctionBlock > >::iterator iter;
		std::pair< iter, iter > range = mBlockInstances.equal_range( bundle->getBundleMetadata()->getInstallDirectory() );
		for ( iter it = range.first; it != range.second; ++it )
		{
			if ( it->second == block )
			{
				it->second->prepareForShutDown();
				if ( it->second->shutDown( timeout ) )
				{
					mBlockInstances.erase( it );
					return;
				}
				else
				{
					std::ostringstream msg;
					msg << " timeout reached on shutdown of " << block->getFullHumanReadableName();
					throw TimeOutException( msg.str() );
				}
			}
		}
	}
}