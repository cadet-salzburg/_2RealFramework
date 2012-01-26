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
#include "_2RealData.h"

#include "Poco/Mutex.h"
#include "Poco/BasicEvent.h"

namespace _2Real
{

	class IOutputListener;
	class Timer;

	typedef void (*DataCallback)(Data &data);

	class OutputSlot : public Parameter
	{

	public:

		OutputSlot(ParameterMetadata const& metadata);

		EngineData					getData();
		void						update();

		void						registerCallback(DataCallback callback);
		void						unregisterCallback(DataCallback callback);
		void						addListener(IOutputListener &listener);
		void						removeListener(IOutputListener &listener);

	private:

		Timer						const& m_Timer;
		mutable Poco::FastMutex		m_Mutex;
		Data						m_CurrentData;
		EngineData					m_WriteData;
		Poco::BasicEvent< Data >	m_Event;

	};

	inline EngineData OutputSlot::getData()
	{
		return m_WriteData;
	}

}