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

#include "engine/_2RealBlockData.h"

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

	void BlockData::addParameter( ParamData const& data )
	{
		m_Parameters.push_back( data );
	}

	void BlockData::addInlet( ParamData const& data )
	{
		m_Inlets.push_back( data );
	}

	void BlockData::addOutlet( ParamData const& data )
	{
		m_Outlets.push_back( data );
	}

	BlockData::ParamMetas const& BlockData::getParameters() const
	{
		return m_Parameters;
	}

	BlockData::ParamMetas const& BlockData::getInlets() const
	{
		return m_Inlets;
	}

	BlockData::ParamMetas const& BlockData::getOutlets() const
	{
		return m_Outlets;
	}

}