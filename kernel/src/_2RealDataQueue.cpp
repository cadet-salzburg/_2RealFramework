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

#include "Poco/Delegate.h"

namespace _2Real
{

	DataQueue::DataQueue(Identifier const& _id, NewDataCallback _callback) :
		m_Sender(_id)
	{
		m_DataEvent += Poco::delegate(_callback);
	}

	void DataQueue::setIDs(std::list< unsigned int > const& _ids)
	{
		m_IDs = _ids;
		m_Received = m_IDs;
		m_DataList.clear();
	}

	void DataQueue::addListener(IDataQueue *const _queue)
	{
		//empty
	}

	void DataQueue::removeListener(IDataQueue *const _queue)
	{
		//empty
	}

	void DataQueue::receiveData(NamedData &_data)
	{
		m_DataList.push_back(_data);

		DataImpl received = *_data.second.get();

		for (std::list< unsigned int >::iterator it = m_Received.begin(); it != m_Received.end(); it++)
		{
			if (received.contains(*it))
			{
				m_Received.erase(it);
			}
		}

		if (m_Received.empty())
		{
			Poco::SharedPtr< DataImpl > packet = Poco::SharedPtr< DataImpl >(new DataImpl());

			for (std::list< NamedData >::iterator it = m_DataList.end(); it != m_DataList.begin(); it--)
			{
				unsigned int sender = it->first;
				DataImpl data = *it->second.get();

				//merge datas
				packet->merge(data);
			}

			std::pair< Identifier, Data > result = std::make_pair(m_Sender, data);

			//send data
			m_DataEvent.notifyAsync(this, result);

			//reset ids
			m_Received = m_IDs;
		}
	}

	void DataQueue::sendData(bool const& _blocking)
	{
		//empty
	}
}