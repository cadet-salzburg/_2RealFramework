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
#include <list>
#include <deque>

#include <iostream>

#include "Poco/SharedPtr.h"
#include "Poco/AutoPtr.h"

namespace _2Real
{
	class Context;
	class ContextPrivate;
	
	class Plugin;
	class IPluginActivator;
	class PluginContext;
	class PluginPool;
	
	class IService;
	class ServiceFactory;
	class ServiceRegistry;
	class ServiceRegistration;
	class AbstractService;
	
	template < typename T >
	class ServiceVariable;
	class AbstractServiceVariable;
	class AbstractServiceParameter;
	
	class Notification;
	class PluginNotification;
	class ServiceNotification;
	class FrameworkNotification;
	
	class Data;
	
	class Metadata;
	class ConfigMetadata;

	class IContainer;
	class ServiceContainer;
	class SequenceContainer;
	class SynchronizationContainer;
	
	typedef Poco::SharedPtr< Context >				ContextPtr;
	typedef Poco::SharedPtr< PluginContext >		PluginContextPtr;
	typedef Poco::SharedPtr< IService >				ServicePtr;
	typedef Poco::SharedPtr< Plugin >				PluginPtr;
	typedef Poco::SharedPtr< ServiceRegistration >	ServiceRegPtr;
	typedef Poco::SharedPtr< Metadata >				MetadataPtr;
	typedef Poco::SharedPtr< ConfigMetadata >		ConfigMetadataPtr;
	typedef Poco::SharedPtr< IContainer >			ContainerPtr;
	typedef Poco::SharedPtr< Data >					DataPtr;
	typedef Poco::SharedPtr< ServiceContainer >		ServiceContainerPtr;
	
	typedef Poco::AutoPtr< ServiceNotification >	ServiceNotificationPtr;
	typedef Poco::AutoPtr< PluginNotification >		PluginNotificationPtr;
	typedef Poco::AutoPtr< FrameworkNotification >	FrameworkNotificationPtr;

	typedef ServicePtr (*ServiceFactoryMethod)(void);

#ifdef _DEBUG
	typedef std::string								VariableName;
	typedef std::string								ParameterName;
	typedef std::string								ServiceName;
	typedef std::string								PluginName;
#else
	typedef std::string								VariableName;
	typedef std::string								ParameterName;
	typedef std::string								ServiceName;
	typedef std::string								PluginName;
#endif

}
