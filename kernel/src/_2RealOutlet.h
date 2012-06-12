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
#include "_2RealTimestampedData.h"
#include "_2RealOutputData.h"
#include "_2RealCallbacks.h"
#include "_2RealCallbacksInternal.h"

#include "Poco/Mutex.h"
#include "Poco/BasicEvent.h"
#include "Poco/Timestamp.h"

#include <map>
#include <list>

namespace _2Real
{

	class Inlet;
	class Timer;

	class Outlet : public Parameter
	{

	public:

		Outlet( ParameterData const& metadata, Poco::Timestamp const& timer );

		EngineData &					getDataForWriting();	//called by output handle to get data
		void							update();
		void							createNewDataItem();
		void							resetLinks();
		void							discardCurrent();		//call by output handle when not writing anything
		const EngineData				getCurrent() const;
		OutputData						getOutputData() const;

		// callback functions
		void							registerCallback( OutletFunctionCallback &callback );
		void							unregisterCallback( OutletFunctionCallback &callback );
		void							registerCallback( AbstractOutletCallbackHandler &callback );
		void							unregisterCallback( AbstractOutletCallbackHandler &callback );

		// inlets
		void							addListener( Inlet &slot );
		void							removeListener( Inlet &slot );
		void							clearLinks();

	private:

		const bool							isLinkedWith(Inlet &inlet) const;

		Poco::Timestamp						const& m_SystemTime;
		mutable Poco::FastMutex				m_Mutex;
		EngineData							m_WriteData;
		Poco::BasicEvent< TimestampedData >	m_InletEvent;
		Poco::BasicEvent< OutputData >		m_CallbackEvent;
		std::list< Inlet * >				m_LinkedInlets;

		//todo: multimap not really necessary, i don't acces elems by key anyway
		std::multimap< long, EngineData >	m_DataItems;
		bool								m_HasData;
		TimestampedData						m_NewestData;

		bool								m_DiscardCurrent;
		

	};

}