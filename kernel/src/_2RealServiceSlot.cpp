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

#include "_2RealServiceSlot.h"
#include "_2RealException.h"
#include "_2RealAbstractRef.h"

namespace _2Real
{

	ServiceSlot::ServiceSlot(IdentifierImpl *const _id, ServiceImpl *const _service) :
		ServiceParam(_id, _service),
		m_Value(NULL),
		m_SenderID(0),
		m_bIsLinked(false)
	{
	}

	ServiceSlot::ServiceSlot(ServiceSlot const& _src) : ServiceParam(_src)
	{
		throw Exception::noCopy();
	}

	ServiceSlot& ServiceSlot::operator=(ServiceSlot const& _src)
	{
		throw Exception::noCopy();
	}

	ServiceSlot::~ServiceSlot()
	{
		delete m_Value;
	}

	unsigned int const& ServiceSlot::senderID() const
	{
		return m_SenderID;
	}

	void ServiceSlot::setValue(AbstractRef *const _val)
	{
		m_bIsInitialized = true;
		m_Value = _val;
	}

	void ServiceSlot::listenTo(unsigned int const& _id)
	{
		m_bIsLinked = true;
		m_SenderID = _id;
	}

	void ServiceSlot::extractFrom(ServiceSlot::SharedAny const& _any)
	{
		m_Value->extractFrom(_any);
	}

	bool const& ServiceSlot::isLinked() const
	{
		return m_bIsLinked;
	}

	ServiceSlot::NamedAny ServiceSlot::getAny()
	{
		if (!m_bIsInitialized)
		{
			throw Exception::failure();
		}

		return ServiceSlot::NamedAny(id(), m_Value->getAny());
	}

}