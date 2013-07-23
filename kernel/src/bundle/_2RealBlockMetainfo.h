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

#include "datatypes/_2RealTypes.h"
#include "datatypes/_2RealCustomBase.h"
#include "datatypes/_2RealCustomData.h"

#include "engine/_2RealInletPolicy.h"
#include "engine/_2RealThreadingPolicy.h"


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

			template< typename TType >
			void addInlet( std::string const& name, TType initialValue = Init< TType >::defaultValue(), InletPolicy const& defaultPolicy = InletPolicy::ALWAYS )
			{
				privateAddInlet( name, Name< TType >::humanReadableName(), BaseToCustomType< TType >( initialValue ), BaseToCustomType< TType >::getTypeMetadata(), defaultPolicy );
			}

			void addCustomTypeInlet( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init, InletPolicy const& defaultPolicy = InletPolicy::ALWAYS );

			template< typename TType >
			void addParameter( std::string const& name, TType initialValue = Init< TType >::defaultValue() )
			{
				privateAddParameter( name, Name< TType >::humanReadableName(), BaseToCustomType< TType >( initialValue ), BaseToCustomType< TType >::getTypeMetadata() );
			}

			void addCustomTypeParameter( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init );

			template< typename TType >
			void addOutlet( std::string const& name )
			{
				privateAddOutlet( name, Name< TType >::humanReadableName(), BaseToCustomType< TType >::getTypeMetadata() );
			}

			void addCustomTypeOutlet( std::string const& name, std::string const& type );

		private:

			void privateAddInlet( std::string const&, std::string const&, std::shared_ptr< const CustomType >, TypeMetadata const*, InletPolicy const& );
			void privateAddParameter( std::string const&, std::string const&, std::shared_ptr< const CustomType >, TypeMetadata const* );
			void privateAddOutlet( std::string const&, std::string const&, TypeMetadata const* );
			BlockMetadata	&m_Impl;

		};
	}
}
