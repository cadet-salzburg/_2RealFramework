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
#include "helpers/_2RealPoco.h"

#include <map>
#include <string>

namespace _2Real
{
	class BundleInternal;
	class Identifier;
	class FunctionBlock;
	class EngineImpl;

	class BundleManager
	{
	
	public:

		BundleManager( EngineImpl &engine );
		~BundleManager();

		void clear();
		void							setBaseDirectory( std::string const& path );
		BundleInternal *				loadLibrary( std::string const& libraryPath );
		bool							isLibraryLoaded( Poco::Path const& path ) const;
		FunctionBlock *					createFunctionBlock( BundleInternal &bundle, std::string const& blockName );

	private:

		FunctionBlock *			createContextBlock( BundleInternal &bundle );

		const Poco::Path				makeAbsolutePath( Poco::Path const& path ) const;
		BundleInternal &				getBundle( Identifier const& id );
		BundleInternal const&			getBundle( Identifier const& id ) const;
		const Identifier				getIdentifier( std::string const& path ) const;
	
		typedef std::map< Identifier, BundleInternal * >	BundleMap;
		typedef std::map< std::string, Identifier >		LookupTable;

		EngineImpl						&m_Engine;
		BundleLoader					m_BundleLoader;
		BundleMap						m_BundleInstances;
		LookupTable						m_BundleLookupTable;		// this might be unnecessary now
		Poco::Path						m_BaseDirectory;

	};

}