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

#include "_2RealEngine.h"
#include "_2RealEngineImpl.h"
#include "_2RealIdentifier.h"
#include "_2RealSingletonHolder.h"
#include "_2RealHelpers.h"

#include "Poco/Path.h"

namespace _2Real
{

	Engine & Engine::instance()
	{
		static SingletonHolder< Engine > holder;
		return holder.instance();
	}

	Engine::Engine() :
		m_EngineImpl(EngineImpl::instance())
	{
	}

	Engine::Engine(Engine const& src) :
		m_EngineImpl(EngineImpl::instance())
	{
	}

	Engine::~Engine()
	{
	}

	void Engine::setBaseDirectory( std::string const& directory )
	{
		m_EngineImpl.setBaseDirectory( Poco::Path(directory) );
	}

	const BundleIdentifier Engine::load( std::string const& path )
	{
		return m_EngineImpl.loadLibrary( Poco::Path(path) );
	}

	bool Engine::isLoaded( std::string const& libraryPath ) const
	{
		return m_EngineImpl.isLibraryLoaded(Poco::Path(libraryPath));
	}

	const std::string Engine::getInfoString( BundleIdentifier const& bundleId )
	{
		return m_EngineImpl.getInfoString( bundleId );
	}

	BundleData const& Engine::getBundleData( BundleIdentifier const& bundleId ) const
	{
		return m_EngineImpl.getBundleData( bundleId );
	}

	BlockData const& Engine::getBlockData( BundleIdentifier const& bundleId, std::string const& blockName ) const
	{
		return m_EngineImpl.getBlockData( bundleId, blockName );
	}

}