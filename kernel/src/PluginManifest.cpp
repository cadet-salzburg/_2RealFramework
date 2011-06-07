#include "PluginManifest.h"

namespace _2Real
{
	PluginManifest::PluginManifest()
	{
		m_Attributes.clear();
	}

	PluginManifest::PluginManifest(const std::string& path)
	{
		//todo: read manifest file
	}

	PluginManifest::~PluginManifest()
	{
		m_Attributes.clear();
	}

	const bool read(const std::string& path)
	{
		//todo: read manifest file
		
		return true;
	}

	const std::string PluginManifest::getAttribute(const std::string& key)
	{
		std::map<std::string, std::string>::const_iterator it;
		it = m_Attributes.find(0);

		if (it == m_Attributes.end())
		{
			std::string result = "";
			return result;
		}

		return it->second;
	}
}