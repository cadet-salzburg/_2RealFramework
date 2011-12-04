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

	/**
	*	metadata representation of a service
	*/

	class ParameterMetadata;

	typedef std::pair< std::string, ParameterMetadata * >	NamedParameterData;
	typedef std::map< std::string, ParameterMetadata * >	ParameterDataMap;
	typedef std::map< std::string, std::string >			StringMap;

	class ServiceMetadata
	{

	public:

		ServiceMetadata::~ServiceMetadata();

		/**
		*	sets service description
		*
		*	@param decscription:	service's description
		*/
		void setDescription(std::string const& description);

		/**
		*	adds setup parameter
		*
		*	@param name:			parameter's name
		*	@param type:			parameter's keyword
		*/
		void addSetupParameter(std::string const& name, std::string const& keyword);

		/**
		*	adds input slot
		*
		*	@param name:			slot's name
		*	@param type:			slot's keyword
		*/
		void addInputSlot(std::string const& name, std::string const& keyword);

		/**
		*	adds output slot
		*
		*	@param name:			slot's name
		*	@param type:			slot's keyword
		*/
		void addOutputSlot(std::string const& name, std::string const& keyword);

		/**
		*	get service's name
		*
		*	@return:			service's name
		*/
		std::string const& getName() const;

		/**
		*	get service's description
		*
		*	@return:			service's description
		*/
		std::string const& getDescription() const;

		/**
		*	@param name:		parameter's name
		*	@return:			true if service has a setup parameter with name
		*/
		const bool containsSetupParameter(std::string const& name) const;

		/**
		*	@param name:		parameter's name
		*	@return:			true if service has an input slot with name
		*/
		const bool containsInputParameter(std::string const& name) const;

		/**
		*	@param name:		parameter's name
		*	@return:			true if service has an output slot with name
		*/
		const bool containsOutputParameter(std::string const& name) const;

		/**
		*	@return: map with setup parameters, name = key, keyword = value
		*/
		const std::map< std::string, std::string > getInputParameters() const;

		/**
		*	@return: map with input slots, name = key, keyword = value
		*/
		const std::map< std::string, std::string > getOutputParameters() const;

		/**
		*	@return: map with output slot, name = key, keyword = value
		*/
		const std::map< std::string, std::string > getSetupParameters() const;

		/**
		*	returns string with service information
		*
		*	@return:			information
		*/
		const std::string getInfoString() const;

	private:

		friend class PluginMetadata;

		ServiceMetadata(std::string const& name, StringMap const& allowedTypes);

		/**
		*	name
		*/
		std::string											const m_ServiceName;

		/**
		*	allowed parameter types
		*/
		StringMap											const& m_AllowedTypes;

		/**
		*	description
		*/
		std::string											m_Description;

		/**
		*	setup parameters
		*/
		ParameterDataMap									m_SetupParameters;

		/**
		*	input slots
		*/
		ParameterDataMap									m_InputSlots;

		/**
		*	output slots
		*/
		ParameterDataMap									m_OutputSlots;

	};

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