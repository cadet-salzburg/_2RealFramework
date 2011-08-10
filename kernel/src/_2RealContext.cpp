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
#include "_2RealOutputContainer.h"

namespace _2Real
{
	ContextPtr Context::s_ContextPtr;
	Poco::Mutex Context::s_Mutex;

	ContextPtr Context::instance()
	{
		Poco::ScopedLock< Poco::Mutex > lock(s_Mutex);
		if(s_ContextPtr.isNull())
		{
			s_ContextPtr = ContextPtr(new Context());
		}

		return s_ContextPtr;
	}

	Context::Context()
	{
		m_FrameworkPtr = new Framework();
	}

	Context::Context(const Context &_src)
	{
	}
	
	Context& Context::operator=(const Context &_src)
	{
		return *this;
	}

	Context::~Context()
	{
		delete m_FrameworkPtr;
	}

	OutputContainerPtr Context::newConfiguration()
	{
		return m_FrameworkPtr->createOutputContainer();
	}
}