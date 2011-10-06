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

#include "_2RealIOSlot.h"
#include "_2RealException.h"
#include "_2RealAbstractRef.h"
#include "_2RealServiceContainer.h"

#include <iostream>

namespace _2Real
{

	IOSlot::IOSlot(Id *const _id, ServiceContainer *const _service, std::string const& _type) :
		Parameter(_id, _type),
		m_Service(_service),
		m_Linked(NULL),
		m_Ref(NULL)
	{
	}

	IOSlot::IOSlot(IOSlot const& _src) : Parameter(_src)
	{
		throw Exception("attempted to copy entity");
	}

	IOSlot& IOSlot::operator=(IOSlot const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	IOSlot::~IOSlot()
	{
		delete m_Ref;
	}

	void IOSlot::setReference(AbstractRef *const _ref)
	{
		m_Ref = _ref;
	}

	const bool IOSlot::isLinked() const
	{
		if (m_Linked)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	ServiceContainer *const IOSlot::service()
	{
		return m_Service;
	}

	ServiceContainer *const IOSlot::service() const
	{
		return m_Service;
	}

}