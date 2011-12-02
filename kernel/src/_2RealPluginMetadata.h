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

#include "_2RealVersion.h"
#include "_2RealServiceMetadata.h"

//#include <map>
//#include <string>
//#include <list>
//#include <fstream>
//#include <vector>
#include <typeinfo.h>

namespace _2Real
{

	class TypeTable;

	typedef std::pair< std::string, ServiceMetadata >	NamedServiceData;
	typedef std::map< std::string, ServiceMetadata >	ServiceDataMap;

	class PluginMetadata
	{

	public:

		/**
		*	sets plugin description
		*
		*	@param description:		plugin's description
		*/
		void setDescription(std::string const& desc);

		/**
		*	sets plugin version
		*
		*	@param version:			plugin's version
		*/
		void setVersion(Version const& version);

		/**
		*	sets plugin author
		*
		*	@param name:			plugin's author
		*/
		void setAuthor(std::string const& author);

		/**
		*	sets plugin contact address
		*
		*	@param contact:			plugin's contact address
		*/
		void setContact(std::string const& contact);

		/**
		*	get plugin install directory
		*
		*	@return:				plugin's install directory
		*/
		std::string const& getInstallDirectory() const;

		/**
		*	get plugin description
		*
		*	@return:				plugin's description
		*/
		std::string const& getDescription() const;

		/**
		*	get plugin author
		*
		*	@return:				plugin's author
		*/
		std::string const& getAuthor() const;

		/**
		*	get plugin contact
		*
		*	@return:				plugin's contact
		*/
		std::string const& getContact() const;

		/**
		*	get plugin classname
		*
		*	@return:				plugin's classname
		*/
		std::string const& getClassname() const;

		/**
		*	get plugin filename
		*
		*	@return:				plugin's dll filename
		*/
		std::string const& getFilename() const;

		/**
		*	get plugin version
		*
		*	@return:				plugin's version
		*/
		PluginMetadata::Version const& getVersion() const;

		/**
		*	returns string with plugin information
		*
		*	@return:				info
		*/
		const std::string info() const;

		/**
		*	add setup parameter
		*
		*	@param name:			parameter's name
		*	@param type:			parameter's keyword
		*/
		void addSetupParameter(std::string const& name, std::string const& keyword);

		/**
		*	add setup parameter without keyword
		*
		*	@param name:			parameter's name
		*/
		template< typename T >
		void addSetupParameter(std::string const& name)
		{
			addSetupParameterByType(name, typeid(T).name());
		}

		/**
		*	true if the metadata contains the parameter metadata for a setup parameter with name
		*/
		bool containsParameterMetadata(std::string const& name) const;

		/**
		*	returns service metadata
		*/
		ParameterMetadata const& getParameterMetadata(std::string const& name) const;

		/**
		*	add metadata
		*
		*	@param name:			service name
		*	@return:				service metadata
		*/
		ServiceMetadata & addServiceMetadata(std::string const& name);

		/**
		*	true if the metadata contains the service metadata for a service with name
		*/
		bool containsServiceMetadata(std::string const& name) const;

		/**
		*	returns service metadata
		*/
		ServiceMetadata const& getServiceMetadata(std::string const& name) const;

	private:

		friend class Plugin;

		PluginMetadata(std::string const& classname, std::string const& directory, StringMap const& types);

		void addSetupParameterByType(std::string const& name, std::string const& type);

		std::string				const m_Classname;
		std::string				const m_InstallDirectory;
		std::string				m_Description;
		std::string				m_Author;
		std::string				m_Contact;
		Version					m_Version;
		StringMap				const& m_AllowedTypes;
		ServiceDataMap			m_Services;
		ParameterDataMap		m_SetupParameters;

	};

}