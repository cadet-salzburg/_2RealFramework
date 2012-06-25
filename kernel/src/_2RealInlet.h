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

#include "Poco/Mutex.h"
#include "Poco/BasicEvent.h"

#include <set>
#include <list>

namespace _2Real
{

	class Outlet;
	class ParameterData;
	class AbstractStateManager;
	class BufferPolicy;

	// the newer the data is, the higher the timestamp
	typedef std::multiset< TimestampedData, std::greater< TimestampedData > >		DataBuffer;

	class Inlet : public Parameter
	{

	public:

		Inlet( std::string const& name, std::string const& longTypename, std::string const& type, EngineData const& defaultValue );
		~Inlet();

		using Parameter::getTypename;
		using Parameter::getLongTypename;
		using Parameter::getName;

		void setToValue( TimestampedData const& data );
		void receiveData( TimestampedData &data ); 

		void clearLinks();
		void linkWith(Outlet &output);
		void breakLink(Outlet &output);
		void unlink(Outlet &output);

		void updateCurrentValue();						// called right before update
		EngineData const& getCurrentValue() const;		//
		void resetData();

		void registerToDataReceived( AbstractStateManager &triggers );
		void unregisterFromDataReceived( AbstractStateManager &triggers );

	private:

		mutable Poco::FastMutex		m_DataAccess;
		mutable Poco::FastMutex		m_OutletsAccess;

		DataBuffer					m_ReceivedDataItems;

		TimestampedData				m_DefaultValue;
		TimestampedData				m_CurrentValue;

		std::list< Outlet * >		m_LinkedOutlets;

		BufferPolicy				*m_BufferPolicy;

		Poco::BasicEvent< std::pair< long, long > >		m_DataReceived;

	};

}