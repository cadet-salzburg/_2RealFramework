#pragma once

#include "Common.h"

/*
	class to read & stor plugin metadata
*/

namespace _2Real
{
	class PluginManifest
	{
	public:
		
		PluginManifest();
		PluginManifest(const std::string& path);
		~PluginManifest();

		const bool read(const std::string& path);
		const std::string getAttribute(const std::string& key);

	private:
		 
		PluginMetadata m_Attributes;
	};
}
