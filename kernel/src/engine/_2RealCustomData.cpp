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

#include "engine/_2RealCustomData.h"
#include "engine/_2RealSharedTypeMetainfo.h"
#include "helpers/_2RealException.h"

namespace _2Real
{
	//CustomData::CustomData()
	//{
	//}

	//CustomData::~CustomData()
	//{
	//}

	//CustomData::CustomData( CustomData const& other )
	//{
	//	for ( auto it : other.mDataFields )
	//	{
	//		std::string name = it.first;
	//		std::shared_ptr< AbstractAnyHolder > value( it.second->clone() );
	//		mDataFields[ name ] = value;
	//	}
	//}

	//CustomData& CustomData::operator=( CustomData const& other )
	//{
	//	if ( this == &other )
	//		return *this;

	//	mDataFields.clear();

	//	for ( auto it : other.mDataFields )
	//	{
	//		std::string name = it.first;
	//		std::shared_ptr< AbstractAnyHolder > value( it.second->clone() );
	//		mDataFields[ name ] = value;
	//	}

	//	return *this;
	//}

	//void CustomData::setValueInternal( std::string const& fieldName, std::shared_ptr< AbstractAnyHolder > value )
	//{
	//	auto it = mDataFields.find( fieldName );
	//	if ( it == mDataFields.end() )
	//	{
	//		std::ostringstream msg;
	//		msg << "no field called " << fieldName << " found in custom data" << std::endl;
	//		throw NotFoundException( msg.str() );
	//	}

	//	it->second = value;

	//	// ok... old code sets the already existing any holder? wtf
	//	// i leave this here, in case there are problems....
	//	//AbstractAnyHolder *h = it->second;
	//	//h->set( *value );
	//	//delete value;
	//}

	//std::shared_ptr< const AbstractAnyHolder > CustomData::getValueInternal( std::string const& fieldName ) const
	//{
	//	auto it = mDataFields.find( fieldName );
	//	if ( it == mDataFields.end() )
	//	{
	//		std::ostringstream msg;
	//		msg << "no field called " << fieldName << " found in custom data" << std::endl;
	//		throw NotFoundException( msg.str() );
	//	}

	//	return it->second;
	//}

	//std::shared_ptr< AbstractAnyHolder > CustomData::getValueInternal( std::string const& fieldName )
	//{
	//	auto it = mDataFields.find( fieldName );
	//	if ( it == mDataFields.end() )
	//	{
	//		std::ostringstream msg;
	//		msg << "no field called " << fieldName << " found in custom data" << std::endl;
	//		throw NotFoundException( msg.str() );
	//	}

	//	return it->second;
	//}

	//bool CustomData::operator==( CustomData const& other )
	//{
	//	return false;
	//}

	//bool CustomData::operator<( CustomData const& other )
	//{
	//	return false;
	//}

}
