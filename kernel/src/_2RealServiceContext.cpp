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

#include "_2RealServiceContext.h"
#include "_2RealServiceContainer.h"

namespace _2Real
{

	ServiceContext::~ServiceContext()
	{
	}

	ServiceContext::ServiceContext(ServiceContainer *const _container) : m_ContainerPtr(_container)
	{
	}

	ServiceContext::ServiceContext(ServiceContext const& _src)
	{
#ifdef _DEBUG
		std::cout << "service context copy constructor called" << std::endl;
#endif

		//does nothing

	}

	ServiceContext& ServiceContext::operator=(ServiceContext const& _src)
	{
#ifdef _DEBUG
		std::cout << "service context assignment operator called" << std::endl;
#endif

		//does nothing
		return *this;

	}

	void ServiceContext::getSetupParameter(AbstractValue *_param)
	{
		m_ContainerPtr->getParameterValue(_param);
	}

	void ServiceContext::registerInputVariable(AbstractValue *_var)
	{
		m_ContainerPtr->registerInputVariable(_var);
	}

	void ServiceContext::registerOutputVariable(AbstractValue *_var)
	{
		m_ContainerPtr->registerOutputVariable(_var);
	}

}