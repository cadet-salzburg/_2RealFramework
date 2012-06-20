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

#include "_2RealBundleData.h"
#include "_2RealBlockData.h"

#include <map>
#include <string>

namespace _2Real
{

	class Version;
	class Block;
	class ContextBlock;
	class AbstractBlockCreator;
	class BlockMetainfo;
	class ContextBlockMetainfo;
	class Typetable;

	class Metainfo
	{

	public:

		Metainfo( Typetable const& typetable );
		~Metainfo();

		void setAuthor( std::string const& author );
		void setDescription( std::string const& description );
		void setCategory( std::string const& category );
		void setVersion( Version const& version );
		void setContact( std::string const& contact );
		void setInstallDirectory( std::string const& path );
		void setName( std::string const& name );

		ContextBlockMetainfo & setContextBlockCreator( AbstractBlockCreator &obj );
		BlockMetainfo & setBlockCreator( std::string const& blockName, AbstractBlockCreator &obj );

		bool hasContext() const;
		Block & createBlock( std::string const& blockName ) const;
		Block & createContextBlock() const;

		BundleData const& getBundleData() const;

		void cleanup();

	private:

		struct BlockInfo
		{
			BlockInfo() : ctor( nullptr ), data( nullptr ), meta( nullptr ) {}
			BlockInfo( BlockInfo const& src ) : ctor( src.ctor ), data( src.data ), meta( src.meta ) {}
			AbstractBlockCreator	*ctor;
			BlockData				*data;
			BlockMetainfo			*meta;
		};

		struct ContextBlockInfo
		{
			ContextBlockInfo() : ctor( nullptr ), data( nullptr ), meta( nullptr ) {}
			ContextBlockInfo( ContextBlockInfo const& src ) : ctor( src.ctor ), data( src.data ), meta( src.meta ) {}
			AbstractBlockCreator	*ctor;
			BlockData				*data;
			ContextBlockMetainfo	*meta;
		};

		typedef std::map< std::string, BlockInfo >	BlockInfoMap;

		bool										m_HasContext;
		BlockInfoMap								m_BlockInfos;
		ContextBlockInfo							m_ContextInfo;
		BundleData									m_BundleData;
		Typetable const&							m_Typetable;

	};

}