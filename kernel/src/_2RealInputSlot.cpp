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

	InputSlot::InputSlot(Id *const _id, Service *const _service, std::string const& _type, std::string const& _key) :
		IOSlot(_id, _service, _type, _key)
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
		m_CurrentTable.clear();
	}

	void InputSlot::receiveData(Data &data)
	{
		Poco::FastMutex::ScopedLock lock(m_Mutex);
		m_ReceivedTable.insert(TimestampedData(data.timestamp(), data.data()));
	}

	const TimestampedData InputSlot::getNewest() const
	{
		if (m_CurrentTable.empty())
		{
			throw Exception("internal exception: input slot " + name() + " has no data.");
		}

		DataTable::const_iterator newest = m_CurrentTable.begin();

		return *newest;
	}

	const TimestampedData InputSlot::getOldest() const
	{
		if (m_CurrentTable.empty())
		{
			throw Exception("internal exception: input slot " + name() + " has no data.");
		}

		DataTable::const_iterator oldest = m_CurrentTable.end();
		oldest--;

		return *oldest;
	}

	//void InputSlot::linkWith(OutputSlot *const _output)
	//{
	//	try
	//	{
	//		if (!_output)
	//		{
	//			throw Exception("internal error: IO slot linkage failed - null pointer");
	//		}

	//		m_Linked = _output;
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void InputSlot::reset()
	//{
	//	try
	//	{
	//		m_Linked = NULL;
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

}