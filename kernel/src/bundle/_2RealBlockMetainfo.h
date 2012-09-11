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

#include "helpers/_2RealAny.h"
#include "datatypes/_2RealTypes.h"
#include "helpers/_2RealOptions.h"

#ifdef _UNIX
	#include <typeinfo>
#else
	#include <typeinfo.h>
#endif
#include <string>

namespace _2Real
{
	class BlockMetadata;

	namespace bundle
	{
		class BlockMetainfo
		{

		public:

			BlockMetainfo( BlockMetadata &data );

			void setDescription( std::string const& description );
			void setCategory( std::string const& category );

			template< typename TData >
			void addInlet( std::string const& name, TData initialValue )
			{
				TypeDescriptor *d = createTypeDescriptor< TData >();
				addInletInternal( name, *d, Any( initialValue ) );
			}

			template< typename TData >
			void addInlet( std::string const& name, TData initialValue, Options< TData > const& options );

			template< typename TData >
			void addMultiInlet( std::string const& name, TData initialValue )
			{
				TypeDescriptor *d = createTypeDescriptor< TData >();
				addMultiInletInternal( name, *d, Any( initialValue ) );
			}

			template< typename TData >
			void addMultiInlet( std::string const& name, TData initialValue, Options< TData > const& options );

			template< typename TData >
			void addOutlet( std::string const& name )
			{
				TypeDescriptor *d = createTypeDescriptor< TData >();
				TData data;
				Any init( data );
				addOutletInternal( name, *d, init );
			}

		private:

			void		addInletInternal( std::string const& name, TypeDescriptor &descriptor, Any const& init );
			void		addInletInternal( std::string const& name, TypeDescriptor &descriptor, Any const& init, AnyOptionSet const& options );
			void		addMultiInletInternal( std::string const& name, TypeDescriptor &descriptor, Any const& init );
			void		addMultiInletInternal( std::string const& name, TypeDescriptor &descriptor, Any const& init, AnyOptionSet const& options );
			void		addOutletInternal( std::string const& name, TypeDescriptor &descriptor, Any const& init );

			BlockMetadata	&m_Impl;

		};

		template< >
		inline void BlockMetainfo::addInlet< int >( std::string const& name, int initialValue, Options< int > const& options )
		{
			TypeDescriptor *d = createTypeDescriptor< int >();
			addInletInternal( name, *d, Any( initialValue ), AnyOptionSet( options ) );
		}

		template< >
		inline void BlockMetainfo::addMultiInlet< int >( std::string const& name, int initialValue, Options< int > const& options )
		{
			TypeDescriptor *d = createTypeDescriptor< int >();
			addMultiInletInternal( name, *d, Any( initialValue ), AnyOptionSet( options ) );
		}
	}
}
