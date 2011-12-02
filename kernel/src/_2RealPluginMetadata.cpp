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

#include "_2RealPluginMetadata.h"
#include "_2RealServiceMetadata.h"
#include "_2RealException.h"
#include "_2RealTypes.h"

#include <iostream>
#include <sstream>

namespace _2Real
{

	PluginMetadata::PluginMetadata(std::string const& classname, std::string const& path, StringMap const& types) :
		m_Classname(classname),
		m_InstallDirectory(path),
		m_Description("this plugin is holy"),
		m_Author("the spaghetti monster"),
		m_Contact("through prayer"),
		m_Version(Version(0, 0, 0)),
		m_Services(),
		m_SetupParameters(),
		m_AllowedTypes(types)
	{
	}

	//TypeTable const& PluginMetadata::types() const
	//{
	//	return *m_Types;
	//}

	//std::ofstream & PluginMetadata::logstream()
	//{
	//	return m_Logstream;
	//}

	ServiceMetadata & PluginMetadata::addServiceMetadata(std::string const& name)
	{
		ServiceDataMap::iterator it = m_Services.find(name);

		if (it != m_Services.end())
		{
			std::ostringstream msg;
			msg << "metadata error: service " << name << " already exists";
			throw _2Real::Exception(msg.str());
		}

		ServiceMetadata metadata(name, m_AllowedTypes);
		m_Services.insert(NamedServiceData(name, metadata));
	}

	void PluginMetadata::setDescription(std::string const& desc)
	{
		m_Description = desc;
	}

	void PluginMetadata::setVersion(Version const& version)
	{
		m_Version = version;
	}

	void PluginMetadata::setAuthor(std::string const& author)
	{
		m_Author = author;
	}

	void PluginMetadata::setContact(std::string const& contact)
	{
		m_Contact = contact;
	}

	std::string const& PluginMetadata::getClassname() const
	{
		return m_Classname;
	}

	std::string const& PluginMetadata::getInstallDirectory() const
	{
		return m_InstallDirectory;
	}

	std::string const& PluginMetadata::getDescription() const
	{
		return m_Description;
	}

	std::string const& PluginMetadata::getAuthor() const
	{
		return m_Author;
	}

	std::string const& PluginMetadata::getContact() const
	{
		return m_Contact;
	}

	Version const& PluginMetadata::getVersion() const
	{
		return m_Version;
	}

	ServiceMetadata const& PluginMetadata::getServiceMetadata(std::string const& name) const
	{
		ServiceDataMap::const_iterator it = m_Services.find(name);

		if (it == m_Services.end())
		{
			std::ostringstream msg;
			msg << "metadata error: service " << name << " not found";
			throw _2Real::Exception(msg.str());
		}

		return it->second;
	}

	void PluginMetadata::addSetupParameter(std::string const& name, std::string const& type)
	{
		ParameterDataMap::iterator it = m_SetupParameters.find(name);

		if (it != m_SetupParameters.end())
		{
			std::ostringstream msg;
			msg << "metadata error: setup parameter " << name << " already exists";
			throw _2Real::Exception(msg.str());
		}

		m_SetupParameters.insert(NamedParameterData(name, ParameterMetadata(name, type)));
	}

	//const StringMap PluginMetadata::getSetupParameters() const
	//{
	//	std::map< std::string, std::string > result;
	//	for (ParameterMetadata::ParameterMap::const_iterator it = m_SetupParameters.begin(); it !=m_SetupParameters.end(); it++)
	//	{
	//		ParameterMetadata data = it->second;
	//		result.insert(std::make_pair(data.getName(), data.getType()));
	//	}

	//	return result;
	//}

	const std::string PluginMetadata::info() const
	{
		std::ostringstream info;
		info << std::endl;
		info << "plugin:\t" << m_Classname << std::endl;
		info << "description:\t" << m_Description << std::endl;
		info << "installed at\t" << m_InstallDirectory << std::endl;
		info << "written by\t" << m_Author << std::endl;
		info << "contact\t\t" << m_Contact << std::endl;
		info << "version\t\t" << m_Version.asString() << std::endl;

		if (!m_SetupParameters.empty())
		{
			info << "this plugin has setup parameters:\t" << std::endl;
			for (ParameterDataMap::const_iterator it = m_SetupParameters.begin(); it != m_SetupParameters.end(); it++)
			{
				info << it->first << ":\t" << it->second.getType() << std::endl;
			}
		}

		info << "this plugin exports the following services:" << std::endl;
		for (ServiceDataMap::const_iterator it = m_Services.begin(); it != m_Services.end(); it++)
		{
			info << "service\t" << it->first << std::endl;
			info << it->second.info();
		}

		return info.str();
	}

}