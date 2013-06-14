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

#include "datatypes/_2RealTypeRegistry.h"
#include <assert.h>

namespace _2Real
{
	void TypeRegistry::registerType( std::string const& bundle, std::string const& name, TypeMetadata &meta )
	{
		std::pair< std::string, std::string > key = std::make_pair( bundle, name );
		mTypes[ key ] = &meta;
	}

	void TypeRegistry::unregisterType( std::string const& bundle, std::string const& name, TypeMetadata &meta )
	{
		std::pair< std::string, std::string > key = std::make_pair( bundle, name );
		Types::iterator it = mTypes.find( key );
#ifdef _DEBUG
		assert( it != mTypes.end() );
#endif
		mTypes.erase( it );
	}

	TypeMetadata const& TypeRegistry::getType( std::string const& bundle, std::string const& name ) const
	{
		std::pair< std::string, std::string > key = std::make_pair( bundle, name );
		Types::const_iterator it = mTypes.find( key );
#ifdef _DEBUG
		assert( it != mTypes.end() );
#endif
		return *( it->second );
	}
}