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

#include "_2RealIdentifier.h"
#include "_2RealEngineTypedefs.h"
#include "_2RealTypedefs.h"
#include "_2RealData.h"
#include "_2RealException.h"

#include <list>

#include "Poco/BasicEvent.h"

namespace _2Real
{

	class DataPacket;

	class DataQueue
	{

	public:

		DataQueue(Identifier const& _id);

		void registerDataCallback(NewDataCallback _callback);

		void unregisterDataCallback();

		void registerExceptionCallback(ExceptionCallback _callback);

		void unregisterExceptionCallback();

		void sendData(Poco::SharedPtr< DataPacket >& _data);

		void sendException(Exception const& _exception);

		bool const& hasDataListeners() const;

		bool const& hasExceptionListeners() const;

	private:

		/**
		*	sender id
		*/
		Identifier						m_Sender;

		/**
		*
		*/
		Poco::BasicEvent< Data >		m_DataEvent;

		/**
		*
		*/
		Poco::BasicEvent< Identifier >	m_ExceptionEvent;

		/**
		*	
		*/
		bool							m_bHasDataListeners;

		/**
		*
		*/
		bool							m_bHasExceptionListeners;
	};
}