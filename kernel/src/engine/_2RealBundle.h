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

namespace _2Real
{

	class BundleCollection;
	class SharedLibraryMetainfo;

	class Bundle : public std::enable_shared_from_this< Bundle >
	{

	public:

		Bundle( std::shared_ptr< BundleCollection >, std::shared_ptr< const SharedLibraryMetainfo > );

		void										unload( const long timeout );

	private:

		Bundle( Bundle const& other );
		Bundle operator=( Bundle const& other );

		std::weak_ptr< BundleCollection >				mBundleCollection;
		std::shared_ptr< const SharedLibraryMetainfo >	mBundleMetadata;

	};

}

/*
*	TODO: bundle should maybe manage both lib & lib metainfo? -> makes bundle importer useless
*/