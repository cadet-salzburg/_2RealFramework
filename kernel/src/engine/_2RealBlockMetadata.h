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

#include "engine/_2RealParameterMetadata.h"

#include <vector>
#include <string>

namespace _2Real
{
	class BlockMetadata
	{

	public:

		typedef std::vector< ParameterMetadata >					ParamMetas;
		typedef std::vector< ParameterMetadata >::iterator			ParamMetaIterator;
		typedef std::vector< ParameterMetadata >::const_iterator	ParamMetaConstIterator;

		BlockMetadata();
		BlockMetadata( std::string const& name );

		void setDescription( std::string const& description );
		void setCategory( std::string const& category );
		void addInlet( ParameterMetadata const& data );
		void addOutlet( ParameterMetadata const& data );

		std::string const& getName() const;
		std::string const& getDescription() const;
		std::string const& getCategory() const;

		ParamMetas const& getInlets() const;
		ParamMetas const& getOutlets() const;

		static void performBlockNameCheck( std::string const& name );

	private:

		std::string			m_Name;
		std::string			m_Description;
		std::string			m_Category;

		ParamMetas			m_Inlets;
		ParamMetas			m_Outlets;

	};
}