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

#pragma once

#include "_2RealParameterData.h"

#include <map>
#include <string>

namespace _2Real
{

	typedef std::map< std::string, ParameterData >	ParameterDataMap;

	class BlockData
	{

	public:

		BlockData();
		BlockData( std::string const& name );
		BlockData( BlockData const& src );
		BlockData& operator=( BlockData const& src );
		~BlockData();

		friend std::ostream& operator<<( std::ostream &out, BlockData const& metadata );

		void setDescription( std::string const& description );
		void setCategory( std::string const& category );
		void addParameter( ParameterData const& data );
		void addInlet( ParameterData const& data );
		void addOutlet( ParameterData const& data );

		std::string const& getName() const;
		std::string const& getDescription() const;
		std::string const& getCategory() const;

		ParameterDataMap const& getParameters() const;
		ParameterDataMap const& getInlets() const;
		ParameterDataMap const& getOutlets() const;

	private:

		void clear();

		std::string					m_Name;
		std::string					m_Description;
		std::string					m_Category;

		ParameterDataMap			m_Parameters;
		ParameterDataMap			m_Inlets;
		ParameterDataMap			m_Outlets;

	};

}