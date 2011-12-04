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
#include "_2RealSetupParameter.h"

namespace _2Real
{

	PluginContext::PluginContext(Plugin &plugin) :
		m_Plugin(plugin)
	{
	}

	PluginContext::PluginContext(PluginContext const& src) :
		m_Plugin(src.m_Plugin)
	{
	}

	PluginContext& PluginContext::operator=(PluginContext const& src) 
	{
		return *this;
	}

	void PluginContext::registerService(std::string const& name, ServiceCreator creator)
	{
		if (m_Plugin.canExportService(name) && !m_Plugin.exportsService(name))
		{
			m_Plugin.registerService(name, creator);
		}
		else
		{
			throw InvalidServiceException(name);
		}
	}

	EngineData PluginContext::getSetupParameter(std::string const& name)
	{
		if (m_Plugin.hasSetupParameter(name))
		{
			return m_Plugin.getSetupParameter(name).get();
		}
		else
		{
			throw InvalidParameterException("setup parameter", name);
		}
	}

}