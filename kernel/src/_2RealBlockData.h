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

#include <map>
#include <string>

namespace _2Real
{

	class ParameterData;

	class BlockData
	{

	public:

		BlockData( std::string const& name );
		~BlockData();

		friend std::ostream& operator<<(std::ostream &out, BlockData const& metadata);

		void setDescription( std::string const& description );
		void addParameter( ParameterData const& data );
		void addInlet( ParameterData const& data );
		void addOutlet( ParameterData const& data );

		std::string const& getName() const;
		std::string const& getDescription() const;

		std::map< std::string, ParameterData const* > const& getParameters() const;
		std::map< std::string, ParameterData const* > const& getInlets() const;
		std::map< std::string, ParameterData const* > const& getOutlets() const;

	private:

		typedef std::map< std::string, ParameterData const* >	ParameterDataMap;

		std::string					m_Name;
		std::string					m_Description;

		ParameterDataMap			m_Parameters;
		ParameterDataMap			m_Inlets;
		ParameterDataMap			m_Outlets;

	};

}