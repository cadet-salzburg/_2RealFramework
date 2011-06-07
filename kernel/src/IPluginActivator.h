#pragma once

#include "Common.h"

/*
	every plugin has to implement this interface
*/

namespace _2Real
{
	class IPluginActivator
	{
	public:
		virtual ~IPluginActivator() {}

		virtual void start(PluginContext* context) = 0;
		virtual void stop(PluginContext* context) = 0;
	};
}