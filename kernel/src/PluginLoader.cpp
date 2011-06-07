#include "Poco\Path.h"

#include "PluginLoader.h"
#include "PluginActivator.h"
#include "Plugin.h"

namespace _2Real
{
	PluginLoader::PluginLoader()
	{
		printf("PluginLoader::ctor\n");

		std::string pluginDir(Poco::Path::current() + "..\\..\\plugin\\");
		m_PluginDirectories.clear();	
		m_PluginDirectories.push_back(pluginDir);
	}

	PluginLoader::~PluginLoader()
	{
		printf("PluginLoader::destructor\n");

		m_PluginDirectories.clear();
	}

	void PluginLoader::addPluginDirectory(const std::string& directory)
	{	
		std::cout << "PluginLoader::addPluginDirectory" << std::endl;
		
		std::vector<std::string>::iterator it;
		for (it=m_PluginDirectories.begin(); it<m_PluginDirectories.end(); it++)
		{
			if ((*it).compare(directory) == 0)
			{
				break;
			}
		}

		if (it == m_PluginDirectories.end())
		{
			m_PluginDirectories.push_back(directory);
		}
	}

	void PluginLoader::removePluginDirectory(const std::string& directory)
	{		
		std::cout << "PluginLoader::removePluginDirectory" << std::endl;
		
		std::vector<std::string>::iterator it;
		for (it=m_PluginDirectories.begin(); it<m_PluginDirectories.end(); it++)
		{
			if ((*it).compare(directory) == 0)
			{
				break;
			}
		}

		if (it != m_PluginDirectories.end())
		{
			m_PluginDirectories.erase(it);
		}
	}

	const PluginPtr PluginLoader::loadPlugin(const std::string& symbolicname)
	{
		std::cout << "PluginLoader::loadPlugin" << std::endl;

		//Plugin* plugin = new Plugin(/**/);
		//PluginPtr ptr(plugin);

		return NULL;
	}
}