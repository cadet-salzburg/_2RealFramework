#pragma once

#include <string>

#include "Common.h"

/*
	reference to a service
	that is registered in the framework
*/


namespace _2Real
{
	class ServiceReference
	{
	public:

		~ServiceReference();

		std::string& getServiceName();
		ServiceProperties& getServiceProperties();
		IService* getService();

	private:

		ServiceReference(const std::string& name, const ServiceProperties& properties, IService* service, Plugin* plugin);

		std::string				m_ServiceName;	//name under which the service is registered
		IService*				m_Service;		//pointer to the service
		ServiceProperties		m_Properties;	//the services' properties
		Plugin*					m_Plugin;		//the plugin that offers the service

		friend class PluginFrameworkContext;
	};
}