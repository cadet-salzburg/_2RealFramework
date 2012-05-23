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
#include "_2RealMetadata.h"
#include "_2RealEngineImpl.h"

#include "Poco/SharedLibrary.h"

#include <iostream>
#include <sstream>

using std::make_pair;
using std::string;
using std::ostringstream;
using Poco::SharedLibrary;

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

	BundleData * BundleLoader::loadLibrary( string const& path )
	{
		if ( isLibraryLoaded( path ) )
		{
			return NULL;
		}

		typedef void ( *MetainfoFunc )( BundleMetainfo info );

		SharedLibrary *lib;
		try
		{
			lib = new SharedLibrary( path );
		}
		catch ( Poco::Exception &e )
		{
			throw _2Real::NotFoundException( e.message() );
		}

		BundleData *result = new BundleData();
		Metainfo *info = new Metainfo( *result );

		if ( lib->hasSymbol( "getBundleMetainfo" ) )
		{
			try
			{
				BundleInfo bundleInfo;
				MetainfoFunc func = ( MetainfoFunc ) lib->getSymbol( "getBundleMetainfo" );

				BundleMetainfo metainfo( *info, EngineImpl::instance().getTypetable() );
				func( metainfo );
				bundleInfo.library = lib;
				bundleInfo.metainfo = info;
				m_LoadedBundles.insert( make_pair( path, bundleInfo ) );

				bundleInfo.metainfo->postProcess();
				bundleInfo.metainfo->setName( path );
				bundleInfo.metainfo->setInstallDirectory( path );

				return result;
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
			throw Exception( msg.str() );
		}
	}

	//Block& BundleLoader::createContext( std::string const& path ) const
	//{
	//	BundleMap::const_iterator it = m_LoadedBundles.find( path );

	//	if ( it == m_LoadedBundles.end() )
	//	{
	//		ostringstream msg;
	//		msg << "shared library " << path << " not found";
	//		throw NotFoundException( msg.str() );
	//	}

	//	return it->second.metainfo->createBundleContext();
	//}

	Block& BundleLoader::createBlock( std::string const& path, std::string const& blockName ) const
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