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

#include <string>
#include <map>

namespace _2Real
{
	class TypeMetadata;

	class TypeRegistry
	{

	public:

		TypeRegistry() {}
		~TypeRegistry() { mTypes.clear(); }

		void registerType( std::string const& bundle, std::string const& name, TypeMetadata &meta );
		void unregisterType( std::string const& bundle, std::string const& name, TypeMetadata &meta );
		TypeMetadata const& getType( std::string const& bundle, std::string const& name ) const;

	private:

		TypeRegistry( TypeRegistry const& other );
		TypeRegistry& operator=( TypeRegistry const& other );

		typedef std::pair< std::string, std::string >	TypeKey;
		typedef std::map< TypeKey, TypeMetadata * >		Types;

		Types			mTypes;

	};
}
