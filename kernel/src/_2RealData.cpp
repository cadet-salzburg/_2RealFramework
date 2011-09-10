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

#pragma once

#include "_2RealData.h"
#include "_2RealException.h"
#include "_2RealDataImpl.h"

namespace _2Real
{

	Data::Data(DataPacket *const _impl) :
		m_Impl(_impl)
	{
	}

	Data::Data(Data const& _src) : m_Impl(NULL)
	{
		if (_src.m_Impl != NULL)
		{
			*m_Impl = *_src.m_Impl;
		}
	}

	Data& Data::operator=(Data const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		if (m_Impl)
		{
			delete m_Impl;
		}

		if (_src.m_Impl != NULL)
		{
			*m_Impl = *_src.m_Impl;
		}

		return *this;
	}

	Data::~Data()
	{
		delete m_Impl;
	}

	Poco::Any Data::get(unsigned int const& _id)
	{
		return m_Impl->getAny(_id);
	}

}