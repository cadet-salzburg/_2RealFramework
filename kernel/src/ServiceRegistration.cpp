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

	IService* ServiceRegistration::servicePtr()
	{
		return m_Service;
	}
}