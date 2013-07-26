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
#include "engine/_2RealLink.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealFunctionBlock.h"
#include "helpers/_2RealSingletonHolder.h"

using std::string;
using std::list;

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
			mImpl( new EngineImpl )
		{
		}

		Engine::~Engine()
		{
			delete mImpl;
		}

// ---------------------------------- clearing

		void Engine::clearAll()
		{
			mImpl->clearFully();
		}

		void Engine::clearBlockInstances()
		{
			mImpl->clearBlockInstances();
		}

// ---------------------------------- clearing

// ---------------------------------- bundle loading

		std::string Engine::getBundleDirectory() const
		{
			return mImpl->getBundleDirectory();
		}

		BundleHandle & Engine::loadBundle( string const& libraryPath )
		{
			return mImpl->loadLibrary( libraryPath ).getHandle();
		}

// ---------------------------------- bundle loading

// ---------------------------------- exception callbacks

		void Engine::registerToException( BlockExceptionCallback callback, void *userData )
		{
			BlockExcCallback *cb = new FunctionCallback< std::pair< Exception, BlockHandle > >( callback, userData );
			mImpl->registerToException( *cb );
		}

		void Engine::unregisterFromException( BlockExceptionCallback callback, void *userData )
		{
			BlockExcCallback *cb = new FunctionCallback< std::pair< Exception, BlockHandle > >( callback, userData );
			mImpl->unregisterFromException( *cb );
		}

		void Engine::registerToExceptionInternal( BlockExcCallback &cb )
		{
			mImpl->registerToException( cb );
		}

		void Engine::unregisterFromExceptionInternal( BlockExcCallback &cb )
		{
			mImpl->unregisterFromException( cb );
		}

		void Engine::registerToExceptionInternal( ContextBlockExcCallback &cb )
		{
			mImpl->registerToException( cb );
		}

		void Engine::unregisterFromExceptionInternal( ContextBlockExcCallback &cb )
		{
			mImpl->unregisterFromException( cb );
		}

		void Engine::registerToException( ContextBlockExceptionCallback callback, void *userData )
		{
			ContextBlockExcCallback *cb = new FunctionCallback< std::pair< Exception, ContextBlockHandle > >( callback, userData );
			mImpl->registerToException( *cb );
		}

		void Engine::unregisterFromException( ContextBlockExceptionCallback callback, void *userData )
		{
			ContextBlockExcCallback *cb = new FunctionCallback< std::pair< Exception, ContextBlockHandle > >( callback, userData );
			mImpl->unregisterFromException( *cb );
		}

