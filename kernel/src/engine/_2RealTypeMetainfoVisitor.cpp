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

#include "engine/_2RealTypeMetainfoVisitor.h"
#include "engine/_2RealSharedTypeMetainfo.h"
#include "engine/_2RealTypeCollection.h"
#include "engine/_2RealCustomData.h"
#include "engine/_2RealId.h"

namespace _2Real
{

	TypeMetainfoVisitor::TypeMetainfoVisitor( std::shared_ptr< const TypeCollection > types ) :
		mTypes( types )
	{
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( const uint8_t val ) const
	{
		( void )( val );
		return mTypes->getMetainfo( "uchar" );
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( const int8_t val ) const
	{
		( void )( val );
		return mTypes->getMetainfo( "char" );
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( const uint32_t val ) const
	{
		( void )( val );
		return mTypes->getMetainfo( "uint" );
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( const int32_t val ) const
	{
		( void )( val );
		return mTypes->getMetainfo( "int" );
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( const uint64_t val ) const
	{
		( void )( val );
		return mTypes->getMetainfo( "ulong" );
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( const int64_t val ) const
	{
		( void )( val );
		return mTypes->getMetainfo( "long" );
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( const double val ) const
	{
		( void )( val );
		return mTypes->getMetainfo( "double" );
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( const float val ) const
	{
		( void )( val );
		return mTypes->getMetainfo( "float" );
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( std::string const& val ) const
	{
		( void )( val );
		return mTypes->getMetainfo( "string" );
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( const bool val ) const
	{
		( void )( val );
		return mTypes->getMetainfo( "bool" );
	}

	std::shared_ptr< const TMetainfo > TypeMetainfoVisitor::operator()( _2Real::CustomDataItem const& val ) const
	{
		return mTypes->getMetainfo( val.mTypeMetainfo->toString() );
	}

}