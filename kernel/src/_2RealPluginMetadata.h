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
#include "_2RealServiceMetadata.h"

#include <map>
#include <string>

namespace _2Real
{

	/**
	*
	*/

	class PluginMetadata
	{

	public:

		/**
		*	helper class for plugin version
		*/
		class Version
		{

		public:

			Version(unsigned int const& _major, unsigned int const& _minor, unsigned int const& _revision);
			Version(Version const& _src);
			Version& operator=(Version const& _src);
			~Version();

			bool operator==(Version const& _rhs);
			bool operator<(Version const& _rhs);

			const std::string asString() const;
			unsigned int const& major() const;
			unsigned int const& minor() const;
			unsigned int const& revision() const;

		private:

			unsigned int		m_Major;
			unsigned int		m_Minor;
			unsigned int		m_Revision;

		};

		/**
		*	creates plugin metadata
		*
		*	@param _name:		plugin's classname
		*/
		PluginMetadata(std::string const& _name, std::string const& _path);

		/**
		*	sets plugin description
		*
		*	@param _dec:		plugin's description
		*/
		void setDescription(std::string const& _desc);

		/**
		*	sets plugin version
		*
		*	@param _version:	plugin's version
		*/
		void setVersion(Version const& _version);

		/**
		*	sets plugin author
		*
		*	@param _name:		plugin's author
		*/
		void setAuthor(std::string const& _author);

		/**
		*	sets plugin contact address
		*
		*	@param _contact:	plugin's contact address
		*/
		void setContact(std::string const& _contact);

		/**
		*	get plugin install directory
		*
		*	@return:			plugin's install directory
		*/
		std::string const& getInstallDirectory() const;

		/**
		*	get plugin description
		*
		*	@return:			plugin's description
		*/
		std::string const& getDescription() const;

		/**
		*	get plugin author
		*
		*	@return:			plugin's author
		*/
		std::string const& getAuthor() const;

		/**
		*	get plugin contact
		*
		*	@return:			plugin's contact
		*/
		std::string const& getContact() const;

		/**
		*	get plugin classname
		*
		*	@return:			plugin's classname
		*/
		std::string const& getClassname() const;

		/**
		*	get plugin version
		*
		*	@return:			plugin's version
		*/
		PluginMetadata::Version const& getVersion() const;

		/**
		*	get metadata of a particular service
		*
		*	@param _name:		name of service
		*	@return:			service's metadata
		*/
		ServiceMetadata const& getServiceMetadata(std::string const& _name) const;

		/**
		*	add metadata to file
		*
		*	@param:				metadata to add
		*/
		void addServiceMetadata(ServiceMetadata const& _info);

		/**
		*	returns string with plugin information
		*
		*	@return:			info
		*/
		const std::string info();

		/**
		*	adds setup parameter
		*
		*	@param _name:		parameter's name
		*	@param _type:		parameter's type as string
		*/
		void addSetupParam(std::string const& _name, std::string const& _type);

		/**
		*	@param _name:		parameter's name
		*	@return:			true if service has setup param with _name
		*/
		const bool hasSetupParam(std::string const& _name) const;

		/**
		*	
		*/
		ParamMetadata::StringMap getSetupParams() const;

	private:

		/**
		*	
		*/
		std::string						m_PluginName;

		/**
		*	
		*/
		std::string						m_InstallDirectory;

		/**
		*	
		*/
		std::string						m_Description;

		/**
		*	
		*/
		std::string						m_Author;

		/**
		*	
		*/
		std::string						m_Contact;

		/**
		*	
		*/
		PluginMetadata::Version			m_Version;

		/**
		*	
		*/
		ServiceMetadata::ServiceMap		m_Services;

		/**
		*	setup params
		*/
		ParamMetadata::ParamMap			m_SetupParams;

	};

}