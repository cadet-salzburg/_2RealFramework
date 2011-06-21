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

#include "Common.h"
#include "PluginContext.h"
#include "ServiceNotification.h"
#include "FrameworkNotification.h"
#include "PluginNotification.h"

/*
	every plugin has to implement this interface
*/

namespace _2Real
{
	class IPluginActivator
	{
	public:
		virtual ~IPluginActivator() {}

		virtual void start(PluginContext* context) = 0;													//will be called by the framework when a plugin is started
		virtual void stop(PluginContext* context) = 0;													//will be called by the framework when a plugin is stopped

		virtual void handleServiceNotification(const ServiceNotificationPtr& notification) {};			//override to handle service related events
		virtual void handlePluginNotification(const PluginNotificationPtr& notification) {};			//override to handle plugin related events
		virtual void handleFrameworkNotification(const FrameworkNotificationPtr& notification) {};		//override to handle framework related events
	};
}

#define _2REAL_EXPORT_PLUGIN(x)\
	POCO_BEGIN_MANIFEST(IPluginActivator)\
	POCO_EXPORT_CLASS(x)\
	POCO_END_MANIFEST