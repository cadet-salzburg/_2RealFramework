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

#include "_2RealPluginNotification.h"

namespace _2Real
{
	PluginNotification::PluginNotification(std::string _plugin, std::string _message) : m_PluginName(_plugin), m_Message(_message)
	{
	}

	PluginNotification::~PluginNotification()
	{
	}

	const std::string PluginNotification::plugin() const
	{
		return m_PluginName;
	}

	const std::string PluginNotification::message() const
	{
		return m_Message;
	}
}