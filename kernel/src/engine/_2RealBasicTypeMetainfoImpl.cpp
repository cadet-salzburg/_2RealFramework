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

#include "engine/_2RealBasicTypeMetainfoImpl.h"
#include "engine/_2RealId.h"
#include "engine/_2RealHumanReadableNameVisitor.h"
#include "engine/_2RealTypeMetainfoVisitor.h"
#include "engine/_2RealTypeCollection.h"

namespace _2Real
{

	std::shared_ptr< BasicTypeMetainfoImpl > BasicTypeMetainfoImpl::make( std::shared_ptr< TypeCollection > types, const DataItem item )
	{
		std::string name = boost::apply_visitor( HumanReadableNameVisitor(), item );
		std::shared_ptr< const MetainfoId > basictypeId = MetainfoId::create( nullptr, MetainfoType::BASICTYPE, name );
		std::shared_ptr< BasicTypeMetainfoImpl > basictypeInfo( new BasicTypeMetainfoImpl( basictypeId, types, item ) );
		return basictypeInfo;
	}

	BasicTypeMetainfoImpl::BasicTypeMetainfoImpl( std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types, DataItem val ) :
		mId( id ),
		mTypes( types ),
		mTemplate( val )
	{
	}

	DataItem BasicTypeMetainfoImpl::makeData() const
	{
		return mTemplate;
	}

	bool BasicTypeMetainfoImpl::isBasicType() const
	{
		return true;
	}

	std::shared_ptr< const MetainfoId > BasicTypeMetainfoImpl::getId() const
	{
		return mId;
	}

	std::string BasicTypeMetainfoImpl::getName() const
	{
		return mId->getName();
	}

}