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

#include "engine/_2RealCustomTypeMetainfoImpl.h"
#include "engine/_2RealId.h"
#include "engine/_2RealHumanReadableNameVisitor.h"
#include "engine/_2RealTypeMetainfoVisitor.h"
#include "engine/_2RealTypeCollection.h"

namespace _2Real
{

	std::shared_ptr< CustomTypeMetainfoImpl > CustomTypeMetainfoImpl::make( std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types, const std::string name )
	{
		std::shared_ptr< const MetainfoId > sharedtypeId = MetainfoId::create( id, MetainfoType::CUSTOMTYPE, name );
		std::shared_ptr< CustomTypeMetainfoImpl > sharedtypeInfo( new CustomTypeMetainfoImpl( sharedtypeId, types ) );
		sharedtypeInfo->mTemplate = CustomDataItem( sharedtypeId );
		return sharedtypeInfo;
	}

	CustomTypeMetainfoImpl::CustomTypeMetainfoImpl( std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types ) :
		TypeMetainfoImpl_I(),
		std::enable_shared_from_this< CustomTypeMetainfoImpl >(),
		mId( id ),
		mDescription( "no description available" ),
		mTemplate(),
		mTypes( types )
	{
	}

	void CustomTypeMetainfoImpl::destroy()
	{
		mDestroyed( shared_from_this() );
		mDestroyed.disconnect_all_slots();
	}

	bool CustomTypeMetainfoImpl::isBasicType() const
	{
		return false;
	}

	std::string CustomTypeMetainfoImpl::getName() const
	{
		return mId->getName();
	}

	std::string CustomTypeMetainfoImpl::getDescription() const
	{
		return mDescription;
	}

	std::shared_ptr< const MetainfoId > CustomTypeMetainfoImpl::getId() const
	{
		return mId;
	}

	DataItem CustomTypeMetainfoImpl::makeData() const
	{
		return mTemplate;
	}

	CustomDataItem CustomTypeMetainfoImpl::makeCustomData() const
	{
		return CustomDataItem( mTemplate );
	}

	void CustomTypeMetainfoImpl::setDescription( const std::string description )
	{
		mDescription = description;
	}

	void CustomTypeMetainfoImpl::addField( std::string fieldName, DataItem value )
	{
		mTemplate.addField( DataField( std::move( fieldName ), std::move( value ) ) );
	}

	std::vector< std::pair< std::string, std::shared_ptr< const TypeMetainfoImpl_I > > > CustomTypeMetainfoImpl::getDataFields() const
	{
		std::vector< std::pair< std::string, std::shared_ptr< const TypeMetainfoImpl_I > > > result;
			
		for ( auto const& it : mTemplate.mDataFields )
			result.push_back( std::make_pair( it.getName(), boost::apply_visitor( TypeMetainfoVisitor( mTypes.lock() ), it.getValue() ) ) );

		return result;
	}

	Connection CustomTypeMetainfoImpl::registerToDestroyed( boost::signals2::signal< void( std::shared_ptr< const CustomTypeMetainfoImpl > ) >::slot_type listener ) const
	{
		return mDestroyed.connect( listener );
	}

}