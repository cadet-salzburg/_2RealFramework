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

#include "engine/_2RealBundleLoader.h"
#include "helpers/_2RealException.h"
#include "engine/_2RealExportMetainfo.h"
#include "bundle/_2RealBundleMetainfo.h"
#include "engine/_2RealEngineImpl.h"
#include "datatypes/_2RealTypeRegistry.h"

using std::make_pair;
using std::string;
using std::ostringstream;

namespace _2Real
{

	BundleLoader::BundleLoader( TypeRegistry *const registry ) : m_Registry( registry )
	{
	}

	BundleLoader::~BundleLoader()
	{
		clear();
	}

	void BundleLoader::clear()
	{
		for ( BundleInfoIterator it = m_LoadedBundles.begin(); it != m_LoadedBundles.end(); /**/ )
		{
			std::string t = it->second.metainfo->getBundleMetadata()->getName();
			delete it->second.metainfo;
			if ( it->second.library != nullptr )
			{
				it->second.library->unload();
				delete it->second.library;
			}
			it = m_LoadedBundles.erase( it );
		}
	}

	void BundleLoader::unloadLibrary( string const& path )
	{
		BundleInfoConstIterator it = m_LoadedBundles.find( path );

		if ( it == m_LoadedBundles.end() )
		{
			ostringstream msg;
			throw NotFoundException( msg.str() );
		}

		delete it->second.metainfo;
		if ( it->second.library != nullptr )
		{
			it->second.library->unload();
			delete it->second.library;
		}
		it = m_LoadedBundles.erase( it );
	}

	//void BundleLoader::resetContextBlock( std::string const& absPath )
	//{
	//	BundleInfoIterator it = m_LoadedBundles.find( absPath );

	//	if ( it == m_LoadedBundles.end() )
	//	{
	//		ostringstream msg;
	//		msg << "shared library " << absPath << " not found";
	//		throw NotFoundException( msg.str() );
	//	}

	//	it->second.metainfo->removeContextBlock();
	//}

	bool BundleLoader::isLibraryLoaded( string const& path ) const
	{
		return ( m_LoadedBundles.find( path ) != m_LoadedBundles.end() );
	}

	bool BundleLoader::exportsContext( string const& path ) const
	{
		BundleInfoConstIterator it = m_LoadedBundles.find( path );

		if ( it == m_LoadedBundles.end() )
		{
			ostringstream msg;
			msg << "shared library " << path << " not found";
			throw NotFoundException( msg.str() );
		}

		return it->second.metainfo->exportsContext();
	}

	//BundleMetadata const& BundleLoader::createBundleEx( std::string const& path, void ( *MetainfoFunc )( bundle::BundleMetainfo & ) )
	//{
	//	Metainfo *info = new Metainfo( path, *m_Registry );

	//	try
	//	{
	//		BundleInfo bundleInfo;
	//		bundle::BundleMetainfo metainfo( *info );
	//		MetainfoFunc( metainfo );
	//		bundleInfo.metainfo = info;
	//		bundleInfo.metainfo->setInstallDirectory( path );
	//		bundleInfo.metainfo->cleanup();
	//		m_LoadedBundles.insert( make_pair( path, bundleInfo ) );
	//	}
	//	catch ( Exception &e )
	//	{
	//		delete info;
	//		throw e;
	//	}

	//	BundleMetadata const& meta = info->getBundleData();
	//	if ( meta.getName() == "undefined" )
	//	{
	//		ostringstream msg;
	//		msg << "bundle " << meta.getInstallDirectory() << " does not define a name";
	//		throw NotFoundException( msg.str() );
	//	}

	//	return meta;
	//}

	std::shared_ptr< const BundleMetadata > BundleLoader::loadLibrary( string const& path )
	{
		if ( isLibraryLoaded( path ) )
		{
			BundleInfoConstIterator it = m_LoadedBundles.find( path );
			return it->second.metainfo->getBundleMetadata();
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
			msg << e.what() << " " << e.message() << " lib could not be loaded";
			throw NotFoundException( msg.str() );
		}

		Metainfo *info = new Metainfo( path, m_Registry );
		std::shared_ptr< BundleMetadata > bundleMetadata = info->getBundleMetadata();

		if ( lib->hasSymbol( "getBundleMetainfo" ) )
		{
			try
			{
				BundleInfo bundleInfo;
				MetainfoFunc func = ( MetainfoFunc ) lib->getSymbol( "getBundleMetainfo" );

				bundleMetadata->setInstallDirectory( path );

				bundle::BundleMetainfo metainfo( bundleMetadata );
				func( metainfo );
				bundleInfo.library = lib;
				bundleInfo.metainfo = info;

				info->postLoading( m_Registry );

				m_LoadedBundles.insert( make_pair( path, bundleInfo ) );
			}
			catch ( Exception &e )
			{
				delete lib;
				delete info;
				throw e;
			}

			return bundleMetadata;
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

	std::shared_ptr< bundle::Block > BundleLoader::createContext( std::string const& path ) const
	{
		BundleInfoConstIterator it = m_LoadedBundles.find( path );

		if ( it == m_LoadedBundles.end() )
		{
			ostringstream msg;
			msg << "shared library " << path << " not found";
			throw NotFoundException( msg.str() );
		}

		return it->second.metainfo->createContextBlock();
	}

	std::shared_ptr< const BundleMetadata > BundleLoader::getBundleMetadata( string const& path ) const
	{
		BundleInfoConstIterator it = m_LoadedBundles.find( path );

		if ( it == m_LoadedBundles.end() )
		{
			ostringstream msg;
			msg << "shared library " << path << " not found";
			throw NotFoundException( msg.str() );
		}

		return it->second.metainfo->getBundleMetadata();
	}

	std::shared_ptr< bundle::Block > BundleLoader::createBlockInstance( std::string const& path, std::string const& blockName ) const
	{
		BundleInfoConstIterator it = m_LoadedBundles.find( path );

		if ( it == m_LoadedBundles.end() )
		{
			ostringstream msg;
			msg << "shared library " << path << " not found";
			throw NotFoundException( msg.str() );
		}

		return it->second.metainfo->createFunctionBlock( blockName );
	}

}