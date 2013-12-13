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

#include "engine/_2RealBundleImporter.h"
#include "engine/_2RealSharedLibrary.h"

#include "helpers/_2RealException.h"
//#include "engine/_2RealExportMetainfo.h"
//#include "bundle/_2RealBundleMetainfo.h"
//#include "engine/_2RealEngineImpl.h"
//#include "datatypes/_2RealTypeRegistry.h"

namespace _2Real
{

	BundleImporter::BundleImporter( std::shared_ptr< TypeRegistry > registry ) :
		mTypeRegistry( registry )
	{
	}

	BundleImporter::~BundleImporter()
	{
		clear();
	}

	void BundleImporter::clear()
	{
		//for ( BundleInfoIterator it = m_LoadedBundles.begin(); it != m_LoadedBundles.end(); )
		//{
		//	std::string t = it->second.metainfo->getBundleMetadata()->getName();
		//	delete it->second.metainfo;
		//	if ( it->second.library != nullptr )
		//	{
		//		it->second.library->unload();
		//		delete it->second.library;
		//	}
		//	it = m_LoadedBundles.erase( it );
		//}
	}

	bool BundleImporter::isLibraryLoaded( Path const& path ) const
	{
		return ( mImportData.find( path ) != mImportData.end() );
	}

	std::shared_ptr< const BundleMetadata > BundleImporter::importLibrary( Path const& path )
	{
		std::shared_ptr< const BundleMetadata > bundleMetadata;

		// fail-silent bhaviour on reload / just return bundle
		if ( isLibraryLoaded( path ) )
		{
			auto it = mImportData.find( path );
			//return it->second.metainfo->getBundleMetadata();
		}

		// may throw libraryloadexception
		std::shared_ptr< SharedLibrary > lib( new SharedLibrary( path ) );

		typedef void ( *MetainfoFunc )( bundle::BundleMetainfo &info );
		std::shared_ptr< Metainfo > meta;//( new Metainfo( m_Registry, path, id ) );
	//	std::shared_ptr< BundleMetadata > bundleMetadata = info->getBundleMetadata();

		if ( lib->hasSymbol( "getBundleMetainfo" ) )
		{
			SharedLibraryImportData importData;
			MetainfoFunc func = ( MetainfoFunc ) lib->getSymbol( "getBundleMetainfo" );

			bundle::BundleMetainfo metainfo( bundleMetadata );
			func( metainfo );
			importData.library = lib;
			importData.metainfo = info;
			mImportData[ path ] = importData;

			return bundleMetadata;
		}
		else
		{
			lib.reset();
			meta.reset();

			std::ostringstream msg;
			msg << "shared library " << path << " does not export required function \'getBundleMetainfo\'";
			throw NotFoundException( msg.str() );
		}
	}

	void BundleImporter::unimportLibrary( Path const& path )
	{
	//	BundleInfoConstIterator it = m_LoadedBundles.find( path );

	//	if ( it == m_LoadedBundles.end() )
	//	{
	//		ostringstream msg;
	//		throw NotFoundException( msg.str() );
	//	}

	//	delete it->second.metainfo;
	//	if ( it->second.library != nullptr )
	//	{
	//		it->second.library->unload();
	//		delete it->second.library;
	//	}
	//	it = m_LoadedBundles.erase( it );
	}

}