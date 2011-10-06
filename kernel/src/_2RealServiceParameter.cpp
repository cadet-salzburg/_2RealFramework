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

#include "_2RealServiceParameter.h"
#include "_2RealException.h"
#include "_2RealServiceContainer.h"

namespace _2Real
{

	ServiceParameter::ServiceParameter(Id *const _id, ServiceContainer *const _service, std::string const& _type) :
		SetupParameter(_id, _type),
		m_Service(_service)
	{
	}

	ServiceParameter::ServiceParameter(ServiceParameter const& _src) : SetupParameter(_src)
	{
		throw Exception("attempted to copy entity");
	}

	ServiceParameter& ServiceParameter::operator=(ServiceParameter const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	ServiceParameter::~ServiceParameter()
	{
	}

	ServiceContainer *const ServiceParameter::service()
	{
		return m_Service;
	}

	ServiceContainer *const ServiceParameter::service() const
	{
		return m_Service;
	}

}