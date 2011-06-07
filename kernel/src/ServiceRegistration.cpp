#include "ServiceRegistration.h"
#include "PluginFrameworkContext.h"
#include "IService.h"

namespace _2Real
{
	ServiceRegistration::ServiceRegistration(const std::string name, PluginFrameworkContext* fwContext, IService* service) 
											: m_FrameworkContext(fwContext), m_Service(service), m_Name(name)
	{
	}

	ServiceRegistration::~ServiceRegistration()
	{
		m_Service = NULL;
		m_FrameworkContext = NULL;
	}

	void ServiceRegistration::unregister()
	{
		m_FrameworkContext->unregisterService(m_Name, m_Service);
	}
}