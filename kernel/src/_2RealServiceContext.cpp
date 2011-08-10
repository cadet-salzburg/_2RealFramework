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

#include "_2RealInternalService.h"

namespace _2Real
{

	ServiceContext::~ServiceContext()
	{
		m_ServicePtr = NULL;
	}

	ServiceContext::ServiceContext(InternalService *const _service) : m_ServicePtr(_service)
	{
	}

	ServiceContext::ServiceContext(const ServiceContext &_src)
	{
		m_ServicePtr = _src.m_ServicePtr;
	}

	ServiceContext& ServiceContext::operator=(const ServiceContext &_src)
	{
		if (this == &_src)
		{
			return *this;
		}
 
		m_ServicePtr= _src.m_ServicePtr;
	 
		return *this;
	}

	const bool ServiceContext::getSetupParameter(AbstractServiceVariable *_param)
	{
		return m_ServicePtr->getSetupParameter(_param);
	}

	const bool ServiceContext::registerInputVariable(AbstractServiceVariable *_var)
	{
		m_ServicePtr->addInputVariable(_var);
		return true;
	}

	const bool ServiceContext::registerOutputVariable(AbstractServiceVariable *_var)
	{
		m_ServicePtr->addOutputVariable(_var);
		return true;
	}

}