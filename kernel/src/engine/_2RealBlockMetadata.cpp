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

#include "engine/_2RealBlockMetadata.h"
#include "engine/_2RealIOMetadata.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealStringHelpers.h"
#include "datatypes/_2RealTypeRegistry.h"
#include "engine/_2RealExportMetainfo.h"

using std::ostringstream;
using std::string;

namespace _2Real
{
	BlockMetadata::BlockMetadata( std::shared_ptr< TemplateId > id, Metainfo *owner, TypeRegistry *fw, TypeRegistry *exp, const bool isContext, const bool needsContext ) :
		mIdentifier( id ),
		mDescription( "undefined" ),
		mCategory( "undefined" ),
		mIsContext( isContext ),
		mNeedsContext( needsContext ),
		mThreadingPolicy( ThreadingPolicy::ANY_THREAD ),
		mOwner( owner ),
		mExportedTypes( exp ),
		mFrameworkTypes( fw )
	{
	}

	bool BlockMetadata::isContext() const
	{
		return mIsContext;
	}

	bool BlockMetadata::needsContext() const
	{
		return mNeedsContext;
	}

	std::string const& BlockMetadata::getName() const
	{
		return mIdentifier->getObjectName();
	}

	std::shared_ptr< const TemplateId >  BlockMetadata::getIdentifier() const
	{
		return mIdentifier;
	}

	string const& BlockMetadata::getDescription() const
	{
		return mDescription;
	}

	void BlockMetadata::setDescription( string const& description )
	{
		mDescription = description;
	}

	std::string const& BlockMetadata::getCategory() const
	{
		return mCategory;
	}

	void BlockMetadata::setCategory( string const& category )
	{
		mCategory = category;
	}

	void BlockMetadata::setThreadingPolicy( ThreadingPolicy const& policy )
	{
		mThreadingPolicy = policy;
	}

	void BlockMetadata::addInlet( std::shared_ptr< IOMetadata > data, std::string const& type )
	{
		// check if metadata is null, get it from the registry otherwise
		if ( nullptr == data->typeMetadata.get() )
		{
			std::shared_ptr< const TypeMetadata > meta = mFrameworkTypes->get( type );
			if ( nullptr == meta.get() )
			{
				meta = mExportedTypes->get( type );
				if ( nullptr == meta.get() )
				{
					std::stringstream msg;
					msg << "type: " << type << " is not known";
					throw NotFoundException( msg.str() );
				
				}
			}

			data->typeMetadata = meta;
		}

		if ( data->initializer.get() == nullptr )
			data->initializer.reset( new CustomType( data->typeMetadata ) );

		for ( BlockMetadata::IOMetadataIterator it = mInlets.begin(); it != mInlets.end(); ++it )
		{
			if ( toLower( ( *it )->identifier->getObjectName() ) == toLower( data->identifier->getObjectName() ) )
			{
				ostringstream msg;
				msg << "inlet: " << data->identifier->getObjectName() << " is already defined in block " << getName() << std::endl;
				throw AlreadyExistsException( msg.str() );
			}
		}

		mInlets.push_back( data );
	}

	void BlockMetadata::addParameter( std::shared_ptr< IOMetadata > data, std::string const& type )
	{
		// check if metadata is null, get it from the regstry otherwise
		if ( nullptr == data->typeMetadata.get() )
		{
			std::shared_ptr< const TypeMetadata > meta = mFrameworkTypes->get( type );
			if ( nullptr == meta.get() )
			{
				meta = mExportedTypes->get( type );
				if ( nullptr == meta.get() )
				{
					std::stringstream msg;
					msg << "type: " << type << " is not known";
					throw NotFoundException( msg.str() );
				
				}
			}

			data->typeMetadata = meta;
		}

		if ( data->initializer.get() == nullptr )
			data->initializer.reset( new CustomType( data->typeMetadata ) );

		for ( BlockMetadata::IOMetadataIterator it = mParameters.begin(); it != mParameters.end(); ++it )
		{
			if ( toLower( ( *it )->identifier->getObjectName() ) == toLower( data->identifier->getObjectName() ) )
			{
				ostringstream msg;
				msg << "parameter: " << data->identifier->getObjectName() << " is already defined in block " << getName() << std::endl;
				throw AlreadyExistsException( msg.str() );
			}
		}

		mParameters.push_back( data );
	}

	void BlockMetadata::addOutlet( std::shared_ptr< IOMetadata > data, std::string const& type )
	{
		// check if metadata is null, get it from the regstry otherwise
		if ( nullptr == data->typeMetadata.get() )
		{
			std::shared_ptr< const TypeMetadata > meta = mFrameworkTypes->get( type );
			if ( nullptr == meta.get() )
			{
				meta = mExportedTypes->get( type );
				if ( nullptr == meta.get() )
				{
					std::stringstream msg;
					msg << "type: " << type << " is not known";
					throw NotFoundException( msg.str() );
				
				}
			}

			data->typeMetadata = meta;
		}

		if ( data->initializer.get() == nullptr )
			data->initializer.reset( new CustomType( data->typeMetadata ) );

		for ( BlockMetadata::IOMetadataIterator it = mOutlets.begin(); it != mOutlets.end(); ++it )
		{
			if ( toLower( ( *it )->identifier->getObjectName() ) == toLower( data->identifier->getObjectName() ) )
			{
				ostringstream msg;
				msg << "outlet: " << data->identifier->getObjectName() << " is already defined in block " << getName() << std::endl;
				throw AlreadyExistsException( msg.str() );
			}
		}

		mOutlets.push_back( data );
	}

	BlockMetadata::IOMetadatas const& BlockMetadata::getInlets() const
	{
		return mInlets;
	}

	BlockMetadata::IOMetadatas const& BlockMetadata::getOutlets() const
	{
		return mOutlets;
	}

	BlockMetadata::IOMetadatas const& BlockMetadata::getParameters() const
	{
		return mParameters;
	}

}