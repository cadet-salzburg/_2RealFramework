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

namespace _2Real
{

	ServiceContext::ServiceContext(Service *const _service) :
		m_Service(_service)
	{
	}

	ServiceContext::ServiceContext(ServiceContext const& _src) :
		m_Service(_src.m_Service)
	{
	}

	ServiceContext& ServiceContext::operator=(ServiceContext const& _src)
	{
		return *this;
	}

	ServiceContext::~ServiceContext()
	{
	}

	SharedAny ServiceContext::getSetupParameter(std::string const& _name)
	{
		return m_Service->getParameterValue(_name);
	}

	InputHandle ServiceContext::getInputHandle(std::string const& _name)
	{
		return m_Service->createInputHandle(_name);
	}

	OutputHandle ServiceContext::getOutputHandle(std::string const& _name)
	{
		return m_Service->createOutputHandle(_name);
	}

}