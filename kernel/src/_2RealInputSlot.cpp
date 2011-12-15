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
#include "_2RealData.h"
#include "_2RealParameterMetadata.h"

namespace _2Real
{

	InputSlot::InputSlot(ParameterMetadata const& metadata) :
		Parameter(metadata),
		m_Output(NULL)
	{
		if (metadata.hasDefaultValue())
		{
			m_ReceivedTable.insert(TimestampedData(0, metadata.getDefaultValue()));
		}
	}

	bool InputSlot::updateCurrent()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (m_ReceivedTable.size() > 0)
		{
			m_CurrentTable = m_ReceivedTable;
			DataTable::const_iterator it = m_CurrentTable.end();
			it--;
			m_ReceivedTable.clear();
			m_ReceivedTable.insert(*it);
			return true;
		}

		return false;
	}

	void InputSlot::setData(TimestampedData const& data)
	{
		resetLink();

		m_ReceivedTable.clear();
		m_ReceivedTable.insert(TimestampedData(data.first, data.second));
	}

	void InputSlot::clearCurrent()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_CurrentTable.clear();
	}

	void InputSlot::receiveData(Data &data)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_ReceivedTable.insert(TimestampedData(data.getTimestamp(), data.data()));
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
			throw Exception("internal exception: input slot " + m_Name + " has no data.");
		}

		DataTable::const_iterator newest = m_CurrentTable.begin();

		return *newest;
	}

	const TimestampedData InputSlot::getOldest() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (m_CurrentTable.empty())
		{
			throw Exception("internal exception: input slot " + getName() + " has no data.");
		}

		DataTable::const_iterator oldest = m_CurrentTable.end();
		oldest--;

		return *oldest;
	}

	void InputSlot::resetLink()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		
		if (isLinked())
		{
			m_Output->removeListener(*this);
		}

		m_Output = NULL;
	}

	void InputSlot::linkWith(OutputSlot &output)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		
		if (isLinked())
		{
			m_Output->removeListener(*this);
		}

		m_Output = &output;
		m_Output->addListener(*this);
	}

}