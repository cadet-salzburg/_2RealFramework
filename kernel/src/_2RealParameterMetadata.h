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
#include <sstream>
#include <iostream>

namespace _2Real
{

	class ParameterMetadata
	{

	public:

		ParameterMetadata(std::string const& name, std::string const& type, std::string const& keyword, EngineData const& defaultValue);
		ParameterMetadata(std::string const& name, std::string const& type, std::string const& keyword);

		~ParameterMetadata();

		friend std::ostream& operator<<(std::ostream &out, ParameterMetadata const& metadata);

		std::string const& getName() const;
		std::string const& getType() const;
		std::string const& getKeyword() const;
		EngineData const& getDefaultValue() const;
		void setDefaultValue(EngineData const& data);
		bool hasDefaultValue() const;

	private:

		const std::string	m_Name;
		const std::string	m_Keyword;
		const std::string	m_Type;
		EngineData			m_DefaultValue;
		bool				m_HasDefault;

	};

	inline ParameterMetadata::ParameterMetadata(std::string const& name, std::string const& type, std::string const& keyword, EngineData const& defaultValue) :
		m_Name(name),
		m_Keyword(keyword),
		m_Type(type),
		m_DefaultValue(defaultValue),
		m_HasDefault(true)
	{
	}

	inline void ParameterMetadata::setDefaultValue(EngineData const& data)
	{
		if (data.typeinfo().name() != m_Type)
		{
			std::ostringstream msg;
			msg << "datatype mismatch: " << m_Type << " vs. template parameter type " << data.typeinfo().name();
			throw TypeMismatchException(msg.str());
		}

		m_DefaultValue = data;
		m_HasDefault = true;
	}

	inline ParameterMetadata::~ParameterMetadata()
	{
	}

	inline ParameterMetadata::ParameterMetadata(std::string const& name, std::string const& type, std::string const& keyword) :
		m_Name(name),
		m_Keyword(keyword),
		m_Type(type),
		m_DefaultValue(),
		m_HasDefault(false)
	{
	}

	inline std::string const& ParameterMetadata::getName() const
	{
		return m_Name;
	}

	inline std::string const& ParameterMetadata::getKeyword() const
	{
		return m_Keyword;
	}

	inline std::string const& ParameterMetadata::getType() const
	{
		return m_Type;
	}

	inline EngineData const& ParameterMetadata::getDefaultValue() const
	{
		return m_DefaultValue;
	}

	inline bool ParameterMetadata::hasDefaultValue() const
	{
		return m_HasDefault;
	}

	inline std::ostream& operator<<(std::ostream &out, ParameterMetadata const& metadata)
	{
		out << metadata.getName() << "\t" << metadata.getKeyword();
		if (metadata.hasDefaultValue())
		{
			out << ", default value: " << metadata.getDefaultValue();
		}
		else
		{
			out << ", no default value";
		}

		return out;
	}

}