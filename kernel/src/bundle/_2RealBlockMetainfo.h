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
#include "engine/_2RealInletPolicy.h"
#include "engine/_2RealThreadingPolicy.h"

#include "datatypes/_2RealCustomData.h"

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
			void setThreadingPolicy( ThreadingPolicy const& policy );

			// this represents a shortcut for adding some kinds of inlets
			// like char, int, string etc... internally, everything is going to be a custom type
			// overload manually
			template< typename TType >
			void addInlet( std::string const& name, TType initialValue = init< TType >(), InletPolicy const& defaultPolicy = InletPolicy::ALWAYS );

			void addInlet( std::string const& name, std::string const& customName, CustomType const& initialValue, InletPolicy const& defaultPolicy = InletPolicy::ALWAYS );
			void addOutlet( std::string const& name, std::string const& customName, CustomType const& initialValue );

			//template< typename TData >
			//void addInlet( std::string const& name, TData initialValue, InletPolicy const& defaultPolicy = InletPolicy::ALWAYS )
			//{
			//	addInletInternal( name, createTypeDescriptor< TData >(), Any( initialValue ), AnyOptionSet(), defaultPolicy );
			//}

			//template< typename TData >
			//void addInlet( std::string const& name, TData initialValue, Options< TData > const& options, InletPolicy const& defaultPolicy = InletPolicy::ALWAYS );

			//template< typename TData >
			//void addMultiInlet( std::string const& name, TData initialValue, InletPolicy const& defaultPolicy = InletPolicy::ALWAYS )
			//{
			//	addMultiInletInternal( name, createTypeDescriptor< TData >(), Any( initialValue ), AnyOptionSet(), defaultPolicy );
			//}

			//template< typename TData >
			//void addMultiInlet( std::string const& name, TData initialValue, Options< TData > const& options, InletPolicy const& defaultPolicy = InletPolicy::ALWAYS );

			//template< typename TData >
			//void addOutlet( std::string const& name )
			//{
			//	addOutletInternal( name, createTypeDescriptor< TData >(), Any( TData() ) );
			//}

		private:

			//void addInletInternal( std::string const& n, TypeDescriptor const *const t, Any const& i, AnyOptionSet const& o, InletPolicy const& p );
			//void addMultiInletInternal( std::string const& n, TypeDescriptor const *const t, Any const& i, AnyOptionSet const& o, InletPolicy const& p );
			//void addOutletInternal( std::string const& n, TypeDescriptor const *const t, Any const& i );

			BlockMetadata	&m_Impl;

		};

		template< >
		inline void BlockMetainfo::addInlet< int >( std::string const& name, int initialValue, InletPolicy const& defaultPolicy )
		{
			// so: create custom type
			// with a single int field
			// init it to 'initialValue'
			//CustomType c( initialValue );

			// create integer type metainfo
			// add an inlet based of the integer type
		}

		//template< >
		//inline void BlockMetainfo::addInlet< int >( std::string const& name, int initialValue, Options< int > const& options, InletPolicy const& defaultPolicy )
		//{
		//	addInletInternal( name, createTypeDescriptor< int >(), Any( initialValue ), AnyOptionSet( options ), defaultPolicy );
		//}

		//template< >
		//inline void BlockMetainfo::addMultiInlet< int >( std::string const& name, int initialValue, Options< int > const& options, InletPolicy const& defaultPolicy )
		//{
		//	addMultiInletInternal( name, createTypeDescriptor< int >(), Any( initialValue ), AnyOptionSet( options ), defaultPolicy );
		//}
	}
}
