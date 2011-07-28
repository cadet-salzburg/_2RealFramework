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

#include "_2RealServiceRegistry.h"
#include "_2RealIService.h"
#include "_2RealServiceRegistration.h"

#include "Poco/ThreadPool.h"

namespace _2Real
{
	bool ServiceRegistry::update()
	{
		for (ServiceMap::iterator it = m_Services.begin(); it != m_Services.end(); it++)
		{
			//todo: check metadata to find out if services should actually be threaded
			//also, use different thread pool
			IService* service = it->second->servicePtr().get();
			Poco::ThreadPool::defaultPool().start(*service);
		}
		
		Poco::ThreadPool::defaultPool().joinAll();

		return true;
	}

	void ServiceRegistry::addService(std::string _name, ServicePtr _service)
	{
		ServiceRegPtr reg(new ServiceRegistration(_name, _service));
		m_Services.insert(ServiceItem(_name, reg));
	}
}