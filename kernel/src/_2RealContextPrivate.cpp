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

#include "_2RealContextPrivate.h"
#include "_2RealPlugin.h"
#include "_2RealIService.h"
#include "_2RealServiceRegistration.h"
#include "_2RealServiceNotification.h"
#include "_2RealFrameworkNotification.h"
#include "_2RealPluginNotification.h"

namespace _2Real
{
	_2RealContextPrivate::_2RealContextPrivate()
	{
	}

	_2RealContextPrivate::~_2RealContextPrivate()
	{
	}

	_2RealPluginPtr _2RealContextPrivate::installPlugin(const std::string& _name, const std::string& _path)
	{
		_2RealPluginPtr plugin = _2RealPluginPtr(new _2RealPlugin(_name, _path, this));
		m_Plugins.insert(std::pair<std::string, _2RealPluginPtr>(_name, plugin));
		if (m_PluginNotificationCenter.hasObservers())
		{
			m_PluginNotificationCenter.postNotification(new _2RealPluginNotification(_name, "installed"));
		}

		return m_Plugins.find(_name)->second;
	}

	void _2RealContextPrivate::uninstallPlugin(_2RealPlugin* _plugin)
	{
		Plugins::iterator it = m_Plugins.find(_plugin->name());
		m_Plugins.erase(it);

		if (m_PluginNotificationCenter.hasObservers())
		{
			std::cout << "UNINSTALLED: " << _plugin->name() << std::endl;
			m_PluginNotificationCenter.postNotification(new _2RealPluginNotification(_plugin->name(), "uninstalled"));
		}
	}

	_2RealServiceRegPtr _2RealContextPrivate::registerService(const std::string& _name, const std::string& _plugin, _2RealServicePtr& _service)
	{
		_2RealServiceRegPtr reg = _2RealServiceRegPtr(new _2RealServiceRegistration(_name, _plugin, this, _service));
	
		m_Services.insert(std::pair<std::string, _2RealServicePtr>(_plugin + "." + _name, _service));

		if (m_ServiceNotificationCenter.hasObservers())
		{
			m_ServiceNotificationCenter.postNotification(new _2RealServiceNotification(_name, _plugin, "added"));
		}

		return reg;
	}

	void _2RealContextPrivate::unregisterService(const std::string& _name, const std::string& _plugin, _2RealServicePtr& _service)
	{
		Services::iterator it = m_Services.find(_plugin + "." + _name);
		m_Services.erase(it);
		
		if (m_ServiceNotificationCenter.hasObservers())
		{
			m_PluginNotificationCenter.postNotification(new _2RealServiceNotification(_name, _plugin, "removed"));
		}
	}

	void _2RealContextPrivate::addServiceListener(_2RealIPluginActivator& _activator)
	{
		m_ServiceNotificationCenter.addObserver(Poco::NObserver<_2RealIPluginActivator, _2RealServiceNotification>(_activator, &_2RealIPluginActivator::handleServiceNotification));
	}

	void _2RealContextPrivate::addFrameworkListener(_2RealIPluginActivator& _activator)
	{
		m_FrameworkNotificationCenter.addObserver(Poco::NObserver<_2RealIPluginActivator, _2RealFrameworkNotification>(_activator, &_2RealIPluginActivator::handleFrameworkNotification));
	}

	void _2RealContextPrivate::addPluginListener(_2RealIPluginActivator& _activator)
	{
		m_PluginNotificationCenter.addObserver(Poco::NObserver<_2RealIPluginActivator, _2RealPluginNotification>(_activator, &_2RealIPluginActivator::handlePluginNotification));
	}

	void _2RealContextPrivate::removeServiceListener(_2RealIPluginActivator& _activator)
	{
		m_ServiceNotificationCenter.removeObserver(Poco::NObserver<_2RealIPluginActivator, _2RealServiceNotification>(_activator, &_2RealIPluginActivator::handleServiceNotification));
	}

	void _2RealContextPrivate::removeFrameworkListener(_2RealIPluginActivator& _activator)
	{
		m_FrameworkNotificationCenter.removeObserver(Poco::NObserver<_2RealIPluginActivator, _2RealFrameworkNotification>(_activator, &_2RealIPluginActivator::handleFrameworkNotification));
	}

	void _2RealContextPrivate::removePluginListener(_2RealIPluginActivator& _activator)
	{
		m_PluginNotificationCenter.removeObserver(Poco::NObserver<_2RealIPluginActivator, _2RealPluginNotification>(_activator, &_2RealIPluginActivator::handlePluginNotification));
	}
}