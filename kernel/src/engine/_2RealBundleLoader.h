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

#pragma once

#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class BundleMetadata;
	class Metainfo;
	class TypeRegistry;
	
	namespace bundle
	{
		class Block;
		class BundleMetainfo;
	}

	class BundleLoader
	{
	
	public:

		BundleLoader( TypeRegistry *registry );
		~BundleLoader();

		void clear();
		//BundleMetadata const& createBundleEx( std::string const& path, void ( *MetainfoFunc )( bundle::BundleMetainfo & ) );
		bool isLibraryLoaded( std::string const& absPath ) const;
		bool exportsContext( std::string const& absPath ) const;
		std::shared_ptr< const BundleMetadata > loadLibrary( std::string const& path );
		void unloadLibrary( std::string const& path );
		std::shared_ptr< bundle::Block > createContext( std::string const& absPath ) const;
		std::shared_ptr< bundle::Block > createBlockInstance( std::string const& absPath, std::string const& ) const;
		std::shared_ptr< const BundleMetadata > getBundleMetadata( std::string const& absPath ) const;
		//void resetContextBlock( std::string const& absPath );

	private:

		struct BundleInfo
		{
			BundleInfo() : library( nullptr ), metainfo( nullptr ) {}
			BundleInfo( BundleInfo const& src ) : library( src.library ), metainfo( src.metainfo ) {}
			Poco::SharedLibrary		*library;
			Metainfo				*metainfo;
		};

		typedef std::map< std::string, BundleInfo >						BundleInfos;
		typedef std::map< std::string, BundleInfo >::iterator			BundleInfoIterator;
		typedef std::map< std::string, BundleInfo >::const_iterator		BundleInfoConstIterator;
		BundleInfos														m_LoadedBundles;
		TypeRegistry													*const m_Registry;

	};

}