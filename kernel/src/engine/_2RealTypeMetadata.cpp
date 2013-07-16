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
#include "datatypes/_2RealTypeRegistry.h"
#include "datatypes/_2RealDataField.h"
#include "_2RealTypeMetadata.h"

namespace _2Real
{
	TypeMetadata::TypeMetadata( std::string const& name, TypeRegistry const* reg ) : mFields(), mName( name ), mRegistry( reg )
	{
		// reg could be nullptr - it's only needed if custom types are accessed, i.e. image does not need it
	}

	TypeMetadata::~TypeMetadata()
	{
		for ( Fields::iterator fieldIter = mFields.begin(); fieldIter != mFields.end(); ++fieldIter )
			delete fieldIter->second;
	}

	void TypeMetadata::addField( std::string const& name, std::string const& type, FieldDescriptor const* desc )
	{
		// make sure field name is unique
		Fields::iterator it = mFields.find( name );
		if ( it != mFields.end() )
		{
			std::ostringstream msg;
			msg << "field: " << name << " already exists in type " << mName << std::endl;
			throw AlreadyExistsException( msg.str() );
		}

		// if desc == null, it's a custom type, so get the metadata
		if ( nullptr == desc )
		{
#ifdef _DEBUG
			assert( mRegistry );
#endif
			TypeMetadata const* meta = mRegistry->get( "", type );
			if ( nullptr == meta )
			{
				std::stringstream msg;
				msg << "type: " << type << "is not known";
				throw NotFoundException( msg.str() );
			}

			_2Real::Fields fields;
			meta->getFields( fields );

			FieldDescriptor const *d = DataField< CustomType >::createFieldDescriptor( name, type, CustomType( meta ), fields );
			mFields[ name ] = d;
		}
		else
			mFields[ name ] = desc;
	}

	void TypeMetadata::getFields( _2Real::Fields &fields ) const
	{
		fields.clear(); fields.reserve( mFields.size() );
		for ( TypeMetadata::Fields::const_iterator it = mFields.begin(); it != mFields.end(); ++it )
		{
			fields.push_back( it->second->getField() );
		}
	}
}