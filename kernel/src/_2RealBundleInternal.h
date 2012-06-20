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

#include <map>
#include <string>

namespace Poco
{
	class Path;
}

namespace _2Real
{

	class Block;
	class BundleData;
	class BlockData;
	class FunctionBlock;
	class BundleIdentifier;

	class BundleInternal
	{

	public:

		BundleInternal( BundleIdentifier const& id, BundleData const& data );
		~BundleInternal();

		BundleIdentifier const&	getIdentifier() const;
		std::string const&		getName() const;

		const std::string	getBundleInfoString() const;
		const std::string	getBlockInfoString( std::string const& blockName ) const;
		BundleData const&	getBundleData() const;
		BlockData const&	getBlockData( std::string const& blockName ) const;
		void				addBlockInstance( Block &block, std::string const& blockName );
		FunctionBlock &		getBundleContext();
		void				setBundleContext( FunctionBlock &block );
		unsigned int		getBlockInstanceCount( std::string const& blockName ) const;

	private:

		typedef std::multimap< std::string, Block * >	BlockMap;

		BundleIdentifier				const m_Identifier;
		BundleData						const& m_BundleData;	// must be deleted before the library is unloaded
		BlockMap						m_BlockInstances;
		FunctionBlock					*m_BundleContext;		// the context: null if none was exported

	};

}