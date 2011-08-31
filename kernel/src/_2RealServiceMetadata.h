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
#include <list>
#include <string>

namespace _2Real
{

	/**
	*	metadata representation of a service
	*/

	class PluginMetadata;

	class ServiceMetadata
	{

		/**
		*	metadata representation of a param
		*/
		class ParamMetadata
		{

		public:

			/**
			*	ctor will throw if either _name or _type is empty
			*/
			ParamMetadata(std::string const& _name, std::string const& _type) throw(...);

			/**
			*	get parameter name
			*
			*	@return:		parameter's name
			*/
			std::string getName() const;

			/**
			*	get parameter type
			*
			*	@return:		parameter's type as string
			*/
			std::string getType() const;

		private:

		/**
		*	name of the param
		*/
		const std::string		m_Name;

		/*
		*	typename as string
		*/
		const std::string		m_Type;

		};

	public:

		typedef std::list< std::string >	ParamList;

		/**
		*	creates service metadata
		*
		*	@param _name:		service's name
		*	@param _plugin:		plugin the service belongs to
		*	@throw:				TODO
			@throw:				TODO
		*/
		ServiceMetadata(std::string const& _name) throw(...);

		/**
		*	sets service description
		*
		*	@param _dec:		service's description
		*/
		void setDescription(std::string const& _desc) throw(...);

		/**
		*	service's reconfiguration status
		*
		*	@param _config:		if service's setup method can be called multiple times
		*/
		void setReconfiguration(bool const& _config) throw(...);

		/**
		*	service's singleton status
		*
		*	@param _singleton:	true if service is a singleton
		*/
		void setSingleton(bool const& _singleton) throw(...);

		/**
		*	adds setup parameter
		*
		*	@param _name:		parameter's name
		*	@param _type:		parameter's type as string
		*/
		void addSetupParam(std::string const& _name, std::string const& _type) throw(...);

		/**
		*	adds input parameter
		*
		*	@param _name:		parameter's name
		*	@param _type:		parameter's type as string
		*/
		void addInputParam(std::string const& _name, std::string const& _type) throw(...);

		/**
		*	adds output parameter
		*
		*	@param _name:		parameter's name
		*	@param _type:		parameter's type as string
		*/
		void addOutputParam(std::string const& _name, std::string const& _type) throw(...);

		/**
		*	adds userclass
		*
		*	@param _name:		userclass' name
		*/
		void addUserclass(std::string const& _name) throw(...);

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
		*	returns service's reconfiguration status
		*
		*	@return:			true if service's setup method can be called multiple times
		*/
		bool const& canReconfigure() const;

		/**
		*	returns service's singleton status
		*
		*	@return:			true if plugin is singleton
		*/
		bool const& isSingleton() const;

		/**
		*	@param _name:		parameter's name
		*	@return:			true if service has setup param with _name
		*/
		const bool hasSetupParam(std::string const& _name) const;

		/**
		*	@param _name:		parameter's name
		*	@return:			true if service has input param with _name
		*/
		const bool hasInputParam(std::string const& _name) const;

		/**
		*	@param _name:		parameter's name
		*	@return:			true if service has output param with _name
		*/
		const bool hasOuputParam(std::string const& _name) const;

		/**
		*	@param _name:		userclass' name
		*	@return:			true if service has userclass with _name
		*/
		const bool hasUserclass(std::string const& _name) const;

		/**
		*	
		*/
		ParamList getInputParams() const;

		/**
		*	
		*/
		ParamList getOutputParams() const;

		/**
		*	
		*/
		ParamList getSetupParams() const;

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
		const std::string				m_ServiceName;

		/**
		*	description
		*/
		std::string						m_Description;

		/**
		*	if setup can be called more than once
		*/
		bool							m_bCanReconfigure;

		/**
		*	if service is singleton
		*/
		bool							m_bIsSingleton;

		/**
		*	yay, typedefs
		*/
		typedef std::pair< std::string, ParamMetadata >	NamedParam;

		/**
		*	typedefs, yay
		*/
		typedef std::map< std::string, ParamMetadata >	ParamMap;

		/**
		*	setup params
		*/
		ParamMap											m_SetupParams;

		/**
		*	input params
		*/
		ParamMap											m_InputParams;

		/**
		*	output params
		*/
		ParamMap											m_OutputParams;

		/**
		*	
		*/
		typedef std::pair< std::string, std::string >		NamedUserclass;

		/**
		*	
		*/
		typedef std::map< std::string, std::string >		UserclassMap;

		/**
		*	user classes (for the time being)
		*/
		UserclassMap										m_Userclasses;

	};

}