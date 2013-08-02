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
#include "engine/_2RealBlockMetadata.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class TypeMetadata;
	class TypeRegistry;

	namespace bundle
	{
		class Block;
		class ContextBlock;
		class AbstractBlockCreator;
	}

	typedef std::string BundleId;

	class Metainfo
	{

	public:

		Metainfo( BundleId const&, TypeRegistry * );
		~Metainfo();

		bool hasContext() const;
		bool exportsContext() const;

		unsigned int getCreationCount( std::string const& ) const;

		std::shared_ptr< BlockMetadata >	exportContextBlock( bundle::AbstractBlockCreator * );
		std::shared_ptr< BlockMetadata >	exportFunctionBlock( bundle::AbstractBlockCreator *, std::string const& );
		std::shared_ptr< TypeMetadata >		exportCustomType( std::string const& );

		std::shared_ptr< _2Real::bundle::Block > createFunctionBlock( std::string const& ) const;
		std::shared_ptr< _2Real::bundle::Block > createContextBlock() const;

		std::shared_ptr< BundleMetadata >			getBundleMetadata();
		std::shared_ptr< const BlockMetadata >		getContextBlockMetadata() const;
		std::shared_ptr< const BlockMetadata >		getFunctionBlockMetadata( std::string const& ) const;

		void getExportedBlocks( std::vector< std::shared_ptr< const BlockMetadata > > & ) const;

		void postLoading( TypeRegistry * );

	private:

		struct BlockInfo
		{
			BlockInfo() : ctor( nullptr ), metadata() {}
			BlockInfo( BlockInfo const& src ) : ctor( src.ctor ), metadata( src.metadata ) {}
			bundle::AbstractBlockCreator		*ctor;
			std::shared_ptr< BlockMetadata >	metadata;
		};

		static const std::string sContextBlockName;

		typedef std::map< std::string, BlockInfo >					BlockInfos;
		typedef std::map< std::string, BlockInfo >::iterator		BlockInfoIterator;
		typedef std::map< std::string, BlockInfo >::const_iterator	BlockInfoConstIterator;

		BlockInfos									mBlockMetadata;
		std::shared_ptr< BundleMetadata	>			mBundleMetadata;
		BundleId									mBundleId;

		TypeRegistry								*const mTypes;
		TypeRegistry								*const mFrameworkTypes;

	};
}