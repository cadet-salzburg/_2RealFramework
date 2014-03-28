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

#include "engine/_2RealSharedServiceIoSlotMetainfo.h"
#include "engine/_2RealSharedTypeMetainfo.h"
#include "engine/_2RealTypeCollection.h"

namespace _2Real
{

	class MetainfoVisitor : public boost::static_visitor< std::shared_ptr< const TMetainfo > >
	{

	public:

		MetainfoVisitor() = delete;

		explicit MetainfoVisitor( std::shared_ptr< TypeCollection > types ) : mTypes( types )
		{
		}

		std::shared_ptr< const TMetainfo > operator()( const uint8_t val ) const
		{
			( void )( val );
			std::shared_ptr< const TMetainfo > result( new BasicTypeMetainfo( ( uint8_t )0 ) );
			return result;
		}

		std::shared_ptr< const TMetainfo > operator()( const int8_t val ) const
		{
			( void )( val );
			std::shared_ptr< const TMetainfo > result( new BasicTypeMetainfo( ( int8_t )0 ) );
			return result;
		}

		std::shared_ptr< const TMetainfo > operator()( const uint32_t val ) const
		{
			( void )( val );
			std::shared_ptr< const TMetainfo > result( new BasicTypeMetainfo( ( uint32_t )0 ) );
			return result;
		}

		std::shared_ptr< const TMetainfo > operator()( const int32_t val ) const
		{
			( void )( val );
			std::shared_ptr< const TMetainfo > result( new BasicTypeMetainfo( ( int32_t )0 ) );
			return result;
		}

		std::shared_ptr< const TMetainfo > operator()( const uint64_t val ) const
		{
			( void )( val );
			std::shared_ptr< const TMetainfo > result( new BasicTypeMetainfo( ( uint64_t )0 ) );
			return result;
		}

		std::shared_ptr< const TMetainfo > operator()( const int64_t val ) const
		{
			( void )( val );
			std::shared_ptr< const TMetainfo > result( new BasicTypeMetainfo( ( int64_t )0 ) );
			return result;
		}

		std::shared_ptr< const TMetainfo > operator()( double const& val ) const
		{
			( void )( val );
			std::shared_ptr< const TMetainfo > result( new BasicTypeMetainfo( 0.0 ) );
			return result;
		}

		std::shared_ptr< const TMetainfo > operator()( const float val ) const
		{
			( void )( val );
			std::shared_ptr< const TMetainfo > result( new BasicTypeMetainfo( 0.f ) );
			return result;
		}

		std::shared_ptr< const TMetainfo > operator()( std::string const& val ) const
		{
			( void )( val );
			std::shared_ptr< const TMetainfo > result( new BasicTypeMetainfo( std::string() ) );
			return result;
		}

		std::shared_ptr< const TMetainfo > operator()( const bool val ) const
		{
			( void )( val );
			std::shared_ptr< const TMetainfo > result( new BasicTypeMetainfo( false ) );
			return result;
		}

		std::shared_ptr< const TMetainfo > operator()( _2Real::CustomDataItem const& val ) const
		{
			return mTypes->getTypeMetainfo( val.getName() );
		}

	private:

		std::shared_ptr< TypeCollection > mTypes;

	};

	SharedServiceIoSlotMetainfo::SharedServiceIoSlotMetainfo( std::string const& name, std::shared_ptr< TypeCollection > types ) :
		std::enable_shared_from_this< SharedServiceIoSlotMetainfo >(),
		mName( name ),
		mDescription( "" ),
		mTypes( types )
	{
		mDatatype = boost::apply_visitor( HumanReadableNameVisitor(), mInitialValue );
	}

	SharedServiceIoSlotMetainfo::~SharedServiceIoSlotMetainfo()
	{
	}

	void SharedServiceIoSlotMetainfo::setName( std::string const& name )
	{
		mName = name;
	}

	void SharedServiceIoSlotMetainfo::setDescription( std::string const& description )
	{
		mDescription = description;
	}

	void SharedServiceIoSlotMetainfo::setDatatypeAndInitialValue( DataItem const& initialValue )
	{
		mDatatype = boost::apply_visitor( HumanReadableNameVisitor(), initialValue );
		mInitialValue = initialValue;
	}

	std::string const& SharedServiceIoSlotMetainfo::getName() const
	{
		return mName;
	}

	std::string const& SharedServiceIoSlotMetainfo::getDescription() const
	{
		return mDescription;
	}

	DataItem const& SharedServiceIoSlotMetainfo::getInitialValue() const
	{
		return mInitialValue;
	}

	std::string const& SharedServiceIoSlotMetainfo::getDatatype() const
	{
		return mDatatype;
	}

	std::shared_ptr< const TMetainfo > SharedServiceIoSlotMetainfo::getTypeMetainfo() const
	{
		return boost::apply_visitor( MetainfoVisitor( mTypes ), mInitialValue );
	}

}