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

#include "_2RealOutputListener.h"
#include "_2RealData.h"

namespace _2Real
{
	OutputListener::OutputListener(std::string const& _name, Variable const& _fwName, unsigned long const& _time) :
		m_ListenerName(_name), m_tDataReceived(_time)
	{
	}

	OutputListener::OutputListener(OutputListener const& _src) :
		m_ListenerName(_src.m_ListenerName), m_tDataReceived(_src.m_tDataReceived), m_FrameworkName(_src.m_FrameworkName),
		m_AnyQueue(_src.m_AnyQueue)
	{
	}

	OutputListener& OutputListener::operator=(OutputListener const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}
 
		m_Mutex.lock();
		m_tDataReceived = _src.m_tDataReceived;
		m_ListenerName = _src.m_ListenerName;
		m_FrameworkName = _src.m_FrameworkName;
		m_AnyQueue = _src.m_AnyQueue;
		m_Mutex.unlock();
	 
		return *this;
	}

	OutputListener::~OutputListener()
	{
	}

	void OutputListener::listenerFunction(DataPtr &_data)
	{
		m_Mutex.lock();

		AnyPtr anyPtr = _data->getAny(m_FrameworkName);
		if (anyPtr.isNull())
		{
			m_AnyQueue.push_back(anyPtr);
		}

		//NOTE TO SELF: time
		m_tDataReceived = 0;

		m_Mutex.unlock();
	}

	const bool OutputListener::hasReceivedData(unsigned long const& _compare) const
	{
		if (_compare > m_tDataReceived)
		{
			return false;
		}

		return true;
	}

	AnyPtr OutputListener::getData()
	{
		m_Mutex.lock();

		if (m_AnyQueue.empty())
		{
			m_Mutex.unlock();
			return AnyPtr();
		}

		AnyPtr result = *m_AnyQueue.begin();
		
		m_Mutex.unlock();

		return result;
	}

}