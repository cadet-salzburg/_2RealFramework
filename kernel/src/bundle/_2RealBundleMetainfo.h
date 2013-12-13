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

#include "bundle/_2RealCreationPolicy.h"
#include "bundle/_2RealBlockMetainfo.h"
#include "bundle/_2RealContextBlockMetainfo.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class BundleMetadata;

	namespace bundle
	{
		class FunctionBlockMetainfo;
		class ContextBlockMetainfo;
		class TypeMetainfo;

		class BundleMetainfo
		{

		public:

			explicit BundleMetainfo( std::shared_ptr< BundleMetadata > );

			void setDescription( std::string const& description );
			void setVersion( unsigned int major, unsigned int minor, unsigned int revision );
			void setAuthor( std::string const& author );
			void setContact( std::string const& contact );
			void setCategory( std::string const& category );

			template< typename ContextDerived >
			ContextBlockMetainfo exportContextBlock()
			{
				AbstractBlockCreator *obj = new BlockCreator< ContextDerived, CreateContext >();
				return exportContextBlockInternal( obj );
			}

			template< typename BlockDerived >
			FunctionBlockMetainfo exportFunctionBlock( std::string const& name )
			{
				AbstractBlockCreator *obj = new BlockCreator< BlockDerived, WithoutContext >();
				return exportFunctionBlockInternal( obj, name ); 
			}

			template< typename BlockDerived, template < typename BlockDerived > class Policy >
			FunctionBlockMetainfo exportFunctionBlock( std::string const& name )
			{
				AbstractBlockCreator *obj = new BlockCreator< BlockDerived, Policy >();
				return exportFunctionBlockInternal( obj, name ); 
			}

			TypeMetainfo exportCustomType( std::string const& name );

		private:

			ContextBlockMetainfo	exportContextBlockInternal( AbstractBlockCreator *obj );
			FunctionBlockMetainfo	exportFunctionBlockInternal( AbstractBlockCreator *obj, std::string const& );
			std::weak_ptr< BundleMetadata >		mImpl;

		};
	}
}