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

#include "_2RealInputSlot.h"
#include "_2RealOutputSlot.h"
#include "_2RealException.h"
#include "_2RealService.h"

namespace _2Real
{

	InputSlot::InputSlot(Identifier const& id, Service *const service, std::string const& type, std::string const& key) :
		IOSlot(id, service, type, key),
		m_Output(NULL)
	{
	}

	InputSlot::~InputSlot()
	{
	}

	void InputSlot::updateCurrent()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_CurrentTable = m_ReceivedTable;
		DataTable::const_iterator it = m_CurrentTable.end();
		it--;
		m_ReceivedTable.clear();
		m_ReceivedTable.insert(*it);
	}

	void InputSlot::clearCurrent()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_CurrentTable.clear();
	}

	void InputSlot::receiveData(Data &data)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_ReceivedTable.insert(TimestampedData(data.timestamp(), data.data()));
	}

	const TimestampedData InputSlot::getData() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		return getNewest();
	}

	const TimestampedData InputSlot::getNewest() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (m_CurrentTable.empty())
		{
			throw Exception("internal exception: input slot " + name() + " has no data.");
		}

		DataTable::const_iterator newest = m_CurrentTable.begin();

		return *newest;
	}

	const TimestampedData InputSlot::getOldest() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (m_CurrentTable.empty())
		{
			throw Exception("internal exception: input slot " + name() + " has no data.");
		}

		DataTable::const_iterator oldest = m_CurrentTable.end();
		oldest--;

		return *oldest;
	}

	void InputSlot::reset()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		
		if (isLinked())
		{
			m_Output->removeListener(*this);
		}

		m_Output = NULL;
	}

	void InputSlot::set(Data const& data)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_ReceivedTable.clear();
		m_ReceivedTable.insert(TimestampedData(data.timestamp(), data.data()));
	}

}