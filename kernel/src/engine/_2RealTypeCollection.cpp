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
#include "engine/_2RealId.h"
#include "helpers/_2RealConstants.h"

namespace _2Real
{
	bool TypeCollection::Cmp::operator()( std::shared_ptr< const MetainfoId > one, std::shared_ptr< const MetainfoId > other ) const
	{
		assert( one.get() );
		assert( other.get() );

		return ( *one.get() < *other.get() );
	}

	TypeCollection::~TypeCollection()
	{
		clear();
	}

	void TypeCollection::clear()
	{
		mTypes.clear();
	}

	void TypeCollection::addType( std::shared_ptr< const TMetainfo > type )
	{
		if ( mTypes.find( type->getId() ) != mTypes.end() )
		{
			std::ostringstream msg;
			msg << "type " << type->getId()->toString() << " already exists" << std::endl;
			throw AlreadyExistsException( msg.str() );
		}

		mTypes[ type->getId() ] = type;
	}

	void TypeCollection::typeRemoved( std::shared_ptr< const MetainfoId > info )
	{
		auto it = mTypes.find( info );
		mTypes.erase( it );
	}

	std::shared_ptr< const TMetainfo > TypeCollection::getMetainfo( std::shared_ptr< const MetainfoId > id ) const
	{
		auto it = mTypes.find( id );
		if ( it != mTypes.end() )
			return it->second;
		else
			return nullptr;
	}

	//std::vector< std::shared_ptr< const SharedTypeMetainfo > > TypeCollection::getTypes() const
	//{
	//	std::vector< std::shared_ptr< const SharedTypeMetainfo > > result;

	//	for ( auto it : mTypes )
	//	{
	//		if ( it.second->isBasicType() )
	//		{
	//			std::cout << "basic typemetainfo in get typemetainfo!" << std::endl;
	//		}
	//		else
	//		{
	//			std::shared_ptr< const SharedTypeMetainfo > info = std::dynamic_pointer_cast< const SharedTypeMetainfo >( it.second );
	//			result.push_back( info );
	//		}
	//	}

	//	return result;
	//}
}