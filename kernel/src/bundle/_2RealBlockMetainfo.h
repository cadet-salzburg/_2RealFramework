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

#include "policies/_2RealUpdatePolicy.h"
#include "policies/_2RealThreadingPolicy.h"

#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class BlockMetadata;

	namespace bundle
	{
		class FunctionBlockMetainfo
		{

		public:

			FunctionBlockMetainfo( std::shared_ptr< BlockMetadata > );

			void setDescription( std::string const& description );
			void setCategory( std::string const& category );
			void setThreadingPolicy( ThreadingPolicy const& policy );

			template< typename TType >
			void addInlet( std::string const& name, TType initialValue = Init< TType >::defaultValue(), UpdatePolicy const& defaultPolicy = UpdatePolicy::ALWAYS )
			{
				privateAddInlet( name, Name< TType >::humanReadableName(), BaseToCustomType< TType >( initialValue ), BaseToCustomType< TType >::getTypeMetadata(), defaultPolicy );
			}

			void addCustomTypeInlet( std::string const& name, std::string const& type, std::shared_ptr< const CustomType > init, UpdatePolicy const& defaultPolicy = UpdatePolicy::ALWAYS );

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

			void privateAddInlet( std::string const&, std::string const&, std::shared_ptr< const CustomType >, std::shared_ptr< const TypeMetadata >, UpdatePolicy const& );
			void privateAddParameter( std::string const&, std::string const&, std::shared_ptr< const CustomType >, std::shared_ptr< const TypeMetadata > );
			void privateAddOutlet( std::string const&, std::string const&, std::shared_ptr< const TypeMetadata > );
			std::weak_ptr< BlockMetadata >		mImpl;

		};
	}
}
