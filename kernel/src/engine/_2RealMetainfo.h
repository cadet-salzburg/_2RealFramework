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

#include "engine/_2RealBundleMetadata.h"

#include <map>
#include <list>
#include <string>

namespace _2Real
{
	class Version;
	class BlockMetadata;
	class TypeMetadata;
	class TypeRegistry;

	namespace bundle
	{
		class Block;
		class ContextBlock;
		class AbstractBlockCreator;
		class BlockMetainfo;
		class ContextBlockMetainfo;
		class TypeMetainfo;
	}

	class Metainfo
	{

	public:

		Metainfo( std::string const& id, TypeRegistry const& init );
		~Metainfo();

		void setAuthor( std::string const& author );
		void setDescription( std::string const& description );
		void setCategory( std::string const& category );
		void setVersion( Version const& version );
		void setContact( std::string const& contact );
		void setInstallDirectory( std::string const& path );
		void setName( std::string const& name );

		bundle::ContextBlockMetainfo & setContextBlockCreator( bundle::AbstractBlockCreator &obj );
		bundle::BlockMetainfo & setBlockCreator( std::string const& blockName, bundle::AbstractBlockCreator &obj );

		bool hasContext() const;
		std::shared_ptr< _2Real::bundle::Block > createBlock( std::string const& ) const;
		std::shared_ptr< _2Real::bundle::Block > createContextBlock() const;

		bundle::TypeMetainfo & addCustomType( std::string const& name );
		void registerTypes( TypeRegistry &registry );

		BundleMetadata const& getBundleData() const;

		void cleanup();

	private:

		struct BlockInfo
		{
			BlockInfo() : ctor( nullptr ), data( nullptr ), meta( nullptr ) {}
			BlockInfo( BlockInfo const& src ) : ctor( src.ctor ), data( src.data ), meta( src.meta ) {}
			bundle::AbstractBlockCreator	*ctor;
			BlockMetadata					*data;
			bundle::BlockMetainfo			*meta;
		};

		struct ContextBlockInfo
		{
			ContextBlockInfo() : ctor( nullptr ), data( nullptr ), meta( nullptr ) {}
			ContextBlockInfo( ContextBlockInfo const& src ) : ctor( src.ctor ), data( src.data ), meta( src.meta ) {}
			bundle::AbstractBlockCreator	*ctor;
			BlockMetadata					*data;
			bundle::ContextBlockMetainfo	*meta;
		};

		typedef std::map< std::string, BlockInfo >					BlockInfos;
		typedef std::map< std::string, BlockInfo >::iterator		BlockInfoIterator;
		typedef std::map< std::string, BlockInfo >::const_iterator	BlockInfoConstIterator;

		bool										m_HasContext;
		BlockInfos									m_BlockInfos;
		ContextBlockInfo							m_ContextInfo;
		BundleMetadata								m_BundleData;
		TypeRegistry								*mTypes;
		std::string									mBundleId;

	};
}