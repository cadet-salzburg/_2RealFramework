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

#include "_2RealFramework.h"
#include "_2RealPluginContext.h"
#include "_2RealServiceNotification.h"
#include "_2RealFrameworkNotification.h"
#include "_2RealPluginNotification.h"
#include "_2RealData.h"

/*
	every plugin has to implement this interface
*/

namespace _2Real
{
	class _2RealIPluginActivator
	{

	public:

		virtual void start(_2RealPluginContextPtr _context) throw(...) = 0;												//will be called by the framework when a plugin is started
		virtual void stop(_2RealPluginContextPtr _context) throw(...) = 0;												//will be called by the framework when a plugin is stopped

		virtual void handleServiceNotification(const _2RealServiceNotificationPtr& notification) = 0;					//handle service related messages sent by the framework
		virtual void handlePluginNotification(const _2RealPluginNotificationPtr& notification) = 0;						//handle plugin related messages sent by the framework
		virtual void handleFrameworkNotification(const _2RealFrameworkNotificationPtr& notification) = 0;				//handle framework related messages sent by the framework

		virtual _2RealServicePtr createService(const std::string& _name, _2RealData& _config) throw(...) = 0;			//register service & return pointer
	};
}

#define _2REAL_EXPORT_PLUGIN(x)\
	POCO_BEGIN_MANIFEST(_2RealIPluginActivator)\
	POCO_EXPORT_CLASS(x)\
	POCO_END_MANIFEST