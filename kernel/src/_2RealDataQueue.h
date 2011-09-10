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

#include "_2RealIDataQueue.h"
#include "_2RealDataImpl.h"
#include "_2RealIdentifier.h"
#include "_2RealEngineTypedefs.h"
#include "_2RealTypedefs.h"

#include <list>

#include "Poco/BasicEvent.h"

namespace _2Real
{

	class DataQueue : public IDataQueue
	{

	public:

		DataQueue(Identifier const& _id, NewDataCallback _callback);

		void setIDs(std::list< unsigned int > const& _ids) throw(...);

		void addListener(IDataQueue *const _queue) throw(...);

		void removeListener(IDataQueue *const _queue) throw(...);

		void receiveData(NamedData &_data);

		void sendData(bool const& _blocking);

	private:

		Identifier						m_Sender;

		/**
		*	all the slots this queue listens to
		*/
		std::list< unsigned int >		m_IDs;

		/**
		*	all the slots this queue listens to
		*/
		std::list< unsigned int >		m_Received;

		/**
		*	
		*/
		std::list< NamedData >			m_DataList;

		/**
		*
		*/
		Poco::BasicEvent< std::pair< Identifier, Data > >	m_DataEvent;

	};

}