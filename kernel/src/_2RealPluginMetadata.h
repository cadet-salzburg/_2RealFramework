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

#include <map>
#include <list>
#include <typeinfo.h>

namespace _2Real
{

	class ParameterMetadata;
	class ServiceMetadata;

	typedef std::pair< std::string, ServiceMetadata * >		NamedServiceData;
	typedef std::map< std::string, ServiceMetadata * >		ServiceDataMap;
	typedef std::pair< std::string, ParameterMetadata * >	NamedParameterData;
	typedef std::map< std::string, ParameterMetadata * >	ParameterDataMap;
	typedef std::map< std::string, std::string >			StringMap;

	class PluginMetadata
	{

	public:

		~PluginMetadata();

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
		*	get plugin version
		*
		*	@return:				plugin's version
		*/
		PluginMetadata::Version const& getVersion() const;

		/**
		*	add setup parameter
		*
		*	@param name:			parameter's name
		*/
		template< typename T >
		void addSetupParameter(std::string const& name)
		{
			addSetupParameterByType(name, typeid(T).name());
		}

		/**
		*	add metadata
		*
		*	@param name:			service name
		*	@return:				service metadata
		*/
		void addServiceMetadata(std::string const& name);

		/**
		*	set service description
		*/
		void setDescription(std::string const& service, std::string const& description);

		/**
		*	add service setup parameter
		*/
		template< typename T >
		void addSetupParameter(std::string const& service, std::string const& name)
		{
			addSetupParameterByType(service, name, typeid(T).name());
		}

		/**
		*	add service input slot
		*/
		template< typename T >
		void addInputSlot(std::string const& service, std::string const& name)
		{
			addInputSlotByType(service, name, typeid(T).name());
		}

		/**
		*	add service output slot
		*/
		template< typename T >
		void addOutputSlot(std::string const& service, std::string const& name)
		{
			addOutputSlotByType(service, name, typeid(T).name());
		}

		const std::string getInfoString() const;

	private:

		friend class Plugin;
		friend class MetadataReader;

		PluginMetadata(std::string const& classname, std::string const& directory, StringMap const& allowedTypes);

		const std::string lookupKeyword(std::string const& type) const;

		void addSetupParameterByKey(std::string const& name, std::string const& keyword);
		void addSetupParameterByKey(std::string const& service, std::string const& name, std::string const& keyword);
		void addInputSlotByKey(std::string const& service, std::string const& name, std::string const& keyword);
		void addOutputSlotByKey(std::string const& service, std::string const& name, std::string const& keyword);
		
		void addSetupParameterByType(std::string const& name, std::string const& type);
		void addSetupParameterByType(std::string const& service, std::string const& name, std::string const& type);
		void addInputSlotByType(std::string const& service, std::string const& name, std::string const& type);
		void addOutputSlotByType(std::string const& service, std::string const& name, std::string const& type);
		
		ServiceMetadata const& getServiceMetadata(std::string const& name) const;
		ParameterMetadata const& getParameterMetadata(std::string const& name) const;
		ServiceMetadata & getServiceMetadata(std::string const& name);
		ParameterMetadata & getParameterMetadata(std::string const& name);
		bool containsServiceMetadata(std::string const& name) const;
		const std::list< std::string > getServices() const;
		bool containsParameterMetadata(std::string const& name) const;
		const std::list< std::string > getSetupParameters() const;

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

	inline void PluginMetadata::setDescription(std::string const& desc)
	{
		m_Description = desc;
	}

	inline void PluginMetadata::setVersion(Version const& version)
	{
		m_Version = version;
	}

	inline void PluginMetadata::setAuthor(std::string const& author)
	{
		m_Author = author;
	}

	inline void PluginMetadata::setContact(std::string const& contact)
	{
		m_Contact = contact;
	}

	inline std::string const& PluginMetadata::getClassname() const
	{
		return m_Classname;
	}

	inline std::string const& PluginMetadata::getInstallDirectory() const
	{
		return m_InstallDirectory;
	}

	inline std::string const& PluginMetadata::getDescription() const
	{
		return m_Description;
	}

	inline std::string const& PluginMetadata::getAuthor() const
	{
		return m_Author;
	}

	inline std::string const& PluginMetadata::getContact() const
	{
		return m_Contact;
	}

	inline Version const& PluginMetadata::getVersion() const
	{
		return m_Version;
	}

}