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

#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Mutex.h"
#include "Poco/SharedPtr.h"
#include "Poco/DynamicAny.h"
#include "Poco/ClassLoader.h"
#include "Poco/ClassLibrary.h"
#include "Poco/Manifest.h"
#include "Poco/Notification.h"
#include "Poco/NotificationQueue.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Observer.h"
#include "Poco/NObserver.h"
#include "Poco/SAX/SAXParser.h"

namespace _2Real
{

	class _2RealContext;
	class _2RealContextPrivate;
	class _2RealPlugin;
	class _2RealPluginContext;
	class _2RealServiceRegistration;
	//class _2RealServiceReference;
	class _2RealPluginNotification;
	class _2RealServiceNotification;
	class _2RealFrameworkNotification;
	class _2RealIPluginActivator;
	class _2RealIService;
	
	typedef Poco::ClassLoader<_2RealIPluginActivator> _2RealPluginLoader;
	
	typedef Poco::SharedPtr<_2RealContext> _2RealContextPtr;
	typedef Poco::SharedPtr<_2RealPlugin> _2RealPluginPtr;
	typedef Poco::SharedPtr<_2RealPluginContext> _2RealPluginContextPtr;
	typedef Poco::SharedPtr<_2RealIService> _2RealServicePtr;
	typedef Poco::SharedPtr<_2RealServiceRegistration> _2RealServiceRegPtr;

	typedef Poco::AutoPtr<_2RealServiceNotification> _2RealServiceNotificationPtr;
	typedef Poco::AutoPtr<_2RealPluginNotification> _2RealPluginNotificationPtr;
	typedef Poco::AutoPtr<_2RealFrameworkNotification> _2RealFrameworkNotificationPtr;

	//todo: maybe use own class (plugin pool & service pool) instead
	typedef std::map<std::string, _2RealPluginPtr> Plugins;
	typedef std::map<std::string, _2RealServicePtr> Services;

	typedef Poco::XML::SAXParser _2RealXMLParser;
}
