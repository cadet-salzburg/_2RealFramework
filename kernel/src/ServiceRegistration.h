#pragma once

#include "Common.h"

/*
	returned by the framework context when a service is registered
	can be used by the plugin activator to unregister a service
*/

namespace _2Real
{
	class ServiceRegistration
	{
	public:

		~ServiceRegistration();
		void unregister();

	private:

		PluginFrameworkContext*		m_FrameworkContext;
		IService*					m_Service;
		std::string					m_Name;

		ServiceRegistration(const std::string name, PluginFrameworkContext* fwContext, IService* service);

		friend class PluginFrameworkContext;
	};
}