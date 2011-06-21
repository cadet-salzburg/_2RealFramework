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

#include "ServiceNotification.h"

namespace _2Real
{
	ServiceNotification::ServiceNotification(std::string service, std::string plugin, std::string msg) : m_ServiceName(service), m_PluginName(plugin), m_Message(msg)
	{
	}

	ServiceNotification::~ServiceNotification()
	{
	}

	const std::string& ServiceNotification::service() const
	{
		return m_ServiceName;
	}

	const std::string& ServiceNotification::plugin() const
	{
		return m_PluginName;
	}

	const std::string& ServiceNotification::message() const
	{
		return m_Message;
	}
}