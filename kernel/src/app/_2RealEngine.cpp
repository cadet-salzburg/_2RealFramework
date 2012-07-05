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

#include "app/_2RealEngine.h"
#include "engine/_2RealEngineImpl.h"
#include "helpers/_2RealSingletonHolder.h"
#include "app/_2RealBundleHandle.h"
#include "engine/_2RealLink.h"
#include "engine/_2RealAbstractIOManager.h"

using std::string;

namespace _2Real
{
	namespace app
	{
		Engine & Engine::instance()
		{
			static SingletonHolder< Engine > holder;
			return holder.instance();
		}

		Engine::Engine() :
			m_EngineImpl( EngineImpl::instance() )
		{
		}

		Engine::Engine( Engine const& src ) :
			m_EngineImpl( EngineImpl::instance() )
		{
		}

		void Engine::setBaseDirectory( std::string const& directory )
		{
			m_EngineImpl.setBaseDirectory( directory );
		}

		void Engine::clearFully()
		{
			m_EngineImpl.clearFully();
		}

		void Engine::clearBlockInstances()
		{
			m_EngineImpl.clearBlockInstances();
		}

		BundleHandle & Engine::loadBundle( std::string const& libraryPath )
		{
			return m_EngineImpl.loadLibrary( libraryPath );
		}

		void Engine::registerToException( ExceptionCallback callback, void *userData )
		{
			ErrorCallback *cb = new FunctionCallback< std::pair< Exception, BlockHandle > const& >( callback, userData );
			m_EngineImpl.registerToException( *cb );
		}

		void Engine::unregisterFromException( ExceptionCallback callback, void *userData )
		{
			ErrorCallback *cb = new FunctionCallback< std::pair< Exception, BlockHandle > const& >( callback, userData );
			m_EngineImpl.unregisterFromException( *cb );
		}

		void Engine::registerToExceptionInternal( ErrorCallback &cb )
		{
			m_EngineImpl.registerToException( cb );
		}

		void Engine::unregisterFromExceptionInternal( ErrorCallback &cb )
		{
			m_EngineImpl.unregisterFromException( cb );
		}

		Engine::Links Engine::getCurrentLinks() const
		{
			EngineImpl::Links const& currLinks = m_EngineImpl.getCurrentLinks();
			Engine::Links links;
			for ( EngineImpl::LinkConstIterator it = currLinks.begin(); it != currLinks.end(); ++it )
			{
				Engine::Link link( ( *it )->getInletIO().getHandle(), ( *it )->getOutletIO().getHandle() );
				links.insert( link );
			}

			return links;
		}
	}
}