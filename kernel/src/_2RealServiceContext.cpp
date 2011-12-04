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

#include "_2RealServiceContext.h"
#include "_2RealService.h"
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"
#include "_2RealSetupParameter.h"
#include "_2RealException.h"

#include <sstream>

namespace _2Real
{

	ServiceContext::ServiceContext(Service &service) :
		m_Service(service)
	{
	}

	ServiceContext::ServiceContext(ServiceContext const& src) :
		m_Service(src.m_Service)
	{
	}

	ServiceContext& ServiceContext::operator=(ServiceContext const& src)
	{
		return *this;
	}

	EngineData ServiceContext::getSetupParameter(std::string const& name)
	{
		if (m_Service.hasSetupParameter(name))
		{
			return m_Service.getSetupParameter(name).get();
		}
		else
		{
			throw InvalidParameterException("setup parameter", name);
		}
	}

	InputHandle ServiceContext::getInputHandle(std::string const& name)
	{
		if (m_Service.hasInputSlot(name))
		{
			return m_Service.createInputHandle(name);
		}
		else
		{
			throw InvalidParameterException("input slot", name);
		}
	}

	OutputHandle ServiceContext::getOutputHandle(std::string const& name)
	{
		if (m_Service.hasOutputSlot(name))
		{
			return m_Service.createOutputHandle(name);
		}
		else
		{
			throw InvalidParameterException("output slot", name);
		}
	}

}