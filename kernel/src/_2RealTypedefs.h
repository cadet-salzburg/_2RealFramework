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

#include "Poco/SharedPtr.h"

namespace _2Real
{
	class Context;
	class Framework;
	
	class Plugin;
	class PluginContext;
	class PluginPool;
	
	class IPluginActivator;
	class AbstractPluginActivator;
	
	class IService;
	class InternalService;
	class ServiceContext;
	class IUserService;
	
	class ServiceFactory;
	class ServiceReference;
	
	class AbstractServiceVariable;
	template < typename T >
	class ServiceVariable;
	
	class Data;
	class Metadata;
	class ConfigMetadata;

	class IContainer;
	class AbstractContainer;
	
	class ServiceContainer;
	class GroupContainer;
	class SequenceContainer;
	class SynchronizationContainer;
	class OutputContainer;

#ifdef _DEBUG
	typedef std::string								Variable;
	typedef std::string								ServiceName;
#else
	typedef unsigned int							Variable;
	typedef unsigned int							ServiceName;
#endif
	
	typedef Poco::SharedPtr< Context >				ContextPtr;

	typedef Poco::SharedPtr< Plugin >				PluginPtr;

	typedef Poco::SharedPtr< IService >				ServicePtr;
	typedef Poco::SharedPtr< IUserService >			UserServicePtr;

	typedef ServicePtr (*ServiceCreator)(ServiceName const& _name, IUserService *const _service);
	typedef UserServicePtr (*UserServiceCreator)(void);
	
	typedef Poco::SharedPtr< Metadata >				MetadataPtr;
	typedef Poco::SharedPtr< ConfigMetadata >		ConfigMetadataPtr;

	typedef Poco::SharedPtr< Data >					DataPtr;

	typedef Poco::SharedPtr< ServiceReference >		ServiceRefPtr;

	typedef Poco::SharedPtr< IContainer >			ContainerPtr;
	typedef Poco::SharedPtr< ServiceContainer >		ServiceContainerPtr;
	typedef Poco::SharedPtr< GroupContainer >		GroupContainerPtr;
	typedef Poco::SharedPtr< OutputContainer >		OutputContainerPtr;

	typedef std::pair< ServiceName, ServicePtr >	NamedServicePtr;

}