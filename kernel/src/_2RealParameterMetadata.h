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

#include <string>
#include <sstream>

namespace _2Real
{

	class ParameterMetadata
	{

	public:

		ParameterMetadata(std::string const& name, std::string const& type);
		std::string const& getName() const;
		std::string const& getType() const;

	private:

		const std::string	m_Name;
		const std::string	m_Type;

	};

	inline ParameterMetadata::ParameterMetadata(std::string const& name, std::string const& type) :
		m_Name(name),
		m_Type(type)
	{
	}

	inline std::string const& ParameterMetadata::getName() const
	{
		return m_Name;
	}

	inline std::string const& ParameterMetadata::getType() const
	{
		return m_Type;
	}

}