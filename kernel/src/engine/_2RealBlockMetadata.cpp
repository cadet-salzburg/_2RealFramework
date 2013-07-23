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
#include "engine/_2RealParameterMetadata.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealStringHelpers.h"
#include "datatypes/_2RealTypeRegistry.h"

#include <sstream>

using std::ostringstream;
using std::string;

namespace _2Real
{

	//BlockMetadata::BlockMetadata( TypeRegistry const* reg ) :
	//	m_Name( "undefined" ),
	//	m_Description( "undefined" ),
	//	m_Category( "undefined" ),
	//	m_ThreadingPolicy( ThreadingPolicy::ANY_THREAD ),
	//	mRegistry( reg )
	//{
	//}

	BlockMetadata::BlockMetadata( BlockId const& name, TypeRegistry const* reg ) :
		mBlockId( name ),
		m_Description( "undefined" ),
		m_Category( "undefined" ),
		m_ThreadingPolicy( ThreadingPolicy::ANY_THREAD ),
		mRegistry( reg )
	{
	}

	BlockMetadata::~BlockMetadata()
	{
		for ( BlockMetadata::InletMetadataIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			delete *it;
		}

		for ( BlockMetadata::OutletMetadataIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			delete *it;
		}

		for ( BlockMetadata::ParameterMetadataIterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it )
		{
			delete *it;
		}
	}

	string const& BlockMetadata::getName() const
	{
		return mBlockId.second;
	}

	string const& BlockMetadata::getDescription() const
	{
		return m_Description;
	}

	void BlockMetadata::setDescription( string const& description )
	{
		m_Description = description;
	}

	std::string const& BlockMetadata::getCategory() const
	{
		return m_Category;
	}

	void BlockMetadata::setCategory( string const& category )
	{
		m_Category = category;
	}

	void BlockMetadata::setThreadingPolicy( ThreadingPolicy const& policy )
	{
		m_ThreadingPolicy = policy;
	}

	void BlockMetadata::addInlet( InletMetadata *data )
	{
		// check if metadata is null, get it from the regstry otherwise
		if ( nullptr == data->metadata )
		{
#ifdef _DEBUG
			assert( mRegistry );
#endif
			TypeMetadata const* meta = mRegistry->get( mBlockId.first, data->type );
			if ( nullptr == meta )
			{
				std::stringstream msg;
				msg << "type: " << data->type << " is not known";
				throw NotFoundException( msg.str() );
			}

			data->metadata = meta;
		}

		for ( BlockMetadata::InletMetadataIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			if ( toLower( ( **it ).name ) == toLower( data->name ) )
			{
				ostringstream msg;
				msg << "inlet: " << data->name << " is already defined in block " << getName() << std::endl;
				throw AlreadyExistsException( msg.str() );
			}
		}

		m_Inlets.push_back( data );
	}

	void BlockMetadata::addParameter( ParameterMetadata *data )
	{
		// check if metadata is null, get it from the regstry otherwise
		if ( nullptr == data->metadata )
		{
#ifdef _DEBUG
			assert( mRegistry );
#endif
			TypeMetadata const* meta = mRegistry->get( mBlockId.first, data->type );
			if ( nullptr == meta )
			{
				std::stringstream msg;
				msg << "type: " << data->type << " is not known";
				throw NotFoundException( msg.str() );
			}

			data->metadata = meta;
		}

		for ( BlockMetadata::ParameterMetadataIterator it = m_Parameters.begin(); it != m_Parameters.end(); ++it )
		{
			if ( toLower( ( **it ).name ) == toLower( data->name ) )
			{
				ostringstream msg;
				msg << "parameter: " << data->name << " is already defined in block " << getName() << std::endl;
				throw AlreadyExistsException( msg.str() );
			}
		}

		m_Parameters.push_back( data );
	}

	void BlockMetadata::addOutlet( OutletMetadata *data )
	{
		// check if metadata is null, get it from the regstry otherwise
		if ( nullptr == data->metadata )
		{
#ifdef _DEBUG
			assert( mRegistry );
#endif
			TypeMetadata const* meta = mRegistry->get( mBlockId.first, data->type );
			if ( nullptr == meta )
			{
				std::stringstream msg;
				msg << "type: " << data->type << " is not known";
				throw NotFoundException( msg.str() );
			}

			data->metadata = meta;
		}

		for ( BlockMetadata::OutletMetadataIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( toLower( ( **it ).name ) == toLower( data->name ) )
			{
				ostringstream msg;
				msg << "outlet: " << data->name << " is already defined in block " << getName() << std::endl;
				throw AlreadyExistsException( msg.str() );
			}
		}

		m_Outlets.push_back( data );
	}

	BlockMetadata::InletMetadatas const& BlockMetadata::getInlets() const
	{
		return m_Inlets;
	}

	BlockMetadata::OutletMetadatas const& BlockMetadata::getOutlets() const
	{
		return m_Outlets;
	}

	BlockMetadata::ParameterMetadatas const& BlockMetadata::getParameters() const
	{
		return m_Parameters;
	}

	void BlockMetadata::performBlockNameCheck( string const& name )
	{
	}

}