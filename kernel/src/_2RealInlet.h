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
#include "_2RealEngineData.h"

#include "Poco/Mutex.h"
#include "Poco/BasicEvent.h"

#include <map>
#include <list>

namespace _2Real
{

	class Outlet;
	class TimestampedData;
	class ParameterData;
	class AbstractStateManager;
	class BufferPolicy;

	typedef std::multimap< long, EngineData, std::greater< long > >		DataBuffer;

	class Inlet : private Parameter
	{

	public:

		Inlet( std::string const& name, std::string const& longTypename, std::string const& type, EngineData const& defaultValue );
		~Inlet();

		using Parameter::getTypename;
		using Parameter::getLongTypename;
		using Parameter::getName;

		void setFixedData( TimestampedData const& data );
		void receiveData( TimestampedData &data ); //arg is nonconst bc poco events require exactely this signature

		void clearLinks();
		void linkWith(Outlet &output);
		void breakLink(Outlet &output);
		void unlink(Outlet &output);

		void updateCurrentData();
		EngineData const& getCurrentData() const;
		void resetData();

		void registerToDataReceived( AbstractStateManager &triggers );
		void unregisterFromDataReceived( AbstractStateManager &triggers );

	private:

		mutable Poco::FastMutex		m_DataAccess;
		mutable Poco::FastMutex		m_OutletsAccess;

		DataBuffer					m_ReceivedTable;

		std::pair< long, EngineData >		m_CurrentData;
		std::pair< long, EngineData >		m_DefaultData;
		std::pair< long, EngineData >		m_FixedData;
		bool								m_HasFixedData;

		long								m_LastTimestamp;

		std::list< Outlet * >		m_LinkedOutlets;


		BufferPolicy				*m_BufferPolicy;

		Poco::BasicEvent< std::pair< long, long > >		m_DataReceived;

	};

}