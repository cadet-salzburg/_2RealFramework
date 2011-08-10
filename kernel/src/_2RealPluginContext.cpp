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

#include "_2RealPluginContext.h"

#include "_2RealPlugin.h"

namespace _2Real
{

	PluginContext::PluginContext(Plugin *const _plugin) : m_PluginPtr(_plugin)
	{
	}

	PluginContext::~PluginContext()
	{
		m_PluginPtr = NULL;
	}

	PluginContext::PluginContext(const PluginContext &_src) : m_PluginPtr(_src.m_PluginPtr)
	{
	}

	PluginContext& PluginContext::operator=(const PluginContext &_src) 
	{
		if (this == &_src)
		{
			return *this;
		}
 
		m_PluginPtr= _src.m_PluginPtr;
	 
		return *this;
	}

	void PluginContext::registerService(const std::string _name, UserServiceCreator _creator)
	{
		m_PluginPtr->registerService(_name, _creator, false);
	}

	void PluginContext::registerSingleton(const std::string _name, UserServiceCreator _creator)
	{
		m_PluginPtr->registerService(_name, _creator, true);
	}
}