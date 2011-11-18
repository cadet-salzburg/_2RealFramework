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

#include "_2RealInputHandle.h"
#include "_2RealInputSlot.h"
#include "_2RealSharedAny.h"

namespace _2Real
{

	InputHandle::InputHandle() :
		ParameterHandle(""),
		m_Input(NULL)
	{
	}

	InputHandle::InputHandle(InputSlot *_slot) :
		ParameterHandle(_slot->name()),
		m_Input(_slot)
	{
	}

	InputHandle::InputHandle(InputHandle const& _src) :
		ParameterHandle(_src),
		m_Input(_src.m_Input)
	{
	}

	InputHandle& InputHandle::operator=(InputHandle const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		ParameterHandle::operator=(_src);
		m_Input = _src.m_Input;

		return *this;
	}

	InputHandle::~InputHandle()
	{
		//input slot is deleted by framework
	}

	SharedAny InputHandle::newest()
	{
		return m_Input->getNewest().second;
	}

	SharedAny InputHandle::oldest()
	{
		return m_Input->getOldest().second;
	}

}