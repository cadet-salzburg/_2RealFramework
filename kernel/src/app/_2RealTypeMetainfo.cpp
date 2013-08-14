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

#include "app/_2RealTypeMetainfo.h"
#include "engine/_2RealTypeMetadata.h"
#include "datatypes/_2RealCustomData.h"

namespace _2Real
{
	namespace app
	{
		TypeMetainfo::TypeMetainfo() :
			mImpl()
		{
		}

		TypeMetainfo::TypeMetainfo( std::shared_ptr< const TypeMetadata > type ) :
			mImpl( type )
		{
		}

		bool TypeMetainfo::isValid() const
		{
			std::shared_ptr< const TypeMetadata > meta = mImpl.lock();
			return ( meta.get() != nullptr );
		}

		void TypeMetainfo::getDataFields( DataFields &fields ) const
		{
			std::shared_ptr< const TypeMetadata > meta = checkValidity< const TypeMetadata >( mImpl, "type metainfo" );
			meta->getDataFields( fields );
		}

		std::string const& TypeMetainfo::getTypename() const
		{
			std::shared_ptr< const TypeMetadata > meta = checkValidity< const TypeMetadata >( mImpl, "type metainfo" );
			return meta->getHumanReadableName();
		}

		std::shared_ptr< CustomType > TypeMetainfo::makeData() const
		{
			std::shared_ptr< const TypeMetadata > meta = checkValidity< const TypeMetadata >( mImpl, "type metainfo" );
			return std::shared_ptr< CustomType >( new CustomType( meta ) );
		}
	}
}