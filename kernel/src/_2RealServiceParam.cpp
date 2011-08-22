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

	ServiceParam::ServiceParam(IdentifierImpl *const _id, ServiceImpl const *const _service) :
		IEntity(_id),
		m_Service(_service),
		m_Value(NULL),
		m_SenderID(0)
	{
	}

	ServiceParam::ServiceParam(ServiceParam const& _src) : IEntity(_src)
	{
		throw Exception::noCopy();
	}

	ServiceParam& ServiceParam::operator=(ServiceParam const& _src)
	{
		throw Exception::noCopy();
	}

	ServiceParam::~ServiceParam()
	{
		delete m_Value;
	}

	ServiceImpl const *const ServiceParam::service() const
	{
		return m_Service;
	}

	unsigned int const& ServiceParam::senderID() const
	{
		return m_SenderID;
	}

	void ServiceParam::setValue(AbstractRef *const _val)
	{
		m_Value = _val;
	}

	void ServiceParam::listenTo(unsigned int const& _id)
	{
		m_SenderID = _id;
	}

	void ServiceParam::extractFrom(ServiceParam::SharedAny const& _any)
	{
		m_Value->extractFrom(_any);
	}

	ServiceParam::NamedAny ServiceParam::getAny()
	{
		return ServiceParam::NamedAny(id(), m_Value->getAny());
	}

}