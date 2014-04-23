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
#include "engine/_2RealTypeMetainfoImpl_I.h"
#include "engine/_2RealCustomTypeMetainfoImpl.h"
#include "engine/_2RealId.h"

namespace _2Real
{
	class TypeByName
	{
	public:
		explicit TypeByName( const std::string name ) : mBaseline( name ) {}
		bool operator()( std::pair< Connection, std::shared_ptr< const TypeMetainfoImpl_I > > const& val ) const
		{
			assert( val.second.get() );
			return mBaseline == val.second->getId()->toString();
		}
	private:
		std::string mBaseline;
	};

	class TypeByAddress
	{
	public:
		explicit TypeByAddress( std::shared_ptr< const TypeMetainfoImpl_I > type ) : mBaseline( type ) { assert( type.get() ); }
		bool operator()( std::pair< Connection, std::shared_ptr< const TypeMetainfoImpl_I > > const& val ) const
		{
			assert( val.second.get() );
			return mBaseline.get() == val.second.get();
		}
	private:
		std::shared_ptr< const TypeMetainfoImpl_I > mBaseline;
	};

	void TypeCollection::clear()
	{
		for ( auto it : mTypes )
			it.first.disconnect();
		mTypes.clear();
	}

	void TypeCollection::addType( std::shared_ptr< const TypeMetainfoImpl_I > type )
	{
		auto it = std::find_if( mTypes.begin(), mTypes.end(), TypeByName( type->getId()->toString() ) );
		if ( it != mTypes.end() )
		{
			std::ostringstream msg;
			msg << "type " << type->getId()->toString() << " already exists" << std::endl;
			throw AlreadyExists( msg.str() );
		}

		if ( !type->isBasicType() )
		{
			auto custom = std::static_pointer_cast< const CustomTypeMetainfoImpl >( type );
			auto connection = custom->registerToDestroyed( std::bind( &TypeCollection::typeDestroyed, this, std::placeholders::_1 ) );
			mTypes.push_back( std::make_pair( connection, type ) );
		}
		else
		{
			mTypes.push_back( std::make_pair( Connection(), type ) );
		}
	}

	void TypeCollection::typeDestroyed( std::shared_ptr< const TypeMetainfoImpl_I > type )
	{
		auto typeIter = std::find_if( mTypes.begin(), mTypes.end(), TypeByAddress( type ) );
		if ( typeIter != mTypes.end() )
		{
			typeIter->first.disconnect();
			mTypes.erase( typeIter );
		}
	}

	std::shared_ptr< const TypeMetainfoImpl_I > TypeCollection::getTypeMetainfo( const std::string name ) const
	{
		auto it = std::find_if( mTypes.begin(), mTypes.end(), TypeByName( name ) );
		if ( it != mTypes.end() )
			return it->second;
		else
			return nullptr;
	}
}