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
#include "_2RealService.h"
#include "_2RealParameterMetadata.h"
#include "_2RealEngineImpl.h"
#include "_2RealTypetable.h"

#include "Poco/Delegate.h"

#include <iostream>

namespace _2Real
{

	OutputSlot::OutputSlot(ParameterMetadata const& metadata, Poco::Timestamp const& timestamp) :
		Parameter(metadata),
		m_SystemTime(timestamp)
	{
		if (metadata.hasDefaultValue())
		{
			m_WriteData = metadata.getDefaultValue();
			m_CurrentData = Data(m_WriteData, (long)m_SystemTime.elapsed());
			m_WriteData.clone(m_WriteData);
		}
		else
		{
			m_WriteData.create(EngineImpl::instance().getTypetable().getInitialValueFromKey(metadata.getKeyword()));
		}
	}

	void OutputSlot::update()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_CurrentData = Data(m_WriteData, (long)m_SystemTime.elapsed());
		m_Event.notify(this, m_CurrentData);
		m_WriteData.clone(m_WriteData);
		m_IsInitialized = true;
	}

	EngineData OutputSlot::getData()
	{
		return m_WriteData;
	}

	void OutputSlot::addListener(IOutputListener &listener)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_Event += Poco::delegate(&listener, &IOutputListener::receiveData);

		if (isInitialized())
		{
			listener.receiveData(m_CurrentData);
		}
	}

	void OutputSlot::removeListener(IOutputListener &listener)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_Event -= Poco::delegate(&listener, &IOutputListener::receiveData);
	}

	void OutputSlot::addListener(InputSlot &inlet)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		std::list< InputSlot * >::iterator it = std::find< std::list< InputSlot * >::iterator, InputSlot * >(m_LinkedInlets.begin(), m_LinkedInlets.end(), &inlet);
		if (it == m_LinkedInlets.end())
		{
			m_Event += Poco::delegate(&inlet, &InputSlot::receiveData);

			if (isInitialized())
			{
				inlet.receiveData(m_CurrentData);
			}
		}
	}

	void OutputSlot::removeListener(InputSlot &inlet)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		std::list< InputSlot * >::iterator it = std::find< std::list< InputSlot * >::iterator, InputSlot * >(m_LinkedInlets.begin(), m_LinkedInlets.end(), &inlet);
		if (it != m_LinkedInlets.end())
		{
			m_Event -= Poco::delegate(&inlet, &InputSlot::receiveData);

			//(*it)->unlink(*this);
		}
	}

	void OutputSlot::registerCallback(DataCallback callback)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_Event += Poco::delegate(callback);

		if (isInitialized())
		{
			callback(m_CurrentData);
		}
	}

	void OutputSlot::unregisterCallback(DataCallback callback)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_Event -= Poco::delegate(callback);
	}

	void OutputSlot::resetLinks()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_Event.clear();

		for(std::list< InputSlot * >::iterator it = m_LinkedInlets.begin(); it != m_LinkedInlets.end(); /**/)
		{
			(*it)->unlink(*this);
			it = m_LinkedInlets.erase(it);
			break;
		}
	}

}