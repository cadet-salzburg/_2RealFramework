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

#include "_2RealEntity.h"

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
	class ServiceBlock;
	class BundleIdentifier;

	class BundleInternal : public Entity
	{

	public:

		BundleInternal( std::string const& name, BundleData const& data );
		~BundleInternal();

		BundleIdentifier	getIdentifier() const;

		const std::string	getBundleInfoString() const;
		const std::string	getBlockInfoString( std::string const& blockName ) const;
		BundleData const&	getBundleData() const;
		BlockData const&	getBlockData( std::string const& blockName ) const;
		void				addBlockInstance( Block &block, std::string const& blockName );
		ServiceBlock &		getBundleContext();
		void				setBundleContext( ServiceBlock &block );
		unsigned int		getBlockInstanceCount( std::string const& blockName ) const;

	private:

		BundleInternal( BundleInternal const& src );
		BundleInternal& operator=( BundleInternal const& src );

		typedef std::multimap< std::string, Block * >	BlockMap;

		BundleData						const& m_BundleData;
		BlockMap						m_BlockInstances;
		ServiceBlock					*m_BundleContext;

	};

}