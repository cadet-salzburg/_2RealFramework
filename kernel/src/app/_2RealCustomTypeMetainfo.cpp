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
#include "app/_2RealTypeMetainfo.h"
#include "engine/_2RealTypeMetainfoImpl_I.h"
#include "engine/_2RealBasicTypeMetainfoImpl.h"
#include "engine/_2RealCustomTypeMetainfoImpl.h"
#include "common/_2RealWeakPtrCheck.h"

namespace _2Real
{
	namespace app
	{
		CustomTypeMetainfo::CustomTypeMetainfo( std::shared_ptr< const CustomTypeMetainfoImpl > meta ) :
			TypeMetainfo_I(),
			mImpl( meta )
		{
		}

		bool CustomTypeMetainfo::isValid() const
		{
			std::shared_ptr< const CustomTypeMetainfoImpl > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		bool CustomTypeMetainfo::isBasicType() const
		{
			std::shared_ptr< const CustomTypeMetainfoImpl > meta = checkValidity< const CustomTypeMetainfoImpl >( mImpl, "custom type metainfo" );
			return false;
		}

		DataItem CustomTypeMetainfo::makeData() const
		{
			std::shared_ptr< const CustomTypeMetainfoImpl > meta = checkValidity< const CustomTypeMetainfoImpl >( mImpl, "custom type metainfo" );
			return meta->makeData();
		}

		std::string CustomTypeMetainfo::getName() const
		{
			std::shared_ptr< const CustomTypeMetainfoImpl > meta = checkValidity< const CustomTypeMetainfoImpl >( mImpl, "custom type metainfo" );
			return meta->getName();
		}

		std::string CustomTypeMetainfo::getDescription() const
		{
			std::shared_ptr< const CustomTypeMetainfoImpl > meta = checkValidity< const CustomTypeMetainfoImpl >( mImpl, "custom type metainfo" );
			return meta->getDescription();
		}

		CustomDataItem CustomTypeMetainfo::makeCustomData() const
		{
			std::shared_ptr< const CustomTypeMetainfoImpl > meta = checkValidity< const CustomTypeMetainfoImpl >( mImpl, "custom type metainfo" );
			return meta->makeCustomData();
		}

		std::vector< std::pair< std::string, std::shared_ptr< const TypeMetainfo_I > > > CustomTypeMetainfo::getDataFields() const
		{
			std::shared_ptr< const CustomTypeMetainfoImpl > meta = checkValidity< const CustomTypeMetainfoImpl >( mImpl, "custom type metainfo" );
			std::vector< std::pair< std::string, std::shared_ptr< const TypeMetainfo_I > > > result;
			std::vector< std::pair< std::string, std::shared_ptr< const TypeMetainfoImpl_I > > > tmp = meta->getDataFields();
			for ( auto const& it : tmp )
			{
				if ( it.second->isBasicType() )
				{
					std::shared_ptr< const TypeMetainfo > info( new TypeMetainfo( std::static_pointer_cast< const BasicTypeMetainfoImpl >( it.second ) ) );
					result.push_back( std::make_pair( it.first, info ) );
				}
				else
				{
					std::shared_ptr< const CustomTypeMetainfo > info( new CustomTypeMetainfo( std::static_pointer_cast< const CustomTypeMetainfoImpl >( it.second ) ) );
					result.push_back( std::make_pair( it.first, info ) );
				}
			}
			return result;
		}
	}
}