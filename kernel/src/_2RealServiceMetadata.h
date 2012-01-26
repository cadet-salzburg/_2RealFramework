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

	class ParameterMetadata;

	typedef std::pair< std::string, ParameterMetadata * >	NamedParameterData;
	typedef std::map< std::string, ParameterMetadata * >	ParameterDataMap;

	class ServiceMetadata
	{

	public:

		ServiceMetadata(std::string const& name);
		ServiceMetadata::~ServiceMetadata();

		friend std::ostream& operator<<(std::ostream &out, ServiceMetadata const& metadata);

		void setDescription(std::string const& description);
		void addSetupParameter(std::string const& name, ParameterMetadata *data);
		void addInputSlot(std::string const& name, ParameterMetadata *data);
		void addOutputSlot(std::string const& name, ParameterMetadata *data);

		std::string const& getName() const;
		std::string const& getDescription() const;

		ParameterDataMap const& getSetupParameters() const;
		ParameterDataMap const& getInputSlots() const;
		ParameterDataMap const& getOutputSlots() const;

	private:

		std::string			const m_ServiceName;
		std::string			m_Description;
		ParameterDataMap	m_SetupParameters;
		ParameterDataMap	m_InputSlots;
		ParameterDataMap	m_OutputSlots;

	};

	inline ParameterDataMap const& ServiceMetadata::getSetupParameters() const
	{
		return m_SetupParameters;
	}

	inline ParameterDataMap const& ServiceMetadata::getInputSlots() const
	{
		return m_InputSlots;
	}

	inline ParameterDataMap const& ServiceMetadata::getOutputSlots() const
	{
		return m_OutputSlots;
	}

	inline std::string const& ServiceMetadata::getName() const
	{
		return m_ServiceName;
	}

	inline std::string const& ServiceMetadata::getDescription() const
	{
		return m_Description;
	}

	inline void ServiceMetadata::setDescription(std::string const& description)
	{
		m_Description = description;
	}

}