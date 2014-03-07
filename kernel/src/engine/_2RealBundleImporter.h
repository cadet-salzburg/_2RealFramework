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

#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealPath.h"

namespace _2Real
{
	class BundleMetadata;
	class TypeCollection;
	class SharedLibrary;
	class SharedLibraryMetainfo;

	namespace bundle
	{
		class BundleMetainfo;
	}

	class BundleImporter
	{
	
	public:

		BundleImporter( std::shared_ptr< TypeCollection > registry );
		~BundleImporter();

		void clear();
		bool isLibraryLoaded( Path const& path ) const;
		std::shared_ptr< const SharedLibraryMetainfo > importLibrary( Path const& path );
		void unimportLibrary( Path const& path );

	private:

		BundleImporter( BundleImporter const& );
		BundleImporter& operator=( BundleImporter const& );

		struct SharedLibraryImportData
		{
			std::shared_ptr< SharedLibrary >				library;
			//std::shared_ptr< SharedLibraryMetainfo >		metainfo;
		};
		typedef std::map< Path, SharedLibraryImportData >	ImportData;

		ImportData									mImportData;
		std::weak_ptr< TypeCollection >				mTypeCollection;

	};

}