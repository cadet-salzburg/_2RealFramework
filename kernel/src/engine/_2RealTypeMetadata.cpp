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

#include "datatypes/_2RealFieldDescriptor.h"
#include "_2RealTypeMetadata.h"

namespace _2Real
{
	TypeMetadata::TypeMetadata( std::string const& name ) : mFields(), mName( name )
	{
	}

	TypeMetadata::~TypeMetadata()
	{
		for ( Fields::iterator fieldIter = mFields.begin(); fieldIter != mFields.end(); ++fieldIter )
			delete fieldIter->second;
	}

	void TypeMetadata::addField( std::string const& name, FieldDescriptor const* desc )
	{
		Fields::iterator it = mFields.find( name );
		if ( it != mFields.end() )
		{
			std::ostringstream msg;
			msg << "field: " << name << " already exists in type " << mName << std::endl;
			throw AlreadyExistsException( msg.str() );
		}
		mFields[ name ] = desc;
	}

	//void TypeMetadata::addField( std::string const& name, std::string const& type, TypeMetadata const* meta )
	//{
	//	FieldDescriptor *f = new FieldDescriptor_t< CustomType >( meta );
	//	mFields[ name ] = f;
	//}

	unsigned int TypeMetadata::getNumFields() const
	{
		return mFields.size();
	}

	void TypeMetadata::getFields( _2Real::Fields &fields ) const
	{
		//f.clear(); f.reserve( mFields.size() );
		for ( TypeMetadata::Fields::const_iterator it = mFields.begin(); it != mFields.end(); ++it )
		{
			//_2Real::Fields f;
			Field *field = it->second->getField();
			field->setName( it->first );
			fields.push_back( field );
		}
	}
}