// ---------------------------------- exception callbacks

		//Engine::Links Engine::getCurrentLinks()
		//{
		//	EngineImpl::Links &currLinks = m_EngineImpl.getCurrentLinks();
		//	Engine::Links links;
		//	for ( EngineImpl::LinkIterator it = currLinks.begin(); it != currLinks.end(); ++it )
		//	{
		//		Engine::Link link( ( *it )->getInletIO().getHandle(), ( *it )->getOutletIO().getHandle() );
		//		links.insert( link );
		//	}

		//	return links;
		//}

		//Engine::BlockHandles Engine::getCurrentBlocks()
		//{
		//	EngineImpl::BlockInstances const& currBlocks = m_EngineImpl.getCurrentBlockInstances();
		//	Engine::BlockHandles blocks;
		//	for ( EngineImpl::BlockInstanceConstIterator it = currBlocks.begin(); it != currBlocks.end(); ++it )
		//	{
		//		blocks.push_back( ( *it )->getHandle() );
		//	}

		//	return blocks;
		//}

		//Engine::BundleHandles Engine::getCurrentBundles()
		//{
		//	EngineImpl::Bundles const& currBundles = m_EngineImpl.getCurrentBundles();
		//	Engine::BundleHandles bundles;
		//	for ( EngineImpl::BundleConstIterator it = currBundles.begin(); it != currBundles.end(); ++it )
		//	{
		//		bundles.push_back( ( *it )->getHandle() );
		//	}

		//	return bundles;
		//}

		//void Engine::saveConfig( string const& filePath )
		//{
		//	//EngineImpl::Bundles const& currBundles = m_EngineImpl.getCurrentBundles();
		//	//EngineImpl::BlockInstances const& currBlocks = m_EngineImpl.getCurrentBlockInstances();
		//	//EngineImpl::Links const& currLinks = m_EngineImpl.getCurrentLinks();

		//	//xml::XMLConfig config;

		//	//for ( EngineImpl::BundleConstIterator it = currBundles.begin(); it != currBundles.end(); ++it )
		//	//{
		//	//	xml::BundleConfig b;
		//	//	b.bundleName = ( *it )->getName();
		//	//	//b.bundlePath = ( *it )->getInstallDirectory();
		//	//	config.addBundle( b );
		//	//}

		//	//for ( EngineImpl::BlockInstanceConstIterator it = currBlocks.begin(); it != currBlocks.end(); ++it )
		//	//{
		//	//	app::BlockInfo const& blockInfo = ( *it )->getBlockInfo();

		//	//	xml::BlockConfig c;
		//	//	c.blockInstanceId = ( *it )->getName();
		//	//	c.blockId = blockInfo.name;
		//	//	c.bundleId = ( *it )->getBundleName();
		//	//	c.fps = ( *it )->getUpdateRateAsString();
		//	//	c.isRunning = ( *it )->isRunning();

		//	//	for ( app::BlockInfo::InletInfoConstIterator pIt = blockInfo.inlets.begin(); pIt != blockInfo.inlets.end(); ++pIt )
		//	//	{
		//	//		xml::InletConfig i;
		//	//		i.inletId = pIt->name;
		//	//		app::InletHandle h = ( *it )->getAppInletHandle( i.inletId );
		//	//		if ( h.isMultiInlet() )
		//	//		{
		//	//			i.isMulti = true;
		//	//			for ( unsigned int j=0; j<h.getSize(); ++j )
		//	//			{
		//	//				xml::BasicInletConfig b;
		//	//				app::InletHandle s = h[ j ];
		//	//				b.inletId = s.getName();
		//	//				app::InletHandle::InletState curr = s.getCurrentState();
		//	//				b.updatePolicy = curr.updatePolicy;
		//	//				b.bufferSize = curr.bufferSize;
		//	//				b.bufferedValue = curr.currentValue;
		//	//				b.initialValue = curr.defaultValue;
		//	//				i.basicInlets.push_back( b );
		//	//			}
		//	//		}
		//	//		else
		//	//		{
		//	//			i.isMulti = false;
		//	//			xml::BasicInletConfig b;
		//	//			b.inletId = h.getName();
		//	//			app::InletHandle::InletState curr = h.getCurrentState();
		//	//			b.updatePolicy = curr.updatePolicy;
		//	//			b.bufferedValue = curr.currentValue;
		//	//			b.initialValue = curr.defaultValue;
		//	//			i.basicInlets.push_back( b );
		//	//		}

		//	//		c.inlets.push_back( i );
		//	//	}

		//	//	config.addBlockInstance( c );
		//	//}

		//	//for ( EngineImpl::LinkConstIterator it = currLinks.begin(); it != currLinks.end(); ++it )
		//	//{
		//	//	xml::ParamConfig in;
		//	//	xml::ParamConfig out;

		//	//	in.paramId = ( *it )->getInletIO().getName();
		//	//	in.blockInstanceId = ( *it )->getInletIO().info().owner.getName();

		//	//	out.paramId = ( *it )->getOutletIO().m_Outlet->getName();
		//	//	out.blockInstanceId = ( *it )->getOutletIO().m_Outlet->getOwningUberBlock().getName();

		//	//	config.addLink( in, out );
		//	//}

		//	//config.writeTo( filePath );
		//}

		//void Engine::loadConfig( string const& filePath )
		//{
		//	//xml::XMLConfig::loadConfig( *this, filePath );
		//}

		//list< string > Engine::tryConfig( string const& filePath )
		//{
		//	returnConfiguration::tryConfig( *this, filePath );
		//}

	//	SystemState * Engine::loadConfiguration( string const& dataSource )
	//	{
	//		std::map< std::string, app::BundleHandle > loadedBundles;
	//		EngineImpl::Bundles const& currBundles = m_EngineImpl.getCurrentBundles();
	//		for ( EngineImpl::BundleConstIterator it = currBundles.begin(); it != currBundles.end(); ++it )
	//		{
	//			loadedBundles.insert( std::make_pair( ( *it )->getBundleInfo().name, ( *it )->getHandle() ) );
	//		}

	//		Configuration config;

	//		std::stringstream str;
	//		str << dataSource;
	//		str >> config;

	//		_2Real::ConfigurationLoader loader;

	//		return loader.load( m_EngineImpl, config );
	//	}

	//	list< string > Engine::testConfiguration( string const& dataSource )
	//	{
	//		return list< string >();
	//	}

	//	void Engine::getCurrentSystemState( SystemState &state )
	//	{
	//		EngineImpl::Bundles const& currBundles = m_EngineImpl.getCurrentBundles();
	//		Engine::BundleHandles bundles;
	//		for ( EngineImpl::BundleConstIterator it = currBundles.begin(); it != currBundles.end(); ++it )
	//			bundles.push_back( ( *it )->getHandle() );

	//		EngineImpl::BlockInstances &currBlocks = m_EngineImpl.getCurrentBlockInstances();
	//		Engine::BlockHandles blocks;
	//		for ( EngineImpl::BlockInstanceConstIterator it = currBlocks.begin(); it != currBlocks.end(); ++it )
	//			blocks.push_back( ( *it )->getHandle() );

	//		EngineImpl::Links &currLinks = m_EngineImpl.getCurrentLinks();
	//		Engine::Links links;
	//		for ( EngineImpl::LinkConstIterator it = currLinks.begin(); it != currLinks.end(); ++it )
	//			links.insert( Engine::Link( ( *it )->getInletIO().getHandle(), ( *it )->getOutletIO().getHandle() ) );

	//		//state.mBundles = bundles;

	//		// !!!!! might lead to a memory leak
	//		state.mVertices.clear();
	//		state.mEdges.clear();

	//		for ( BlockHandles::const_iterator bIt = blocks.begin(); bIt != blocks.end(); ++bIt )
	//		{
	//			BlockHandle const& handle = ( *bIt );
	//			state.mVertices.insert( std::make_pair( handle, new SystemState::Vertex( handle ) ) );
	//		}

	//		for ( Links::iterator lIt = links.begin(); lIt != links.end(); ++lIt )
	//		{
	//			InletHandle inlet = ( *lIt ).first;
	//			BlockHandle inletOwner = inlet.getOwningBlock();
	//			OutletHandle outlet = ( *lIt ).second;
	//			BlockHandle outletOwner = outlet.getOwningBlock();

	//			// both blockHandles are now for sure in the graph's vertices
	//			SystemState::Vertex *in = state.mVertices[ inletOwner ];
	//			SystemState::Vertex *out = state.mVertices[ outletOwner ];

	//			SystemState::Edge *edge = new SystemState::Edge( *lIt, *in, *out );
	//			state.mEdges.insert( edge );
	//		}
	//	};
	}
}