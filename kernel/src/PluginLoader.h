#pragma once

#include "Common.h"
#include "PluginActivator.h"

namespace _2Real
{
	class PluginLoader
	{
	public:

		PluginLoader();
		~PluginLoader();

		const PluginPtr loadPlugin(const std::string& symbolicname);
		void addPluginDirectory(const std::string& directory);
		void removePluginDirectory(const std::string& directory);

	private:

		PluginDirectories m_PluginDirectories;
		PluginClassLoader m_ClassLoader;
	};
}