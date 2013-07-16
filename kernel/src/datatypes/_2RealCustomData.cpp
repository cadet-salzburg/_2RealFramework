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

#include "datatypes/_2RealCustomData.h"
#include "bundle/_2RealTypeMetainfo.h"
#include "engine/_2RealTypeMetadata.h"
#include "app/_2RealParameterInfo.h"

#include <assert.h>

namespace _2Real
{
	CustomType::~CustomType()
	{
		for ( DataFields::iterator it = mDataFields.begin(); it != mDataFields.end(); ++it )
		{
			delete it->second;
		}
	}

	CustomType::CustomType( bundle::TypeMetainfo const& meta )
	{
		TypeMetadata const* metadata = meta.mImpl;
		initFrom( metadata );
	}

	CustomType::CustomType( app::TypeMetainfo const& meta )
	{
		TypeMetadata const* metadata = meta.mImpl;
		initFrom( metadata );
	}

	CustomType::CustomType( TypeMetadata const* metadata )
	{
		initFrom( metadata );
	}

	void CustomType::initFrom( TypeMetadata const* metadata )
	{
		mTypeId = TypeMetadata::TypeId( "undefined", "undefined" );

		if ( metadata == nullptr )
			return;

		mTypeId = metadata->getTypeId();

		for ( TypeMetadata::FieldDescriptions::const_iterator it = metadata->mFields.begin(); it != metadata->mFields.end(); ++it )
		{
			// create an any of appropriate type
			AbstractAnyHolder *init = ( it->second )->createAnyHolder();
			// add it to map ( or whatever structure is used )
			this->initField( it->first, init );
		}
	}

	CustomType::CustomType( CustomType const& other )
	{
		for ( DataFields::const_iterator it = other.mDataFields.begin(); it != other.mDataFields.end(); ++it )
		{
			std::string name = it->first;
			AbstractAnyHolder * value = it->second->clone();
			mDataFields[ name ] = value;
		}
	}

	void CustomType::cloneFrom( CustomType const& other )
	{
		mDataFields.clear();

		for ( DataFields::const_iterator it = other.mDataFields.begin(); it != other.mDataFields.end(); ++it )
		{
			std::string name = it->first;
			AbstractAnyHolder * value = it->second->clone();
			mDataFields[ name ] = value;
		}
	}

	void CustomType::initField( std::string const& name, AbstractAnyHolder *init )
	{
#ifdef _DEBUG
		assert( init != nullptr );
		assert( mDataFields.find( name ) == mDataFields.end() );
#endif
		// ONLY here, in the init field method, assignment operator may be used
		// for all later operations, set must be used instead ( involves a typecheck! )
		mDataFields[ name ] = init;
	}

	CustomType::DataFields::iterator CustomType::iter( std::string const& name )
	{
		DataFields::iterator dataIter = mDataFields.find( name );
		if ( dataIter == mDataFields.end() )
		{
			// TODO name of type & type of exception
			std::ostringstream msg;
			msg << "field " << name << " not defined in data type::" << mTypeId.first << "::" << mTypeId.second;
			throw _2Real::Exception( msg.str() );
		}
			
		return dataIter;
	}

	CustomType::DataFields::const_iterator CustomType::constIter( std::string const& name ) const
	{
		DataFields::const_iterator dataIter = mDataFields.find( name );
		if ( dataIter == mDataFields.end() )
		{
			// TODO name of type & type of exception
			std::ostringstream msg;
			msg << "field " << name << " not defined in data type " << "XXXX";
			throw _2Real::Exception( msg.str() );
		}
			
		return dataIter;
	}

	void CustomType::setValueInternal( std::string const& field, AbstractAnyHolder *value )
	{
		// may throw if not found
		DataFields::iterator it = iter( field );

		AbstractAnyHolder *h = it->second;

		// may also throw
		h->set( *value );
		delete value;
	}

	AbstractAnyHolder const* CustomType::getValueInternal( std::string const& field ) const
	{
		// may throw if not found
		DataFields::const_iterator it = constIter( field );
		return it->second;
	}

	AbstractAnyHolder * CustomType::getValueInternal( std::string const& field )
	{
		// may throw if not found
		DataFields::iterator it = iter( field );
		return it->second;
	}

	void CustomType::writeTo( std::ostringstream &out ) const
	{
	}

	bool CustomType::isEqualTo( CustomType const& other ) const
	{
		return false;
	}
}
