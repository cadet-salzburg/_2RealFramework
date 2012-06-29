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

#include "_2RealBundleLoader.h"
#include "_2RealException.h"
#include "_2RealBundleMetainfoImpl.h"
#include "_2RealBundleMetainfo.h"
#include "_2RealEngineImpl.h"

#include <iostream>
#include <sstream>

using std::make_pair;
using std::string;
using std::ostringstream;

namespace _2Real
{

	BundleLoader::BundleLoader()
	{
	}

	BundleLoader::~BundleLoader()
	{
		for ( BundleMap::iterator it = m_LoadedBundles.begin(); it != m_LoadedBundles.end(); /**/ )
		{
			delete it->second.metainfo;
			it->second.library->unload();
			delete it->second.library;
			it = m_LoadedBundles.erase( it );
		}
	}

	BundleLoader::BundleLoader( BundleLoader const& src )
	{
	}

	BundleLoader& BundleLoader::operator=( BundleLoader const& src )
	{
		return *this;
	}

	bool BundleLoader::isLibraryLoaded( string const& path ) const
	{
		return ( m_LoadedBundles.find( path ) != m_LoadedBundles.end() );
	}

	bool BundleLoader::hasContext( string const& path ) const
	{
		BundleMap::const_iterator it = m_LoadedBundles.find( path );

		if ( it == m_LoadedBundles.end() )
		{
			ostringstream msg;
			msg << "shared library " << path << " not found";
			throw NotFoundException( msg.str() );
		}

		return it->second.metainfo->hasContext();
	}

	BundleData const& BundleLoader::loadLibrary( string const& path )
	{
		if ( isLibraryLoaded( path ) )
		{
			BundleMap::const_iterator it = m_LoadedBundles.find( path );
			return it->second.metainfo->getBundleData();
		}

		typedef void ( *MetainfoFunc )( bundle::BundleMetainfo &info );

		Poco::SharedLibrary *lib;
		try
		{
			lib = new Poco::SharedLibrary( path );
		}
		catch ( Poco::Exception &e )
		{
			ostringstream msg;
			msg << e.what() << " shared library " << path << " does not exist";
			throw NotFoundException( msg.str() );
		}

		Metainfo *info = new Metainfo( EngineImpl::instance().getTypetable() );

		if ( lib->hasSymbol( "getBundleMetainfo" ) )
		{
			try
			{
				BundleInfo bundleInfo;
				MetainfoFunc func = ( MetainfoFunc ) lib->getSymbol( "getBundleMetainfo" );

				bundle::BundleMetainfo metainfo( *info );
				func( metainfo );
				bundleInfo.library = lib;
				bundleInfo.metainfo = info;
				m_LoadedBundles.insert( make_pair( path, bundleInfo ) );

				bundleInfo.metainfo->cleanup();
				bundleInfo.metainfo->setName( path );
				bundleInfo.metainfo->setInstallDirectory( path );

				return info->getBundleData();
			}
			catch ( Exception &e )
			{
				delete lib;
				delete info;
				throw e;
			}
		}
		else
		{
			delete lib;
			delete info;

			ostringstream msg;
			msg << "shared library " << path << " does not contain metadata";
			throw NotFoundException( msg.str() );
		}
	}

	bundle::Block& BundleLoader::createContext( std::string const& path ) const
	{
		BundleMap::const_iterator it = m_LoadedBundles.find( path );

		if ( it == m_LoadedBundles.end() )
		{
			ostringstream msg;
			msg << "shared library " << path << " not found";
			throw NotFoundException( msg.str() );
		}

		return it->second.metainfo->createContextBlock();
	}

	bundle::Block& BundleLoader::createBlock( std::string const& path, std::string const& blockName ) const
	{
		BundleMap::const_iterator it = m_LoadedBundles.find( path );

		if ( it == m_LoadedBundles.end() )
		{
			ostringstream msg;
			msg << "shared library " << path << " not found";
			throw NotFoundException( msg.str() );
		}

		return it->second.metainfo->createBlock( blockName );
	}

}