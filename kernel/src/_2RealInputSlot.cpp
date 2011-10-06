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

#include "_2RealInputSlot.h"
#include "_2RealOutputSlot.h"
#include "_2RealException.h"
#include "_2RealAbstractRef.h"
#include "_2RealServiceContainer.h"

namespace _2Real
{

	InputSlot::InputSlot(Id *const _id, ServiceContainer *const _service, std::string const& _type) :
		IOSlot(_id, _service, _type)
	{
	}

	InputSlot::InputSlot(InputSlot const& _src) : IOSlot(_src)
	{
		throw Exception("attempted to copy entity");
	}

	InputSlot& InputSlot::operator=(InputSlot const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	InputSlot::~InputSlot()
	{
	}

	OutputSlot *const InputSlot::linkedOutput()
	{
		return static_cast< OutputSlot * >(m_Linked);
	}

	void InputSlot::extractFrom(IOSlot::SharedAny const& _any)
	{
		m_Ref->extractFrom(_any);
	}

	void InputSlot::linkWith(OutputSlot *const _output)
	{
		try
		{
			if (!_output)
			{
				throw Exception("internal error: IO slot linkage failed - null pointer");
			}

			m_Linked = _output;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void InputSlot::reset()
	{
		try
		{
			m_Linked = NULL;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

}