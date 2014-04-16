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
#include "engine/_2RealId.h"

namespace _2Real
{
	class TypeNameCmp : public std::unary_function< bool, std::shared_ptr< const TypeMetainfoImpl_I > >
	{

	public:

		explicit TypeNameCmp( const std::string name ) : mName( name )
		{
		}

		bool operator()( std::shared_ptr< const TypeMetainfoImpl_I > type ) const
		{
			return mName == type->getId()->toString();
		}

	private:

		std::string mName;

	};

	void TypeCollection::addType( std::shared_ptr< const TypeMetainfoImpl_I > type )
	{
		auto it = std::find_if( mTypes.begin(), mTypes.end(), TypeNameCmp( type->getId()->toString() ) );
		if ( it != mTypes.end() )
		{
			std::ostringstream msg;
			msg << "type " << type->getId()->toString() << " already exists" << std::endl;
			throw AlreadyExists( msg.str() );
		}

		mTypes.push_back( type );
	}

	void TypeCollection::typeRemoved( std::shared_ptr< const TypeMetainfoImpl_I > type )
	{
		auto it = std::find_if( mTypes.begin(), mTypes.end(), TypeNameCmp( type->getId()->toString() ) );
		if ( it != mTypes.end() )
			mTypes.erase( it );
	}

	std::shared_ptr< const TypeMetainfoImpl_I > TypeCollection::getTypeMetainfo( const std::string name ) const
	{
		auto it = std::find_if( mTypes.begin(), mTypes.end(), TypeNameCmp( name ) );
		if ( it != mTypes.end() )
			return *it;
		else
			return nullptr;
	}
}