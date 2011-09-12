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

#include "_2RealServiceParam.h"
#include "_2RealException.h"
#include "_2RealAbstractRef.h"

namespace _2Real
{

	ServiceParam::ServiceParam(Id *const _id, ServiceContainer *const _service, std::string const& _type) :
		Entity(_id),
		m_Service(_service),
		m_bIsInitialized(false),
		m_Typename(_type)
	{
	}

	ServiceParam::ServiceParam(ServiceParam const& _src) : Entity(_src)
	{
		throw Exception("attempted to copy entity");
	}

	ServiceParam& ServiceParam::operator=(ServiceParam const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	ServiceParam::~ServiceParam()
	{
	}

	ServiceContainer *const ServiceParam::service()
	{
		return m_Service;
	}

	bool const& ServiceParam::isInitialized() const
	{
		return m_bIsInitialized;
	}

	std::string const& ServiceParam::datatype() const
	{
		return m_Typename;
	}

}