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

#include "_2RealOutputHandle.h"
#include "_2RealOutputSlot.h"
#include "_2RealSharedAny.h"

namespace _2Real
{

	OutputHandle::OutputHandle() :
		ParameterHandle(""),
		m_Output(0)
	{
	}

	OutputHandle::OutputHandle(OutputSlot *_slot) :
		ParameterHandle(_slot->name()),
		m_Output(_slot)
	{
	}

	OutputHandle::OutputHandle(OutputHandle const& _src) :
		ParameterHandle(_src),
		m_Output(_src.m_Output)
	{
	}

	OutputHandle& OutputHandle::operator=(OutputHandle const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		ParameterHandle::operator=(_src);
		m_Output = _src.m_Output;

		return *this;
	}

	OutputHandle::~OutputHandle()
	{
		//output slot is deleted by framework
	}

	SharedAny OutputHandle::data()
	{
		return m_Output->data();
	}

}