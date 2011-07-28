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
#include "_2RealIPluginActivator.h"

namespace _2Real
{
	class AbstractPluginActivator : public IPluginActivator
	{

	public:

		MetadataPtr metadata();

		virtual void start(PluginContextPtr _context) = 0;									//needs to be implemented by actual plugin
		virtual void stop(PluginContextPtr _context) = 0;									//this one as well

		void handleServiceNotification(const ServiceNotificationPtr& _notification) {}		//those will be implemented in time
		void handlePluginNotification(const PluginNotificationPtr& _notification) {}
		void handleFrameworkNotification(const FrameworkNotificationPtr& _notification) {}
	};
}