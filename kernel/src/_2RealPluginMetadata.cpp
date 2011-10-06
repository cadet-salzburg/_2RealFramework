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

#include <iostream>
#include <sstream>

namespace _2Real
{

	PluginMetadata::Version::Version(unsigned int const& _major, unsigned int const& _minor, unsigned int const& _revision) :
		m_Major(_major),
		m_Minor(_minor),
		m_Revision(_revision)
	{
	}

	PluginMetadata::Version::Version(Version const& _src) :
		m_Major(_src.m_Major),
		m_Minor(_src.m_Minor),
		m_Revision(_src.m_Revision)
	{
	}

	PluginMetadata::Version::~Version()
	{
	}

	PluginMetadata::Version& PluginMetadata::Version::operator=(Version const& _src)
	{
		m_Major = _src.m_Major;
		m_Minor = _src.m_Minor;
		m_Revision = _src.m_Revision;

		return *this;
	}

	bool PluginMetadata::Version::operator==(Version const& _rhs)
	{
		return (m_Major == _rhs.m_Major && m_Minor == _rhs.m_Minor && m_Revision == _rhs.m_Revision);
	}

	bool PluginMetadata::Version::operator<(Version const& _rhs)
	{
		return (m_Major < _rhs.m_Major && m_Minor < _rhs.m_Minor && m_Revision < _rhs.m_Revision);
	}

	const std::string PluginMetadata::Version::asString() const
	{
		std::stringstream result;
		result << m_Major << "." << m_Minor << "." << m_Revision;
		return result.str();
	}

	unsigned int const& PluginMetadata::Version::major() const
	{
		return m_Major;
	}

	unsigned int const& PluginMetadata::Version::minor() const
	{
		return m_Minor;
	}

	unsigned int const& PluginMetadata::Version::revision() const
	{
		return m_Revision;
	}

	PluginMetadata::PluginMetadata(std::string const& _name, std::string const& _path) :
		m_PluginName(_name),
		m_InstallDirectory(_path),
		m_Description("this plugin is holy"),
		m_Author("the spaghetti monster"),
		m_Contact("through prayer"),
		m_Version(PluginMetadata::Version(0, 0, 0)),
		m_Services()
	{
	}

	void PluginMetadata::setDescription(std::string const& _desc)
	{
		m_Description = _desc;
	}

	void PluginMetadata::setVersion(PluginMetadata::Version const& _version)
	{
		m_Version = _version;
	}

	void PluginMetadata::setAuthor(std::string const& _author)
	{
		m_Author = _author;
	}

	void PluginMetadata::setContact(std::string const& _contact)
	{
		m_Contact = _contact;
	}

	std::string const& PluginMetadata::getClassname() const
	{
		return m_PluginName;
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

	PluginMetadata::Version const& PluginMetadata::getVersion() const
	{
		return m_Version;
	}

	void PluginMetadata::addServiceMetadata(ServiceMetadata const& _info)
	{
		m_Services.insert(ServiceMetadata::NamedService(_info.getName(), _info));
	}

	ServiceMetadata const& PluginMetadata::getServiceMetadata(std::string const& _name) const
	{
		ServiceMetadata::ServiceMap::const_iterator it = m_Services.find(_name);

		if (it == m_Services.end())
		{
			throw Exception("could not retrieve service metadata - metadata not found");
		}

		return it->second;
	}

	void PluginMetadata::addSetupParam(std::string const& _name, std::string const& _type)
	{
		ParamMetadata::ParamMap::iterator it = m_SetupParams.find(_name);

		if (it != m_SetupParams.end())
		{
			throw Exception("setup parameter " + _name + " already exists");
		}

		m_SetupParams.insert(ParamMetadata::NamedParam(_name, ParamMetadata(_name, _type)));
	}

	ParamMetadata::StringMap PluginMetadata::getSetupParams() const
	{
		ParamMetadata::StringMap result;
		for (ParamMetadata::ParamMap::const_iterator it = m_SetupParams.begin(); it !=m_SetupParams.end(); it++)
		{
			ParamMetadata data = it->second;
			result.insert(std::make_pair(data.getName(), data.getType()));
		}
		return result;
	}

	const bool PluginMetadata::hasSetupParam(std::string const& _name) const
	{
		ParamMetadata::ParamMap::const_iterator it = m_SetupParams.find(_name);

		if (it != m_SetupParams.end())
		{
			return false;
		}

		return true;
	}

	const std::string PluginMetadata::info()
	{
		std::stringstream info;
		info << std::endl;
		info << "plugin:\t" <<m_PluginName << std::endl;
		info << "description:\t" << m_Description << std::endl;
		info << "installed at\t" << m_InstallDirectory << std::endl;
		info << "written by\t" << m_Author << std::endl;
		info << "contact\t\t" << m_Contact << std::endl;
		info << "version\t\t" << m_Version.asString() << std::endl;

		if (!m_SetupParams.empty())
		{
			info << "this plugin has setup parameters:\t" << std::endl;
			for (ParamMetadata::ParamMap::const_iterator it = m_SetupParams.begin(); it != m_SetupParams.end(); it++)
			{
				info << it->first << ":\t" << it->second.getType() << std::endl;
			}
		}

		info << "this plugin exports the following services:" << std::endl;
		for (ServiceMetadata::ServiceMap::iterator it = m_Services.begin(); it != m_Services.end(); it++)
		{
			info << "service\t" << it->first << std::endl;
		}

		return info.str();
	}

}