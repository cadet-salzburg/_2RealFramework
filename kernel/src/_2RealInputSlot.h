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

#include "_2RealParameter.h"
#include "_2RealIOutputListener.h"
#include "_2RealEngineData.h"

#include "Poco/Mutex.h"

#include <map>

namespace _2Real
{

	class OutputSlot;
	class Data;
	class ParameterMetadata;

	typedef std::pair< long, EngineData >	TimestampedData;
	typedef std::map< long, EngineData >	DataTable;

	class InputSlot : public Parameter, public IOutputListener
	{

	public:

		InputSlot(ParameterMetadata const& metadata);

		void reset();
		void linkWith(OutputSlot &output);
		bool isLinked() const;

		bool updateCurrent();
		void clearCurrent();
		void receiveData(Data &data);

		const TimestampedData getNewest() const;
		const TimestampedData getOldest() const;
		const TimestampedData getData() const;

	private:

		mutable Poco::FastMutex		m_Mutex;
		DataTable					m_ReceivedTable;
		DataTable					m_CurrentTable;
		OutputSlot					*m_Output;

	};

	inline bool InputSlot::isLinked() const
	{
		return (m_Output != NULL);
	}

}