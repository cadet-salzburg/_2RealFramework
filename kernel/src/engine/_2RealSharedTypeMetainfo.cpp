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

#include "engine/_2RealSharedTypeMetainfo.h"
#include "engine/_2RealTypeCollection.h"
#include "engine/_2RealCustomData.h"
#include "engine/_2RealDataFieldMetainfo.h"
#include "helpers/_2RealException.h"

namespace _2Real
{

	SharedTypeMetainfo::SharedTypeMetainfo( std::string const& name ) :
		std::enable_shared_from_this< SharedTypeMetainfo >(),
		mName( name ),
		mIsFinal( false )
	{
	}

	SharedTypeMetainfo::~SharedTypeMetainfo()
	{
		std::cout << "deleting shared type metainfo " << mName << std::endl;
	}

	std::shared_ptr< CustomData > SharedTypeMetainfo::makeData() const
	{
		std::shared_ptr< CustomData > data( new CustomData );
		data->mMetainfo = shared_from_this();
		for ( auto it : mFields )
		{
			data->mDataFields[ it.first ] = it.second->makeData();
		}
		return data;
	}

	std::string const& SharedTypeMetainfo::getName() const
	{
		return mName;
	}

	bool SharedTypeMetainfo::finalize()
	{
		mIsFinal = true;
		return true;
	}

	bool SharedTypeMetainfo::performExport()
	{
		return true;
	}

	void SharedTypeMetainfo::addField( std::string const& fieldName, std::string const& typeName, std::shared_ptr< const AbstractFieldDescriptor > desc )
	{
		// make sure field name is unique
		for ( auto it : mFields )
		{
			if ( fieldName == it.first )
			{
				std::ostringstream msg;
				msg << "a field named " << fieldName << " already exists in type" << mName << std::endl;
				throw AlreadyExistsException( msg.str() );
			}
		}

		// if desc == null, it's a custom type, so get the metadata: first from global, then from local types
		if ( nullptr == desc )
		{
			std::shared_ptr< const SharedTypeMetainfo > meta = mGlobalTypes.lock()->get( typeName );
			if ( nullptr == meta.get() )
			{
				meta = mLocalTypes.lock()->get( typeName );
				if ( nullptr == meta.get() )
				{
					std::stringstream msg;
					msg << "unknown field type " << typeName;
					throw NotFoundException( msg.str() );
				}
			}

			std::shared_ptr< AbstractDataFieldMetainfo > field( new ComplexDataFieldMetainfo( meta ) );
			mFields.push_back( std::make_pair( fieldName, field ) );
		}
		// otherwise, it was a basic type anyway
		else
		{
			std::shared_ptr< AbstractDataFieldMetainfo > field( new BasicDataFieldMetainfo( desc ) );
			mFields.push_back( std::make_pair( fieldName, field ) );
		}
	}

}