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

#include "_2RealOutputSlot.h"
#include "_2RealInputSlot.h"
#include "_2RealException.h"
#include "_2RealAbstractRef.h"
#include "_2RealServiceContainer.h"

namespace _2Real
{

	OutputSlot::OutputSlot(Id *const _id, ServiceContainer *const _service, std::string const& _type) :
		IOSlot(_id, _service, _type)
	{
	}

	OutputSlot::OutputSlot(OutputSlot const& _src) : IOSlot(_src)
	{
		throw Exception("attempted to copy entity");
	}

	OutputSlot& OutputSlot::operator=(OutputSlot const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	OutputSlot::~OutputSlot()
	{
	}

	InputSlot *const OutputSlot::linkedInput()
	{
		return static_cast< InputSlot * >(m_Linked);
	}

	void OutputSlot::linkWith(InputSlot *const _input)
	{
		try
		{
			if (!_input)
			{
				throw Exception("internal error: IO slot linkage failed - null pointer");
			}
			else if (_input == m_Linked)
			{
				return;
			}
			else if (_input->datatype() != m_Typename)
			{
				throw Exception("internal error: IO slot linkage failed - datatype mismatch");
			}

			if (m_Linked)
			{
				//this will call input's reset as well
				reset();
			}

			_input->service()->listenTo(m_Service);
			m_Service->addListener(_input->service());
			_input->linkWith(this);

			m_Linked = _input;
		}
		catch (Exception &e)
		{
			m_Linked = NULL;
			throw e;
		}
	}

	OutputSlot::NamedAny OutputSlot::getAsAny()
	{
		if (!m_Ref)
		{
			throw Exception("output slot value could not be retrieved - slot is uninitialized");
		}

		return IOSlot::NamedAny(id(), m_Ref->getAny());
	}

	void OutputSlot::reset()
	{
		try
		{
			if (m_Linked)
			{
				InputSlot *input = static_cast< InputSlot * >(m_Linked);
				input->service()->stopListeningTo(m_Service);
				m_Service->removeListener(input->service());
				input->reset();
				m_Linked = NULL;
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}