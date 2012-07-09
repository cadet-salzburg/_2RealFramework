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

#include "engine/_2RealIdentifier.h"
#include "app/_2RealBlockHandle.h"
#include "app/_2RealContextBlockHandle.h"
#include "app/_2RealBundleHandle.h"

#include "helpers/_2RealHandleable.h"
#include "helpers/_2RealNonCopyable.h"

#include <map>
#include <string>

namespace _2Real
{

	namespace app
	{
		class BundleHandle;
		class BlockHandle;
		class ContextBlockHandle;
		class BundleInfo;
	}

	namespace bundle
	{
		class Block;
	}

	class BundleData;
	class Identifier;
	class BundleManager;

	class Bundle : private NonCopyable< Bundle >, private Handleable< app::BundleHandle >
	{

	public:

		Bundle( Identifier const& id, BundleData const& data, BundleManager &bundleManager );
		~Bundle();

		using Handleable< app::BundleHandle >::getHandle;
		using Handleable< app::BundleHandle >::registerHandle;
		using Handleable< app::BundleHandle >::unregisterHandle;

		Identifier const&		getIdentifier() const;
		std::string const&		getName() const;

		void					clear();
		app::BundleHandle		createHandle();
		app::BundleInfo			getBundleData() const;
		app::BlockHandle &		createBlockInstance( std::string const& blockName );

		BundleData const&		getMetadata() const;
		void					addBlockInstance( bundle::Block &block, std::string const& blockName );
		unsigned int			getBlockInstanceCount( std::string const& blockName ) const;

	private:

		typedef std::multimap< std::string, bundle::Block * >					Blocks;
		typedef std::multimap< std::string, bundle::Block * >::iterator			BlockIterator;
		typedef std::multimap< std::string, bundle::Block * >::const_iterator	BlockConstIterator;

		BundleManager			&m_BundleManager;
		Identifier				const m_Identifier;
		BundleData				const& m_Metadata;		// must be deleted before the library is unloaded
		Blocks					m_BlockInstances;

	};

}