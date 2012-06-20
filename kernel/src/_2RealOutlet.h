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

#include <map>
#include <list>

namespace _2Real
{

	class Inlet;
	class Timer;
	class EngineImpl;

	class Outlet : private Parameter
	{

	public:

		Outlet( std::string const& name, std::string const& longTypename, std::string const& typeName, EngineData const& emptyData );

		using Parameter::getTypename;
		using Parameter::getLongTypename;
		using Parameter::getName;

		void									update();					// called by fw at the end of each update cycle
		//void									createNewDataItem();
		void									resetLinks();
		//void									discardCurrent();
		/*void									setCurrentData( EngineData const& value );*/
		EngineData &							getCurrentData();
		const EngineData						getLastData() const;		// returns the value that was sent after the last update cycle
		const OutputData						getLastOutputData() const;

		// application callback functions
		void									registerCallback( OutletFunctionCallback &callback );
		void									unregisterCallback( OutletFunctionCallback &callback );
		void									registerCallback( AbstractOutletCallbackHandler &callback );
		void									unregisterCallback( AbstractOutletCallbackHandler &callback );

		// inlets
		void									addListener( Inlet &slot );
		void									removeListener( Inlet &slot );
		void									clearLinks();

	private:

		typedef std::list< Inlet * >			LinkList;
		typedef std::list< TimestampedData >	DataItemList;

		const bool								isLinkedWith( Inlet &inlet ) const;

		EngineImpl								&m_Engine;					// engine is necessary for timestamps

		DataItemList							m_DataItemsToSend;			// stores all data items that are written during an update cycle

		mutable Poco::FastMutex					m_Access;

		TimestampedData							m_LastDataItem;				// the data item that was last at the end of the most recent update cycle
																			// initially, holds an empty ( created by () ) data item of correct type
		TimestampedData							m_CurrentDataItem;			// the data item that is currently being written
																			// initially, holds an empty ( created by () ) data item of correct type
																			// after the first update, holds a copy of the m_LastDataItem
		bool									m_SendData;					// is flagged if the current data should be sent

		Poco::BasicEvent< TimestampedData >		m_InletEvent;
		LinkList								m_Links;
		Poco::BasicEvent< OutputData >			m_CallbackEvent;			// unlike the linked inlets, callbacks are managed by the io mgr
																			// hmmm, maybe i should move the links there as well....

	};

}