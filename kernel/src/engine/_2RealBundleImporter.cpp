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

#include "bundle/_2RealBundleMetainfo.h"
#include "engine/_2RealBundleImporter.h"
#include "engine/_2RealSharedLibrary.h"
#include "engine/_2RealSharedLibraryMetainfo.h"
#include "helpers/_2RealException.h"

namespace _2Real
{

	BundleImporter::BundleImporter( std::shared_ptr< TypeCollection > registry ) :
		mTypeCollection( registry )
	{
	}

	BundleImporter::~BundleImporter()
	{
		clear();
	}

	void BundleImporter::clear()
	{
		for ( auto it = mImportData.begin(); it != mImportData.end(); )
		{
			//it->second.metainfo.reset();
			it->second.library.reset();
			it = mImportData.erase( it );
		}
	}

	bool BundleImporter::isLibraryLoaded( Path const& path ) const
	{
		return ( mImportData.find( path ) != mImportData.end() );
	}

	std::shared_ptr< const SharedLibraryMetainfo > BundleImporter::importLibrary( Path const& path )
	{
		if ( isLibraryLoaded( path ) )
		{
			auto it = mImportData.find( path );
			//return it->second.metainfo;
			return nullptr;
		}

		typedef void ( *MetainfoFunc )( bundle::BundleMetainfo &info );

		// may throw libraryloadexception
		std::shared_ptr< SharedLibrary > lib( new SharedLibrary( path ) );
		std::shared_ptr< SharedLibraryMetainfo > info( new SharedLibraryMetainfo( path ) );

		if ( lib->hasSymbol( "getBundleMetainfo" ) )
		{
			MetainfoFunc func = ( MetainfoFunc ) lib->getSymbol( "getBundleMetainfo" );

			bundle::BundleMetainfo bundleMetainfo( info );
			func( bundleMetainfo );

			// exported services, exported types
			bool isOk = info->performExport();
			if ( !isOk )
				throw BundleImportException( "library metainfo is malformed" );

			SharedLibraryImportData importData;
			importData.library = lib;
			//importData.metainfo = info;
			mImportData[ path ] = importData;

			return info;
		}
		else
		{
			lib.reset();
			info.reset();

			std::ostringstream msg;
			msg << "shared library " << path << " does not export required function \'getBundleMetainfo\'";
			throw BundleImportException( msg.str() );
		}
	}

	void BundleImporter::unimportLibrary( Path const& path )
	{
		auto it = mImportData.find( path );

		if ( it == mImportData.end() )
		{
			std::ostringstream msg;
			msg << "could not find library" << path.string() << std::endl;
			throw NotFoundException( msg.str() );
		}

		// delete ? calls dtor
		/*it->second.metainfo.reset();*/
		it->second.library.reset();
		mImportData.erase( it );
	}

}