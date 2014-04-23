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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealPath.h"
#include "common/_2RealSignals.h"

namespace _2Real
{
	class BundleImpl;
	class TypeCollection;

	class BundleCollection
	{

	public:

		BundleCollection( std::shared_ptr< TypeCollection > registry );
		~BundleCollection() = default;

		BundleCollection( BundleCollection const& other ) = delete;
		BundleCollection( BundleCollection && other ) = delete;
		BundleCollection& operator=( BundleCollection const& other ) = delete;
		BundleCollection& operator=( BundleCollection && other ) = delete;

		typedef std::pair< std::shared_ptr< BundleImpl >, std::shared_ptr< const BundleMetainfoImpl > > LoadResult;

		LoadResult			loadBundle( Path const& pathToBundle, std::shared_ptr< TypeCollection > );
	
		void				clear( const unsigned long timeout );
		void				bundleUnloaded( std::shared_ptr< const BundleImpl > );

		Path				getBundleDirectory() const;

	private:

		void updateBundleDirectory();

		typedef std::vector< std::pair< boost::signals2::connection, std::shared_ptr< BundleImpl > > >	Bundles;

		Path				mBundleDirectory;
		Bundles				mBundles;

	};
}