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

#include "app/_2RealCustomTypeMetainfo.h"
#include "app/_2RealHandleValidity.h"
#include "engine/_2RealSharedTypeMetainfo.h"

namespace _2Real
{
	namespace app
	{

		CustomTypeMetainfo::CustomTypeMetainfo( std::shared_ptr< const SharedTypeMetainfo > meta ) :
			TypeMetainfo( meta ),
			mImpl( meta )
		{
		}

		std::string CustomTypeMetainfo::getDescription() const
		{
			std::shared_ptr< const SharedTypeMetainfo > meta = checkValidity< const SharedTypeMetainfo >( mImpl, "custom type metainfo" );
			return meta->getDescription();
		}

		CustomDataItem CustomTypeMetainfo::makeCustomData() const
		{
			std::shared_ptr< const SharedTypeMetainfo > meta = checkValidity< const SharedTypeMetainfo >( mImpl, "custom type metainfo" );
			return meta->makeCustomData();
		}

		std::vector< std::pair< std::string, std::shared_ptr< const TypeMetainfo > > > CustomTypeMetainfo::getDataFields() const
		{
			std::shared_ptr< const SharedTypeMetainfo > meta = checkValidity< const SharedTypeMetainfo >( mImpl, "custom type metainfo" );
			std::vector< std::pair< std::string, std::shared_ptr< const TypeMetainfo > > > result;
			std::vector< std::pair< std::string, std::shared_ptr< const TMetainfo > > > tmp = meta->getDataFields();
			for ( auto const& it : tmp )
			{
				if ( it.second->isBasicType() )
				{
					std::shared_ptr< TypeMetainfo > info( new TypeMetainfo( it.second ) );
					result.push_back( std::make_pair( it.first, info ) );
				}
				else
				{
					std::shared_ptr< const SharedTypeMetainfo > shared = std::dynamic_pointer_cast< const SharedTypeMetainfo >( it.second );
					std::shared_ptr< const CustomTypeMetainfo > info( new CustomTypeMetainfo( shared ) );
					result.push_back( std::make_pair( it.first, info ) );
				}
			}
			return result;
		}

	}
}