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

#include "helpers/_2RealDeleter.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class TypeMetadata;

	class TypeRegistry
	{

	public:

		const static std::string sFrameworkTypes;

		TypeRegistry();
		~TypeRegistry();

		void registerType( std::string const& bundle, std::string const& name, std::shared_ptr< const TypeMetadata >/*, ADeleter< TypeMetadata > *del*/ );
		void unregisterType( std::string const& bundle, std::string const& name );
		std::shared_ptr< const TypeMetadata > get( std::string const& bundle, std::string const& name ) const;

		void merge( TypeRegistry const& other, std::string const& bundle, std::string const& alias );

	private:

		TypeRegistry( TypeRegistry const& other );
		TypeRegistry& operator=( TypeRegistry const& other );

		typedef std::pair< std::string, std::string >						TypeKey;
		typedef std::map< TypeKey, std::shared_ptr< const TypeMetadata > >	Types;

		Types																mTypes;
	};
}
