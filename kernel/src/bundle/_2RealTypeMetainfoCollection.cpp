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

#include "bundle/_2RealTypeMetainfoCollection.h"
#include "bundle/_2RealTypeMetainfo_I.h"
#include "bundle/_2RealCustomTypeMetainfo.h"
#include "bundle/_2RealTypeMetainfo.h"
#include "engine/_2RealTypeCollection.h"
#include "engine/_2RealTypeMetainfoImpl_I.h"
#include "engine/_2RealBasicTypeMetainfoImpl.h"
#include "engine/_2RealCustomTypeMetainfoImpl.h"

namespace _2Real
{
	namespace bundle
	{
		TypeMetainfoCollection::TypeMetainfoCollection( std::shared_ptr< TypeCollection > impl ) :
			mImpl( impl )
		{
		}

		std::shared_ptr< const TypeMetainfo_I > TypeMetainfoCollection::getTypeMetainfo( std::string const& name ) const
		{
			auto type = mImpl->getTypeMetainfo( name );
			if ( type->isBasicType() )
			{
				auto basictype = std::static_pointer_cast< BasicTypeMetainfoImpl >( type );
				return std::shared_ptr< const TypeMetainfo_I >(  new TypeMetainfo( basictype ) );
			}
			else
			{
				auto customtype = std::static_pointer_cast< CustomTypeMetainfoImpl >( type );
				return std::shared_ptr< const TypeMetainfo_I >(  new CustomTypeMetainfo( customtype ) );
			}
		}
	}
}