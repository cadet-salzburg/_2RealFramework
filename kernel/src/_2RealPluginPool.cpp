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

#include "_2RealPluginPool.h"
#include "_2RealPlugin.h"

namespace _2Real
{

	//PluginPtr PluginPool::getPlugin(std::string const& _plugin)
	//{
	//	NamedPluginMap::iterator it = m_PluginMap.find(_plugin);

	//	if (it != m_PluginMap.end())
	//	{
	//		return it->second;
	//	}

	//	return PluginPtr();
	//}

	//bool PluginPool::installPlugin(PluginPtr _pluginPtr)
	//{
	//	m_PluginMap.insert(NamedPlugin(_pluginPtr->name(), _pluginPtr));

	//	_pluginPtr->load();

	//	_pluginPtr->start();

	//	return (_pluginPtr->state() == Plugin::ACTIVE);
	//}

	//void PluginPool::uninstallPlugin(std::string const& _plugin)
	//{
	//	NamedPluginMap::iterator it = m_PluginMap.find(_plugin);

	//	if (it != m_PluginMap.end())
	//	{
	//		PluginPtr tmp = it->second;
	//		tmp->stop();
	//		tmp->unload();
	//	}

	//	m_PluginMap.erase(it);
	//}

}