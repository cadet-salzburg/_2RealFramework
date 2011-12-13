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
#include "_2RealEngine.h"

#include <map>
#include <list>
#include <string>

#include "Poco/SharedPtr.h"

namespace _2Real
{

	class ParameterMetadata;
	class ServiceMetadata;
	class Typetable;
	class EngineData;

	typedef std::pair< std::string, ServiceMetadata * >			NamedServiceData;
	typedef std::map< std::string, ServiceMetadata * >			ServiceDataMap;
	typedef std::pair< std::string, ParameterMetadata * >		NamedParameterData;
	typedef std::map< std::string, ParameterMetadata * >		ParameterDataMap;

	class PluginMetadata
	{

	public:

		PluginMetadata(std::string const& classname, std::string const& directory);
		~PluginMetadata();

		void clear();

		friend std::ostream& operator<<(std::ostream &out, PluginMetadata const& metadata);

		ParameterDataMap const&		getSetupParameters() const;
		ServiceDataMap const&		getServices() const;

		void						setDescription(std::string const& desc);
		void						setVersion(Version const& version);
		void						setAuthor(std::string const& author);
		void						setContact(std::string const& contact);

		std::string const&			getInstallDirectory() const;
		std::string const&			getDescription() const;
		std::string const&			getAuthor() const;
		std::string const&			getContact() const;
		std::string const&			getClassname() const;
		Version const&				getVersion() const;

		void						addServiceMetadata(std::string const& name);
		void						setDescription(std::string const& service, std::string const& description);

		void						addSetupParameterByKey(std::string const& name, std::string const& keyword);
		void						addSetupParameterByKey(std::string const& service, std::string const& name, std::string const& keyword);
		void						addInputSlotByKey(std::string const& service, std::string const& name, std::string const& keyword);
		void						addOutputSlotByKey(std::string const& service, std::string const& name, std::string const& keyword);
		
		void						addSetupParameterByType(std::string const& name, std::string const& type);
		void						addSetupParameterByType(std::string const& service, std::string const& name, std::string const& type);
		void						addInputSlotByType(std::string const& service, std::string const& name, std::string const& type);
		void						addOutputSlotByType(std::string const& service, std::string const& name, std::string const& type);

		void						addSetupParameterByData(std::string const& name, EngineData const& defaultValue);
		void						addSetupParameterByData(std::string const& service, std::string const& name, EngineData const& defaultValue);
		void						addInputSlotByData(std::string const& service, std::string const& name, EngineData const& defaultValue);

		bool						containsServiceMetadata(std::string const& name) const;
		bool						containsParameterMetadata(std::string const& name) const;

		ServiceMetadata	&			getServiceMetadata(std::string const& name);
		ParameterMetadata const&	getParameterMetadata(std::string const& name) const;
		ServiceMetadata const&		getServiceMetadata(std::string const& name) const;

		Poco::SharedPtr< AbstractDataHolder >	m_Test;

	private:

		void						addSetupParameter(std::string const& name, ParameterMetadata *data);

		ParameterMetadata *			createParameterFromKey(std::string const& name, std::string const& keyword);
		ParameterMetadata *			createParameterFromType(std::string const& name, std::string const& type);
		ParameterMetadata *			createParameterFromData(std::string const& name, EngineData defaultValue);

		std::string					const m_Classname;
		std::string					const m_InstallDirectory;
		Typetable					const& m_Typetable;
		std::string					m_Description;
		std::string					m_Author;
		std::string					m_Contact;
		Version						m_Version;
		ServiceDataMap				m_Services;
		ParameterDataMap			m_SetupParameters;

	};

	inline ParameterDataMap const& PluginMetadata::getSetupParameters() const
	{
		return m_SetupParameters;
	}

	inline ServiceDataMap const& PluginMetadata::getServices() const
	{
		return m_Services;
	}

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