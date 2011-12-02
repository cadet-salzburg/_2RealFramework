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

#include "Poco/Delegate.h"

namespace _2Real
{

	OutputSlot::OutputSlot(Identifier const& id, Service *const service, std::string const& type, std::string const& key, SharedAny init) :
		IOSlot(id, service, type, key)
	{
		this->init(init);
	}

	OutputSlot::~OutputSlot()
	{
	}

	void OutputSlot::init(SharedAny const& initialData)
	{
		m_WriteData = initialData;
		m_IsInitialized = true;

		update();
	}

	void OutputSlot::update()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		//store data - in case a new listener is added somewhere in between
		m_CurrentData = Data(m_WriteData, Poco::Timestamp());
		m_Event.notifyAsync(this, m_CurrentData);
		m_WriteData.clone(m_WriteData);
	}

	void OutputSlot::addListener(OutputListener &receiver)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_Event += Poco::delegate(&receiver, &OutputListener::receiveData);

		//makes sure the newly added listener receives the current data
		//because, if the service writing the output has a long update function
		//while the service reading it has a short update function, well....
		receiver.receiveData(m_CurrentData);
	}

	void OutputSlot::removeListener(OutputListener &receiver)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_Event -= Poco::delegate(&receiver, &OutputListener::receiveData);
	}

	void OutputSlot::registerCallback(DataCallback callback)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_Event += Poco::delegate(callback);
	}

	void OutputSlot::unregisterCallback(DataCallback callback)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_Event -= Poco::delegate(callback);
	}
}