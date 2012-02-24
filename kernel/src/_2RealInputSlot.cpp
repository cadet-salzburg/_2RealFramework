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

#include <iostream>

namespace _2Real
{

	InputSlot::InputSlot(ParameterMetadata const& metadata, BufferPolicy &policy, const unsigned int bufferSize) :
		Parameter(metadata),
		m_Outputs(),
		m_Policy(policy),
		m_FixedValue()
	{
		m_ReceivedTable = new DataBuffer(bufferSize);
		m_CurrentTable = new DataBuffer(bufferSize);

		if (metadata.hasDefaultValue())
		{
			m_ReceivedTable->insert(TimestampedData(0, metadata.getDefaultValue()));
		}
	}

	InputSlot::~InputSlot()
	{
		m_ReceivedTable->clear();
		m_CurrentTable->clear(),
		delete m_ReceivedTable;
		delete m_CurrentTable;
	}

	const bool InputSlot::updateCurrent()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		DataBuffer *tmp;
		tmp = m_CurrentTable;

		m_CurrentTable = m_ReceivedTable;

		tmp->clear();

		m_ReceivedTable = m_CurrentTable;

		/* old behaviour
		if (m_ReceivedTable->size() > 0)
		{
			m_CurrentTable = m_ReceivedTable;
			DataBuffer::const_iterator it = m_CurrentTable->end();
			it--;
			m_ReceivedTable->clear();
			m_ReceivedTable->insert(*it);
			return true;
		}
		*/

		return false;
	}

	void InputSlot::setData(TimestampedData const& data)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		//OPEN Q: really good idea to clear all links?
		//we'll see
		resetLinks();

		m_FixedValue = data;

		//this will stay in the queue until a new value is set OR a link comes
		m_ReceivedTable->clear();
		m_ReceivedTable->insert(data);
	}

	void InputSlot::insertData(TimestampedData const& data)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_ReceivedTable->insert(data);
	}

	void InputSlot::clearCurrent()
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_CurrentTable->clear();
	}

	void InputSlot::receiveData(Data &data)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		m_ReceivedTable->insert(TimestampedData(data.getTimestamp(), data.data()));
	}

	//no update should happen if there is no data at all
	const TimestampedData InputSlot::getData() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (m_CurrentTable->empty())
		{
			throw Exception("internal exception: input slot " + m_Name + " has no data.");
		}

		DataBuffer::const_iterator newest = m_CurrentTable->begin();

		return *newest;
	}

	const TimestampedData InputSlot::getNewest() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (m_CurrentTable->empty())
		{
			throw Exception("internal exception: input slot " + m_Name + " has no data.");
		}

		DataBuffer::const_iterator newest = m_CurrentTable->begin();

		return *newest;
	}

	const TimestampedData InputSlot::getOldest() const
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		if (m_CurrentTable->empty())
		{
			throw Exception("internal exception: input slot " + getName() + " has no data.");
		}

		DataBuffer::const_iterator oldest = m_CurrentTable->end();
		oldest--;

		return *oldest;
	}

	void InputSlot::linkWith(OutputSlot &output)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_Outputs.push_back(&output);
		output.addListener(*this);
	}

	void InputSlot::breakLink(OutputSlot &output)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);

		for (std::list< OutputSlot * >::iterator it = m_Outputs.begin(); it != m_Outputs.end(); ++it)
		{
			if (*it == &output)
			{
				output.removeListener(*this);
				m_Outputs.erase(it);
				break;
			}
		}
	}

	void InputSlot::resetLinks()
	{
		for (std::list< OutputSlot * >::iterator it = m_Outputs.begin(); it != m_Outputs.end(); ++it)
		{
			(*it)->removeListener(*this);
		}

		m_Outputs.clear();
	}

	const bool InputSlot::isLinked() const
	{
		return (m_Outputs.size() > 0);
	}

}