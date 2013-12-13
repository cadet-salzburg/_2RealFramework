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

#include "engine/_2RealBundleImporter.h"
#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealPath.h"

namespace _2Real
{
	class Bundle;
	class FunctionBlock;
	class EngineImpl;
	class TypeRegistry;

	class BundleCollection : public std::enable_shared_from_this< BundleCollection >
	{
	
	public:

		BundleCollection( std::shared_ptr< TypeRegistry > registry );
		~BundleCollection();

		void										clear();
		std::shared_ptr< Bundle > 					loadBundle( Path const& pathToBundle );
		void										unloadBundle( std::shared_ptr< Bundle >, const long timeout );
		Path const&									getBundleDirectory() const;

	private:

		BundleCollection( BundleCollection const& other );
		BundleCollection& operator=( BundleCollection const& other );

		void updateBundleDirectory();

		//typedef std::map< std::string, std::shared_ptr< Bundle > >					Bundles;

		/*std::weak_ptr< EngineImpl >						mEngineImpl;*/
		// responsible for loading bundles, duh
		BundleImporter								mBundleImporter;
		// initialized on contruction by querying an env variable; can't change during runtime
		Path										mBundleDirectory;
		//Bundles											mBundles;

		//template< typename TId >
		//class IdCounter
		//{
		//public:
		//	IdCounter( TId const& init ) : mCounter( init ) {}
		//	TId getNextId() { return ++mCounter; }
		//private:
		//	TId		mCounter;
		//};

		//IdCounter< int >								mIdCounter;

	};
}