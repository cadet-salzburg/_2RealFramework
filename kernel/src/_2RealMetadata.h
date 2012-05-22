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

#include "_2RealEngineData.h"
#include "_2RealSingletonHolder.h"
#include "_2RealBundleData.h"
#include "_2RealBlockData.h"
#include "_2RealBlock.h"

#include <typeinfo.h>
#include <string>
#include <iostream>

namespace _2Real
{

	class Bundle;
	class BundleContext;
	class Block;
	class FrameworkContext;
	class Typetable;

	class AbstractBlockCreator
	{

	public:

		virtual ~AbstractBlockCreator()
		{
		}

		virtual Block & create() const = 0;

	};

	template< typename BlockDerived >
	class CreationPolicy
	{

	public:

		virtual ~CreationPolicy()
		{
		}

		virtual Block & create() const = 0;

	};

	template< typename BlockDerived >
	class NoSingleton : public CreationPolicy< BlockDerived >
	{

	public:

		~NoSingleton()
		{
		}

		Block & create() const
		{
			return *( new BlockDerived() );
		}

	};

	template< typename BlockDerived >
	class Singleton : public CreationPolicy< BlockDerived >
	{

	public:

		~Singleton()
		{
		}

		Block & create() const
		{
			return m_Holder.instance();
		}

	private:

		mutable SingletonHolder< BlockDerived >	m_Holder;

	};

	template< typename BlockDerived, template < typename BlockDerived > class CreationPolicy >
	class BlockCreator : public AbstractBlockCreator
	{

	public:

		~BlockCreator()
		{
		}

		Block & create() const
		{
			return m_Policy.create();
		}

	private:

		CreationPolicy< BlockDerived >	m_Policy;

	};

	class Metainfo;
	class BundleData;
	class BundleMetainfo;
	class BlockMetainfo;

	class BlockMetainfo
	{

	public:

		BlockMetainfo( BlockData &data, Typetable const& typetable );
		BlockMetainfo( BlockMetainfo const& src );
		~BlockMetainfo();

		template< typename DataType >
		void addParameter( std::string const& paramName, DataType defaultValue )
		{
			EngineData data( defaultValue );
			addParameterInternal( setupName, data );
		}

		template< typename DataType >
		void addInlet( std::string const& inletName, DataType defaultValue )
		{
			EngineData data( defaultValue );
			addInletInternal( inletName, data );
		}

		template< typename DataType >
		void addOutlet( std::string const& outletName, DataType defaultValue )
		{
			EngineData data( defaultValue );
			addOutletInternal( outletName, defaultValue );
		}

		void setDescription( std::string const& description );

	private:

		void	addSetupParameterInternal( std::string const& paramName, EngineData const& defaultValue );
		void	addInletInternal( std::string const& inletName, EngineData const& defaultValue );
		void	addOutletInternal( std::string const& outletName, EngineData const& defaultValue );

		Typetable		const& m_Typetable;
		BlockData		&m_Impl;

	};

	class BundleMetainfo
	{

	public:

		BundleMetainfo( Metainfo &info, Typetable const& typetable );
		BundleMetainfo( BundleMetainfo const& src );
		~BundleMetainfo();

		template< typename BlockDerived >
		void exportBundleContext()
		{
			AbstractBlockCreator *obj = new BlockCreator< BlockDerived, Singleton >();
			setBundleContextInternal( *obj );
		}

		template< typename DataType >
		void addContextParameter( std::string const& paramName, DataType defaultValue )
		{
			EngineData data(defaultValue);
			addContextParameterInternal( paramName, data );
		}

		void setDescription(std::string const& description);
		void setVersion(unsigned int major, unsigned int minor, unsigned int revision);
		void setAuthor(std::string const& author);
		void setContact(std::string const& contact);
		void setCategory(std::string const& category);

		template< typename BlockDerived >
		BlockMetainfo exportBlock( std::string const& blockName )
		{
			AbstractBlockCreator *obj = new BlockCreator< BlockDerived, NoSingleton >();
			return addExportedBlockInternal( *obj, blockName ); 
		}

	private:

		void			setBundleContextInternal( AbstractBlockCreator &obj );
		BlockMetainfo	addExportedBlockInternal( AbstractBlockCreator &obj, std::string const& blockName );
		void			addContextParameterInternal( std::string const& paramName, EngineData const& defaultValue );

		Typetable		const& m_Typetable;
		Metainfo		&m_Impl;

	};

	class Metainfo
	{

	public:

		Metainfo( BundleData &data );
		~Metainfo();

		void setAuthor( std::string const& author );
		void setDescription( std::string const& description );
		void setCategory( std::string const& category );
		void setVersion( Version const& version );
		void setContact( std::string const& contact );
		void setInstallDirectory( std::string const& path );
		void setName( std::string const& name );

		void setContextCreator( AbstractBlockCreator &obj );
		void addContextParameter( ParameterData &data );
		BlockData & addExportedBlock( AbstractBlockCreator &obj, std::string const& blockName );

		bool hasContext() const;
		Block & createBlock( std::string const& blockName ) const;
		//Block & createBundleContext() const;

		void postProcess();

	private:

		struct BlockInfo
		{
			BlockInfo() : ctor( NULL ), data( NULL ) {}
			BlockInfo( BlockInfo const& src ) : ctor( src.ctor ), data( src.data ) {}
			AbstractBlockCreator	*ctor;
			BlockData				*data;
		};

		typedef std::map< std::string, BlockInfo >	BlockInfoMap;

		bool					m_HasContext;
		BlockInfoMap			m_ExportedBlocks;
		BlockInfo				m_ContextInfo;
		BundleData				*m_BundleData;

	};

}