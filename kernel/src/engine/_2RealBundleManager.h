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

#include "engine/_2RealBundleLoader.h"
#include "engine/_2RealFunctionBlock.h"
#include "helpers/_2RealPoco.h"

#include <set>
#include <string>

namespace _2Real
{
	class Bundle;
	class Identifier;
	class EngineImpl;

	class BundleManager
	{
	
	public:

		typedef std::set< Bundle * >					Bundles;
		typedef std::set< Bundle * >::iterator			BundleIterator;
		typedef std::set< Bundle * >::const_iterator	BundleConstIterator;

		BundleManager( EngineImpl &engine );
		~BundleManager();

		void clear();
		void											setBaseDirectory( std::string const& path );
		Bundle *										loadLibrary( std::string const& libraryPath );
		bool											isLibraryLoaded( Poco::Path const& path ) const;
		FunctionBlock< app::BlockHandle > *				createFunctionBlock( Bundle &bundle, std::string const& blockName );
		Bundles const&									getBundles() const;

	private:

		FunctionBlock< app::ContextBlockHandle > *		createContextBlock( Bundle &bundle );

		const Poco::Path								makeAbsolutePath( Poco::Path const& path ) const;

		EngineImpl										&m_Engine;
		BundleLoader									m_BundleLoader;
		Bundles											m_Bundles;
		Poco::Path										m_BaseDirectory;

	};

}