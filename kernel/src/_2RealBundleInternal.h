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

#include "_2RealBundleIdentifier.h"
#include "app/_2RealBlockHandle.h"
#include "app/_2RealContextBlockHandle.h"

#include <map>
#include <string>

namespace _2Real
{

	namespace app
	{
		class BundleHandle;
		class BlockHandle;
		class ContextBlockHandle;
	}

	namespace bundle
	{
		class Block;
	}

	class BundleData;
	class BundleIdentifier;
	class BundleManager;

	class BundleInternal
	{

	public:

		BundleInternal( BundleIdentifier const& id, BundleData const& data, BundleManager &bundleManager );
		~BundleInternal();

		BundleIdentifier const&	getIdentifier() const;
		std::string const&		getName() const;

		app::BundleHandle		createHandle();
		BundleData const&		getMetadata() const;
		app::ContextBlockHandle	getBundleContextHandle() const;
		void					setBundleContextHandle( app::ContextBlockHandle const& handle );
		app::BlockHandle		createBlockInstance( std::string const& blockName );
		void					addBlockInstance( bundle::Block &block, std::string const& blockName );
		unsigned int			getBlockInstanceCount( std::string const& blockName ) const;

	private:

		typedef std::multimap< std::string, bundle::Block * >	BlockMap;

		BundleManager			&m_BundleManager;
		BundleIdentifier		const m_Identifier;
		BundleData				const& m_BundleData;	// must be deleted before the library is unloaded
		BlockMap				m_BlockInstances;
		app::ContextBlockHandle	m_BundleContext;		// for strange reasons, i also hold a handle to the context here

	};

}