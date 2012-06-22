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

#include "_2RealBlockData.h"

namespace _2Real
{

	BlockData::BlockData() :
		m_Name( "undefined" ),
		m_Description( "undefined" ),
		m_Category( "undefined" )
	{
	}

	BlockData::BlockData( std::string const& name ) :
		m_Name( name ),
		m_Description( "undefined" ),
		m_Category( "undefined" )
	{
	}

	BlockData::BlockData( BlockData const& src ) :
		m_Name( src.m_Name ),
		m_Description( src.m_Description ),
		m_Category( src.m_Category )
	{
		for ( ParameterDataMap::const_iterator it = src.m_Parameters.begin(); it != src.m_Parameters.end(); ++it )
		{
			ParameterData data( it->second );
			m_Parameters[ data.getName() ] = data;
		}

		for ( ParameterDataMap::const_iterator it = src.m_Inlets.begin(); it != src.m_Inlets.end(); ++it )
		{
			ParameterData data( it->second );
			m_Inlets[ data.getName() ] = data;
		}

		for ( ParameterDataMap::const_iterator it = src.m_Outlets.begin(); it != src.m_Outlets.end(); ++it )
		{
			ParameterData data( it->second );
			m_Outlets[ data.getName() ] = data;
		}
	}

	BlockData& BlockData::operator=( BlockData const& src )
	{
		if ( this == &src )
		{
			return *this;
		}

		m_Name = src.m_Name;
		m_Description = src.m_Description;
		m_Category = src.m_Category;

		for ( ParameterDataMap::const_iterator it = src.m_Parameters.begin(); it != src.m_Parameters.end(); ++it )
		{
			ParameterData data( it->second );
			m_Parameters[ data.getName() ] = data;
		}

		for ( ParameterDataMap::const_iterator it = src.m_Inlets.begin(); it != src.m_Inlets.end(); ++it )
		{
			ParameterData data( it->second );
			m_Inlets[ data.getName() ] = data;
		}

		for ( ParameterDataMap::const_iterator it = src.m_Outlets.begin(); it != src.m_Outlets.end(); ++it )
		{
			ParameterData data( it->second );
			m_Outlets[ data.getName() ] = data;
		}

		return *this;
	}

	BlockData::~BlockData()
	{
	}

	std::string const& BlockData::getName() const
	{
		return m_Name;
	}

	std::string const& BlockData::getDescription() const
	{
		return m_Description;
	}

	void BlockData::setDescription(std::string const& description)
	{
		m_Description = description;
	}

	std::string const& BlockData::getCategory() const
	{
		return m_Category;
	}

	void BlockData::setCategory(std::string const& category)
	{
		m_Category = category;
	}

	void BlockData::addParameter( ParameterData const& data )
	{
		m_Parameters[ data.getName() ] = data;
	}

	void BlockData::addInlet( ParameterData const& data )
	{
		m_Inlets[ data.getName() ] = data;
	}

	void BlockData::addOutlet( ParameterData const& data )
	{
		std::string outletName = data.getName();

		m_Outlets[ data.getName() ] = data;
	}

	std::ostream& operator<<( std::ostream &out, BlockData const& data )
	{
		out << data.getName() << std::endl;
		out << data.getDescription() << std::endl;
		out << data.getCategory() << std::endl;
		out << "parameters:" << std::endl;
		out << data.getParameters() << std::endl;
		out << "inlets:" << std::endl;
		out << data.getInlets() << std::endl;
		out << "outlets:" << std::endl;
		out << data.getOutlets() << std::endl;
		return out;
	}

	ParameterDataMap const& BlockData::getParameters() const
	{
		return m_Parameters;
	}

	ParameterDataMap const& BlockData::getInlets() const
	{
		return m_Inlets;
	}

	ParameterDataMap const& BlockData::getOutlets() const
	{
		return m_Outlets;
	}

}