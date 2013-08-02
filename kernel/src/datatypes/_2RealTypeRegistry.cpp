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
#include "engine/_2RealTypeMetadata.h"

namespace _2Real
{
	const std::string TypeRegistry::sFrameworkTypes = "FrameworkType";

	TypeRegistry::TypeRegistry()
	{
	}

	TypeRegistry::~TypeRegistry()
	{
		mTypes.clear();
	}

	void TypeRegistry::registerType( std::string const& bundle, std::string const& name, std::shared_ptr< const TypeMetadata > data )
	{
		std::pair< std::string, std::string > key = std::make_pair( bundle, name );
#ifdef _DEBUG
		Types::const_iterator it = mTypes.find( key );
		assert( mTypes.end() == it );
#endif
		mTypes[ key ] = data;
	}

	void TypeRegistry::unregisterType( std::string const& bundle, std::string const& name )
	{
		std::pair< std::string, std::string > key = std::make_pair( bundle, name );
		Types::iterator it = mTypes.find( key );
#ifdef _DEBUG
		assert( it != mTypes.end() );
#endif
		mTypes.erase( it );
	}

	std::shared_ptr< const TypeMetadata > TypeRegistry::get( std::string const& bundle, std::string const& name ) const
	{
		Types::const_iterator fwIt = mTypes.find( std::make_pair( TypeRegistry::sFrameworkTypes, name ) );
		if ( fwIt != mTypes.end() )
		{
			return fwIt->second;
		}
		else
		{
			Types::const_iterator userIt = mTypes.find( std::make_pair( bundle, name ) );
			if ( userIt != mTypes.end() )
				return userIt->second;
			else
				return nullptr;
		}
	}

	void TypeRegistry::merge( TypeRegistry const& other, std::string const& bundle, std::string const& alias )
	{
		for ( Types::const_iterator it = other.mTypes.begin(); it != other.mTypes.end(); ++it )
		{
			if ( it->first.first == bundle )
			{
				std::shared_ptr< const TypeMetadata > m = it->second;
				registerType( alias, it->first.second, m );
			}
		}
	}
}