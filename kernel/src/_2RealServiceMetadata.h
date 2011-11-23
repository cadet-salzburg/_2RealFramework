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

#include "_2RealParameterMetadata.h"

#include <map>
#include <list>
#include <string>

namespace _2Real
{

	/**
	*	metadata representation of a service
	*/

	class ServiceMetadata
	{

	public:

		/**
		*	
		*/
		typedef std::pair< const std::string, ServiceMetadata >	NamedService;

		/**
		*	
		*/
		typedef std::map< const std::string, ServiceMetadata >	ServiceMap;

		/**
		*	creates service metadata
		*
		*	@param _name:		service's name
		*	@param _plugin:		plugin the service belongs to
		*/
		ServiceMetadata(std::string const& _name);

		/**
		*	sets service description
		*
		*	@param _dec:		service's description
		*/
		void setDescription(std::string const& _desc);

		/**
		*	adds setup parameter
		*
		*	@param _name:		parameter's name
		*	@param _type:		parameter's type as string
		*/
		void addSetupParameter(std::string const& _name, std::string const& _type);

		/**
		*	adds input parameter
		*
		*	@param _name:		parameter's name
		*	@param _type:		parameter's type as string
		*/
		void addInputParameter(std::string const& _name, std::string const& _type);

		/**
		*	adds output parameter
		*
		*	@param _name:		parameter's name
		*	@param _type:		parameter's type as string
		*/
		void addOutputParameter(std::string const& _name, std::string const& _type);

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
		*	@param _name:		parameter's name
		*	@return:			true if service has setup param with _name
		*/
		const bool hasSetupParameter(std::string const& _name) const;

		/**
		*	@param _name:		parameter's name
		*	@return:			true if service has input param with _name
		*/
		const bool hasInputParameter(std::string const& _name) const;

		/**
		*	@param _name:		parameter's name
		*	@return:			true if service has output param with _name
		*/
		const bool hasOuputParameter(std::string const& _name) const;

		/**
		*	
		*/
		ParameterMetadata::StringMap getInputParameters() const;

		/**
		*	
		*/
		ParameterMetadata::StringMap getOutputParameters() const;

		/**
		*	
		*/
		ParameterMetadata::StringMap getSetupParameters() const;

		/**
		*	returns string with service information
		*
		*	@return:			info
		*/
		const std::string info() const;

	private:

		/**
		*	name
		*/
		const std::string									m_ServiceName;

		/**
		*	description
		*/
		std::string											m_Description;

		/**
		*	if setup can be called more than once
		*/
		bool												m_bCanReconfigure;

		/**
		*	if service is singleton
		*/
		bool												m_bIsSingleton;

		/**
		*	setup params
		*/
		ParameterMetadata::ParameterMap								m_SetupParameters;

		/**
		*	input params
		*/
		ParameterMetadata::ParameterMap								m_InputParameters;

		/**
		*	output params
		*/
		ParameterMetadata::ParameterMap								m_OutputParameters;

	};

}