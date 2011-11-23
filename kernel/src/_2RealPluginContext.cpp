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
#include "_2RealServiceFactory.h"
#include "_2RealException.h"

#include <iostream>

namespace _2Real
{

	PluginContext::PluginContext(Plugin *const _plugin) :
		m_Plugin(_plugin)
	{
	}

	PluginContext::PluginContext(PluginContext const& _src) :
		m_Plugin(_src.m_Plugin)
	{
	}

	PluginContext& PluginContext::operator=(PluginContext const& _src) 
	{
		return *this;
	}

	void PluginContext::registerService(std::string const& _name, ServiceCreator _creator)
	{
		m_Plugin->registerService(_name, _creator);
	}

	SharedAny PluginContext::getSetupParameter(std::string const& _name)
	{
		return m_Plugin->getParameterValue(_name);
	}

}