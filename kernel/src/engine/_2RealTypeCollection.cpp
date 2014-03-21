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

#include "engine/_2RealTypeCollection.h"
#include "engine/_2RealSharedTypeMetainfo.h"
#include "helpers/_2RealConstants.h"

namespace _2Real
{
	TypeCollection::TypeCollection()
	{
	}

	TypeCollection::~TypeCollection()
	{
		clear();
	}

	void TypeCollection::clear()
	{
		//for ( auto it = mTypes.begin(); it != mTypes.end(); )
		//{
		//	it->second->unregisterFromRemove( this, &TypeCollection::typeRemoved );
		//	it = mTypes.erase( it );
		//	// careful... type may be stored in more than one collection, i.e. local & global
		//}
	}

	//void TypeCollection::addType( std::shared_ptr< SharedTypeMetainfo > info )
	//{
	//	mTypes[ info->getName() ] = info;
	//}

	//void TypeCollection::typeRemoved( std::shared_ptr< const SharedTypeMetainfo > info )
	//{
	//	Types::iterator it = mTypes.find( info->getName() );
	//	mTypes.erase( it );
	//}

	//std::shared_ptr< const SharedTypeMetainfo > TypeCollection::get( std::string const& name ) const
	//{
	//	auto it = mTypes.find( name );
	//	if ( it != mTypes.end() )
	//		return it->second;
	//	else
	//		return nullptr;
	//}
}