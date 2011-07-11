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

#include "_2RealServiceNotification.h"

namespace _2Real
{
	_2RealServiceNotification::_2RealServiceNotification(std::string _service, std::string _plugin, std::string _message) : m_ServiceName(_service), m_PluginName(_plugin), m_Message(_message)
	{
	}

	_2RealServiceNotification::~_2RealServiceNotification()
	{
	}

	const std::string& _2RealServiceNotification::service() const
	{
		return m_ServiceName;
	}

	const std::string& _2RealServiceNotification::plugin() const
	{
		return m_PluginName;
	}

	const std::string& _2RealServiceNotification::message() const
	{
		return m_Message;
	}
}