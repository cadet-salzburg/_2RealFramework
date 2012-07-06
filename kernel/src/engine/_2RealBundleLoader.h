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

#include "helpers/_2RealPoco.h"

#include <map>

namespace _2Real
{
	class BundleData;
	class Metainfo;
	
	namespace bundle
	{
		class Block;
	}

	class BundleLoader
	{
	
	public:

		BundleLoader();
		~BundleLoader();

		void clear();
		bool isLibraryLoaded( std::string const& path ) const;
		bool hasContext( std::string const& path ) const;
		BundleData const& loadLibrary( std::string const& path );
		bundle::Block& createContext( std::string const& path ) const;
		bundle::Block& createBlock( std::string const& path, std::string const& blockName ) const;

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

	};

}