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

#include "_2RealContext.h"
#include "_2RealFramework.h"
#include "_2RealConfiguration.h"

namespace _2Real
{
	Context* Context::s_ContextPtr;
	Poco::Mutex Context::s_Mutex;

	Context* const Context::instance()
	{
		Poco::ScopedLock< Poco::Mutex > lock(s_Mutex);

		if(s_ContextPtr == NULL)
		{
			s_ContextPtr = new Context();
		}

		return s_ContextPtr;
	}

	Context::Context()
	{
		m_FrameworkPtr = new Framework();
	}

	Context::Context(Context const& _src) : m_FrameworkPtr(_src.m_FrameworkPtr)
	{
	}
	
	Context& Context::operator=(Context const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}
 
		m_FrameworkPtr = _src.m_FrameworkPtr; 
	 
		return *this;
	}

	Context::~Context()
	{
		delete m_FrameworkPtr;
	}

	Configuration *const Context::createConfiguration()
	{
		return m_FrameworkPtr->createConfiguration();
	}
}