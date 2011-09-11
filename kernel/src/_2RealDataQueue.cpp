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

#include "_2RealDataQueue.h"
#include "_2RealData.h"
#include "_2RealDataImpl.h"

#include "Poco/Delegate.h"

namespace _2Real
{

	DataQueue::DataQueue(Identifier const& _id) :
		m_Sender(_id),
		m_bHasDataListeners(false),
		m_bHasExceptionListeners(false)
	{
	}

	void DataQueue::registerDataCallback(NewDataCallback _callback)
	{
		m_bHasDataListeners = true;
		m_DataEvent += Poco::delegate(_callback);
	}

	void DataQueue::registerExceptionCallback(ExceptionCallback _callback)
	{
		m_bHasExceptionListeners = true;
		m_ExceptionEvent += Poco::delegate(_callback);
	}

	void DataQueue::unregisterDataCallback()
	{
		m_bHasDataListeners = false;
		m_DataEvent.clear();
	}

	void DataQueue::unregisterExceptionCallback()
	{
		m_bHasExceptionListeners = false;
		m_ExceptionEvent.clear();
	}

	void DataQueue::sendData(Poco::SharedPtr< DataPacket >& _data)
	{
		Data data(_data, m_Sender);
		m_DataEvent.notifyAsync(this, data);
	}

	void DataQueue::sendException(Exception const& _exception)
	{
		m_ExceptionEvent.notifyAsync(this, m_Sender);
	}

	bool const& DataQueue::hasDataListeners() const
	{
		return m_bHasDataListeners;
	}

	bool const& DataQueue::hasExceptionListeners() const
	{
		return m_bHasExceptionListeners;
	}
}