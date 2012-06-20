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

#include "_2RealEngineData.h"

#include <string>

namespace _2Real
{

	class ParameterData
	{

	public:

		ParameterData();
		ParameterData( std::string const& name, std::string const& longType, std::string const& type, std::string const& category, EngineData const& defaultValue );

		friend std::ostream& operator<<( std::ostream &out, ParameterData const& metadata );

		std::string const& getName() const;
		std::string const& getLongTypename() const;
		std::string const& getTypename() const;
		std::string const& getCategory() const;
		EngineData const& getDefaultValue() const;

	private:

		std::string			m_Name;
		std::string			m_Typename;
		std::string			m_LongTypename;
		std::string			m_Category;
		EngineData			m_DefaultValue;

	};

}