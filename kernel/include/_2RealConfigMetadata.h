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

#include "_2RealFramework.h"
#include "_2RealMetadata.h"

namespace _2Real
{
	class ConfigMetadata : public Metadata
	{

	public:

		ConfigMetadata(std::string _name);
		ConfigMetadata(const ConfigMetadata& _src);
		ConfigMetadata& operator= (const ConfigMetadata& _src);
		~ConfigMetadata();

		Data const& setupAttributes() const;
		Data const& intputAttributes() const;
		Data const& outputAttributes() const;

		template <typename T>
		const bool setupParameter(std::string _name, T& _value) const
		{
			return Metadata::attribute<T>(m_SetupPath, _name, _value);
		}
		
		template <typename T>
		const bool inputParameter(std::string _name, T& _value) const
		{
			return Metadata::attribute<T>(m_InputPath, _name, _value);
		}
		
		template <typename T>
		const bool outputParameter(std::string _name, T& _value) const
		{
			return Metadata::attribute<T>(m_OutputPath, _name, _value);
		}

	private:

		template <typename T>
		void setSetupParameter(std::string _name, const T& _value)
		{
			Metadata::setAttribute<T>(m_SetupPath, _name, _value);
		}

		template <typename T>
		void setInputParameter(std::string _name, const T& _value)
		{
			Metadata::setAttribute<T>(m_InputPath, _name, _value);
		}

		template <typename T>
		void setOutputParameter(std::string _name, const T& _value)
		{
			Metadata::setAttribute<T>(m_OutputPath, _name, _value);
		}

		std::vector<std::string>		m_SetupPath;
		std::vector<std::string>		m_InputPath;
		std::vector<std::string>		m_OutputPath;

		friend class Context;
	};
}