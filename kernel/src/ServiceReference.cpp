#include "ServiceReference.h"
#include "IService.h"
#include "Plugin.h"

namespace _2Real
{
	ServiceReference::ServiceReference(const std::string& name, const ServiceProperties& properties, IService* service, Plugin* plugin) :
										m_ServiceName(name), m_Properties(properties), m_Service(service), m_Plugin(plugin)
	{
	}

	ServiceReference::~ServiceReference()
	{
		m_Service = NULL;
		m_Plugin = NULL;
	}

	std::string& ServiceReference::getServiceName()
	{
		return m_ServiceName;
	}

	ServiceProperties& ServiceReference::getServiceProperties()
	{
		return m_Properties;
	}

	IService* ServiceReference::getService()
	{
		return m_Service;
	}
}