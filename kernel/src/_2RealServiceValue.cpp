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

#include "_2RealServiceValue.h"
#include "_2RealException.h"
#include "_2RealAbstractRef.h"

namespace _2Real
{

	ServiceValue::ServiceValue(Id *const _id, ServiceContainer *const _service, std::string const& _type) :
		ServiceParam(_id, _service, _type)
	{
	}

	ServiceValue::ServiceValue(ServiceValue const& _src) : ServiceParam(_src)
	{
		throw Exception("attempted to copy entity");
	}

	ServiceValue& ServiceValue::operator=(ServiceValue const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	ServiceValue::~ServiceValue()
	{
	}

	void ServiceValue::setValue(Poco::Any const& _any)
	{
		m_bIsInitialized = true;
		m_Value = _any;
	}

	Poco::Any const& ServiceValue::value() const
	{
		if (!m_bIsInitialized)
		{
			throw Exception("ServiceValue::value(), " + name() + " is uninitialized");
		}

		return m_Value;
	}

}