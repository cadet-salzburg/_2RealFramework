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

#include "app/_2RealBlockData.h"

using std::string;

namespace _2Real
{
	namespace app
	{
		BlockData::BlockData() :
			m_Name( "undefined" ),
			m_Description( "undefined" ),
			m_Category( "undefined" )
		{
		}

		string const& BlockData::getName() const
		{
			return m_Name;
		}

		string const& BlockData::getDescription() const
		{
			return m_Description;
		}

		string const& BlockData::getCategory() const
		{
			return m_Category;
		}

		BlockData::Params const& BlockData::getParameters() const
		{
			return m_Parameters;
		}

		BlockData::Params const& BlockData::getInlets() const
		{
			return m_Inlets;
		}

		BlockData::Params const& BlockData::getOutlets() const
		{
			return m_Outlets;
		}
	}
}