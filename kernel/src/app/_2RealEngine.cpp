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
#include "app/_2RealAppData.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealLink.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealFunctionBlock.h"
#include "helpers/_2RealSingletonHolder.h"
#include "xml/_2RealXMLWriter.h"

using std::string;
using std::list;

namespace _2Real
{
	namespace app
	{
		class ConfigAccess
		{
		};

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

		void Engine::clearAll()
		{
			m_EngineImpl.clearFully();
		}

		void Engine::clearBlockInstances()
		{
			m_EngineImpl.clearBlockInstances();
		}

		BundleHandle & Engine::loadBundle( string const& libraryPath )
		{
			return m_EngineImpl.loadLibrary( libraryPath );
		}

		BundleHandle & Engine::findBundleByName( string const& name ) const
		{
			return m_EngineImpl.findBundleByName( name );
		}

		BundleHandle & Engine::findBundleByPath( string const& libraryPath ) const
		{
			return m_EngineImpl.findBundleByPath( libraryPath );
		}

		void Engine::registerToException( BlockExceptionCallback callback, void *userData )
		{
			BlockExcCallback *cb = new FunctionCallback< std::pair< Exception, BlockHandle > const& >( callback, userData );
			m_EngineImpl.registerToException( *cb );
		}

		void Engine::unregisterFromException( BlockExceptionCallback callback, void *userData )
		{
			BlockExcCallback *cb = new FunctionCallback< std::pair< Exception, BlockHandle > const& >( callback, userData );
			m_EngineImpl.unregisterFromException( *cb );
		}

		void Engine::registerToExceptionInternal( BlockExcCallback &cb )
		{
			m_EngineImpl.registerToException( cb );
		}

		void Engine::unregisterFromExceptionInternal( BlockExcCallback &cb )
		{
			m_EngineImpl.unregisterFromException( cb );
		}

		void Engine::registerToExceptionInternal( ContextBlockExcCallback &cb )
		{
			m_EngineImpl.registerToException( cb );
		}

		void Engine::unregisterFromExceptionInternal( ContextBlockExcCallback &cb )
		{
			m_EngineImpl.unregisterFromException( cb );
		}

		void Engine::registerToException( ContextBlockExceptionCallback callback, void *userData )
		{
			ContextBlockExcCallback *cb = new FunctionCallback< std::pair< Exception, ContextBlockHandle > const& >( callback, userData );
			m_EngineImpl.registerToException( *cb );
		}

		void Engine::unregisterFromException( ContextBlockExceptionCallback callback, void *userData )
		{
			ContextBlockExcCallback *cb = new FunctionCallback< std::pair< Exception, ContextBlockHandle > const& >( callback, userData );
			m_EngineImpl.unregisterFromException( *cb );
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

		Engine::BlockHandles Engine::getCurrentBlocks() const
		{
			EngineImpl::BlockInstances const& currBlocks = m_EngineImpl.getCurrentBlockInstances();
			Engine::BlockHandles blocks;
			for ( EngineImpl::BlockInstanceConstIterator it = currBlocks.begin(); it != currBlocks.end(); ++it )
			{
				blocks.push_back( ( *it )->getHandle() );
			}

			return blocks;
		}

		Engine::BundleHandles Engine::getCurrentBundles() const
		{
			EngineImpl::Bundles const& currBundles = m_EngineImpl.getCurrentBundles();
			Engine::BundleHandles bundles;
			for ( EngineImpl::BundleConstIterator it = currBundles.begin(); it != currBundles.end(); ++it )
			{
				bundles.push_back( ( *it )->getHandle() );
			}

			return bundles;
		}

		void Engine::safeConfig( string const& filePath )
		{
			EngineImpl::Bundles const& currBundles = m_EngineImpl.getCurrentBundles();
			EngineImpl::BlockInstances const& currBlocks = m_EngineImpl.getCurrentBlockInstances();
			EngineImpl::Links const& currLinks = m_EngineImpl.getCurrentLinks();

			xml::XMLConfig config;

			for ( EngineImpl::BundleConstIterator it = currBundles.begin(); it != currBundles.end(); ++it )
			{
				xml::BundleConfig b;
				b.bundleName = ( *it )->getName();
				//b.bundlePath = ( *it )->getInstallDirectory();
				config.addBundle( b );
			}

			for ( EngineImpl::BlockInstanceConstIterator it = currBlocks.begin(); it != currBlocks.end(); ++it )
			{
				app::BlockInfo const& blockInfo = ( *it )->getBlockInfo();

				xml::BlockConfig c;
				c.blockInstanceId = ( *it )->getName();
				c.blockId = blockInfo.name;
				c.bundleId = ( *it )->getBundleName();
				c.fps = ( *it )->getUpdateRateAsString();
				c.isRunning = ( *it )->isRunning();

				for ( app::BlockInfo::InletInfoConstIterator pIt = blockInfo.inlets.begin(); pIt != blockInfo.inlets.end(); ++pIt )
				{
					//xml::InletConfig i;
					//i.inletId = pIt->getName();
					//app::InletHandle h = ( *it )->getAppInletHandle( i.inletId );
					//i.updatePolicy = ( *it )->getUpdatePolicyAsString( pIt->getName() );
					//i.bufferSize = ( *it )->getBufferSizeAsString( pIt->getName() );
					//// TODO: hacky wacky!
					//if ( h.getTypename().find( "img_" ) == string::npos )
					//{
					//	i.value = h.getCurrentInput().getDataAsString();
					//}
					//else
					//{
					//	i.value = "";
					//}
					//c.inlets.push_back( i );
				}

				config.addBlockInstance( c );
			}

			for ( EngineImpl::LinkConstIterator it = currLinks.begin(); it != currLinks.end(); ++it )
			{
				xml::ParamConfig in;
				xml::ParamConfig out;

				in.paramId = ( *it )->getInletIO().getName();
				in.blockInstanceId = ( *it )->getInletIO().info().blockName;

				out.paramId = ( *it )->getOutletIO().m_Outlet->getName();
				out.blockInstanceId = ( *it )->getOutletIO().m_Outlet->getOwningUberBlock().getName();

				config.addLink( in, out );
			}

			config.writeTo( filePath );
		}

		void Engine::loadConfig( string const& filePath )
		{
			xml::XMLConfig::loadConfig( *this, filePath );
		}

		list< string > Engine::tryConfig( string const& filePath )
		{
			return xml::XMLConfig::tryConfig( *this, filePath );
		}
	}
}