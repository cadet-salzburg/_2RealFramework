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

#include "_2RealBundleLoader.h"

#include <map>
#include <string>

#include "Poco/Path.h"

namespace _2Real
{

	class BundleData;
	class BlockData;
	class BundleInternal;
	class BundleIdentifier;
	class SystemImpl;
	class ServiceBlock;
	class UpdatePolicyImpl;

	class BundleManager
	{
	
	public:

		BundleManager();
		~BundleManager();

		void							setBaseDirectory( Poco::Path const& path );
		const BundleIdentifier			loadLibrary( Poco::Path const& path );
		bool							isLibraryLoaded( Poco::Path const& path ) const;
		const std::string				getInfoString( BundleIdentifier const& bundleId ) const;
		ServiceBlock &					createServiceBlock( BundleIdentifier const& bundleId, std::string const& blockName, SystemImpl &sys, UpdatePolicyImpl const& triggers );
		BundleData const&				getBundleData( BundleIdentifier const& bundleId ) const;
		BlockData const&				getBlockData( BundleIdentifier const& bundleId, std::string const& blockName ) const;

	private:

		BundleManager( BundleManager const& src );
		BundleManager& operator=( BundleManager const& src );

		const Poco::Path				makeAbsolutePath( Poco::Path const& path ) const;
		BundleInternal &				getBundle( BundleIdentifier const& id );
		BundleInternal const&			getBundle( BundleIdentifier const& id ) const;
		const BundleIdentifier			getIdentifier( std::string const& path ) const;
	
		typedef std::map< BundleIdentifier, BundleInternal * >	BundleMap;
		typedef std::map< std::string, BundleIdentifier >		LookupTable;

		BundleLoader					m_BundleLoader;
		BundleMap						m_BundleInstances;
		Poco::Path						m_BaseDirectory;
		LookupTable						m_BundleNames;
		SystemImpl						*m_BundleContexts;

	};

}