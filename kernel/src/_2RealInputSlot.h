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

#include "_2RealIOSlot.h"
#include "_2RealData.h"

#include "Poco\Mutex.h"

#include <map>

namespace _2Real
{

	/**
	*	represents an input slot of a service
	*/

	class OutputSlot;

	typedef std::pair< Poco::Timestamp, SharedAny >		TimestampedData;

	class InputSlot : public IOSlot
	{

	public:

		InputSlot(Identifier const& id, Service *const service, std::string const& type, std::string const& key);
		~InputSlot();

		OutputSlot *const linkedOutput()
		{
			return m_Output;
		}

		OutputSlot const *const linkedOutput() const
		{
			return m_Output;
		}

		const bool isLinked() const
		{
			if (m_Output)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		//void linkWith(OutputSlot *const output);
		//void reset();

		void updateCurrent();
		void clearCurrent();
		void receiveData(Data &data);

		const TimestampedData getNewest() const;
		const TimestampedData getOldest() const;

	private:

		typedef std::map< Poco::Timestamp, SharedAny >		DataTable;

		Poco::FastMutex				m_Mutex;
		DataTable					m_ReceivedTable;
		DataTable					m_CurrentTable;

		OutputSlot					*m_Output;

	};

}