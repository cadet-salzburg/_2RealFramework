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

#include <sstream>

using std::ostringstream;
using std::string;

namespace _2Real
{

	BlockMetadata::BlockMetadata() :
		m_Name( "undefined" ),
		m_Description( "undefined" ),
		m_Category( "undefined" )
	{
	}

	BlockMetadata::BlockMetadata( string const& name ) :
		m_Name( name ),
		m_Description( "undefined" ),
		m_Category( "undefined" )
	{
	}

	BlockMetadata::~BlockMetadata()
	{
		for ( BlockMetadata::ParameterMetadataIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			delete *it;
		}

		for ( BlockMetadata::ParameterMetadataIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			delete *it;
		}
	}

	string const& BlockMetadata::getName() const
	{
		return m_Name;
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

	void BlockMetadata::addInlet( ParameterMetadata const& data )
	{
		for ( BlockMetadata::ParameterMetadataIterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it )
		{
			if ( toLower( ( **it ).name ) == toLower( data.name ) )
			{
				ostringstream msg;
				msg << "inlet named " << data.name << " is already defined in block " << getName() << std::endl;
				throw AlreadyExistsException( msg.str() );
			}
		}

		m_Inlets.push_back( &data );
	}

	void BlockMetadata::addOutlet( ParameterMetadata const& data )
	{
		for ( BlockMetadata::ParameterMetadataIterator it = m_Outlets.begin(); it != m_Outlets.end(); ++it )
		{
			if ( toLower( ( **it ).name ) == toLower( data.name ) )
			{
				ostringstream msg;
				msg << "outlet named " << data.name << " is already defined in block " << getName() << std::endl;
				throw AlreadyExistsException( msg.str() );
			}
		}

		m_Outlets.push_back( &data );
	}

	BlockMetadata::ParameterMetadatas const& BlockMetadata::getInlets() const
	{
		return m_Inlets;
	}

	BlockMetadata::ParameterMetadatas const& BlockMetadata::getOutlets() const
	{
		return m_Outlets;
	}

	void BlockMetadata::performBlockNameCheck( string const& name )
	{
	}

}