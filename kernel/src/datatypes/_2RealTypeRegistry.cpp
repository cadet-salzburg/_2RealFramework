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
#include "helpers/_2RealConstants.h"

namespace _2Real
{
	void TypeRegistry::registerType( std::shared_ptr< const TypeMetadata > data )
	{
		mTypes[ data->getIdentifier()->getObjectName() ] = data;
	}

	//void TypeRegistry::unregisterType( std::string const& name )
	//{
	//	Types::iterator it = mTypes.find( name );
	//	mTypes.erase( it );
	//}

	std::shared_ptr< const TypeMetadata > TypeRegistry::get( std::string const& name ) const
	{
		Types::const_iterator userIt = mTypes.find( name );
		if ( userIt != mTypes.end() )
			return userIt->second;
		else
			return nullptr;
	}

	//void TypeRegistry::merge( TypeRegistry const& other, std::string const& bundle, std::string const& alias )
	//{
	//	for ( Types::const_iterator it = other.mTypes.begin(); it != other.mTypes.end(); ++it )
	//	{
	//		if ( it->first.first == bundle )
	//		{
	//			std::shared_ptr< const TypeMetadata > m = it->second;
	//			registerType( alias, it->first.second, m );
	//		}
	//	}
	//}
}