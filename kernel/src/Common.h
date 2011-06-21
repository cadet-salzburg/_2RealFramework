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

#pragma once

#include <vector>
#include <map>
#include <string>

#include <iostream>

#include "Poco/SharedPtr.h"
#include "Poco/DynamicAny.h"
#include "Poco/ClassLoader.h"
#include "Poco/ClassLibrary.h"
#include "Poco/Manifest.h"
#include "Poco/Notification.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Observer.h"
#include "Poco/NObserver.h"
#include "Poco/SAX/SAXParser.h"

namespace _2Real
{
	class Plugin;
	class PluginManifest;
	class PluginFramework;
	class PluginContext;
	class PluginFrameworkContext;
	class ServiceReference;
	class ServiceRegistration;
	class PluginNotification;
	class ServiceNotification;
	class FrameworkNotification;
	class IPluginActivator;
	class IService;

	typedef Poco::SharedPtr<IService> ServicePtr;
	typedef Poco::SharedPtr<Plugin> PluginPtr;
	typedef Poco::SharedPtr<ServiceReference> ServiceRefPtr;
	typedef Poco::SharedPtr<ServiceRegistration> ServiceRegPtr;
	typedef std::multimap<std::string, ServiceRefPtr> Services;
	typedef std::multimap<std::string, std::string> PluginMetadata;
	typedef std::map<std::string, PluginPtr> Plugins;
	typedef std::map<std::string, Poco::DynamicAny> ServiceProperties;
	typedef Poco::ClassLoader<IPluginActivator> PluginLoader;
	typedef Poco::AutoPtr<ServiceNotification> ServiceNotificationPtr;
	typedef Poco::AutoPtr<PluginNotification> PluginNotificationPtr;
	typedef Poco::AutoPtr<FrameworkNotification> FrameworkNotificationPtr;
	typedef Poco::XML::SAXParser XMLParser;
}
