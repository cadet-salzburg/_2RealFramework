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
#include "_2RealEngine.h"

#include "Poco/Delegate.h"

namespace _2Real
{

	OutputSlot::OutputSlot(Identifier const& id, Service &service, std::string const& type, std::string const& keyword, EngineData initialData) :
		IOSlot(id, service, type, keyword),
		m_Timer(Engine::instance().timer()),
		m_WriteData(initialData)
	{
		m_CurrentData = Data(m_WriteData, m_Timer.getTimestamp());
		m_WriteData.clone(m_WriteData);
	}

	void OutputSlot::update()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		//store data - in case a new listener is added somewhere in between
		m_CurrentData = Data(m_WriteData, m_Timer.getTimestamp());
		m_Event.notifyAsync(this, m_CurrentData);
		m_WriteData.clone(m_WriteData);
	}

	void OutputSlot::addListener(OutputListener &listener)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_Event += Poco::delegate(&listener, &OutputListener::receiveData);
		listener.receiveData(m_CurrentData);
	}

	void OutputSlot::removeListener(OutputListener &listener)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_Event -= Poco::delegate(&listener, &OutputListener::receiveData);
	}

	void OutputSlot::registerCallback(DataCallback callback)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_Event += Poco::delegate(callback);
		callback(m_CurrentData);
	}

	void OutputSlot::unregisterCallback(DataCallback callback)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_Event -= Poco::delegate(callback);
	}
}