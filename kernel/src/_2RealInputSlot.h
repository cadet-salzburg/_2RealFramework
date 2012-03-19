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
#include "Poco/BasicEvent.h"

#include <map>

namespace _2Real
{

	class OutputSlot;
	class Data;
	class ParameterMetadata;
	class RunnableTriggers;

	typedef std::pair< long, EngineData >	TimestampedData;

	class DataBuffer : public std::map< long, EngineData, std::greater< long > >
	{

	public:

		DataBuffer(const unsigned int maxSize);

		const unsigned int getMaxSize() const;
		const bool isFull() const;
		void setNewMax(const unsigned int max);

	private:

		unsigned int	m_MaxSize;

	};

	inline DataBuffer::DataBuffer(const unsigned int maxSize) :
		m_MaxSize(maxSize)
	{
	}

	inline const unsigned int DataBuffer::getMaxSize() const
	{
		return m_MaxSize;
	}

	inline const bool DataBuffer::isFull() const
	{
		return (this->size() >= m_MaxSize);
	}

	inline void DataBuffer::setNewMax(const unsigned int max)
	{
		m_MaxSize = max;
	}

	class BufferPolicy
	{

	public:

		virtual ~BufferPolicy();
		virtual const bool insertData(TimestampedData const& data, DataBuffer &buffer) = 0;
		virtual const bool copyData(DataBuffer &dst, DataBuffer &src) = 0;

	};

	inline BufferPolicy::~BufferPolicy()
	{
	}

	//yeah, even i know that's a stupid name for a policy
	class NoInsertOnMaxSize : public BufferPolicy
	{

	public:

		const bool insertData(TimestampedData const& data, DataBuffer &buffer)
		{
			if (buffer.size() < buffer.getMaxSize())
			{
				buffer.insert(data);
				return true;
			}
			else
			{
				return false;
			}
		}

		const bool copyData(DataBuffer &dst, DataBuffer &src)
		{
			for (DataBuffer::iterator it = src.begin(); it != src.end(); )
			{
				if (dst.size() < dst.getMaxSize())
				{
					dst.insert(*it);
					it = src.erase(it);
				}
				else
				{
					return false;
				}
			}

			return true;
		}

	};

	//ugh, this class keeps getting bigger & bigger
	class InputSlot : public Parameter, public IOutputListener
	{

	public:

		InputSlot(ParameterMetadata const& metadata, BufferPolicy &policy, const unsigned int bufferSize);
		~InputSlot();

		/*
		*	inserts the value into the data
		*/
		void setData(TimestampedData const& data);

		/**
		*	deletes all links
		*/
		void resetLinks();

		/**
		*	clears all received data
		*/
		void clearReceived();

		/**
		*	links this slot w. an output slot
		*/
		void linkWith(OutputSlot &output);

		/**
		*	breaks link w. output, does nothing if no such link exists
		*/
		void breakLink(OutputSlot &output);

		/**
		*	breaks link w. output, does nothing if no such link exists
		*/
		void unlink(OutputSlot &output);

		/*
		*	true if there is any link w. an output slot
		*/
		const bool isLinked() const;

		/**
		*	copy received data into access buffer
		*/
		void syncBuffers();

		/**
		*	inserts new data into received
		*/
		void receiveData(Data &data);

		const EngineData getNewest() const;
		
		/**
		*	starting with the newest data item & going backwards, returns the next entry from current table & marks it as consumed
		*	currently, this function is called at most once per update of a service
		*	also, it should only ever be called by an input handle, never by the framework itself! (no synchronization in place)
		*/
		EngineData consumeDataItem();

		/**
		*	deletes the consumed data items
		*/
		void removeConsumedItems();

		/**
		*	true if this inlet has a default value
		*/
		const bool hasDefault() const;

		/**
		*	argh
		*/
		void registerToDataReceived(RunnableTriggers &triggers);
		void unregisterFromDataReceived(RunnableTriggers &triggers);

	private:

		/**
		*	mutex for data update
		*/
		mutable Poco::FastMutex		m_DataMutex;

		/**
		*	mutex for callback registration
		*/
		mutable Poco::FastMutex		m_CallbackMutex;

		/**
		*	mutex for linking
		*/
		mutable Poco::FastMutex		m_OutletsMutex;

		/*
		*	todo: callback
		*/
		bool						m_UseCallback;
		//

		/**
		*	there's 2 buffers: one for accessing data, one for receiving data
		*	the received data is inserted into the access buffer just once before each update
		*	otherwise, i'd have to protect everything from concurrent accesses
		*/
		DataBuffer					m_ReceivedTable;
		DataBuffer					m_CurrentTable;

		/**
		*	all outlets from which this inlet receives data
		*/
		std::list< OutputSlot * >	m_LinkedOutlets;

		/**
		*	this deals with overflow handling inside the buffers
		*/
		BufferPolicy				&m_OverflowPolicy;

		/**
		*	default value - this will be used whenever there's no data available
		*	if an inlet has a default value, the 'data available' condition will always be fulfilled;
		*/
		bool						m_HasDefault;
		TimestampedData				m_DefaultValue;

		/**
		*	nr of data items consumed during the last update
		*/
		unsigned int				m_NrOfConsumed;

		/**
		*	the timestamp of the newest data during the last update
		*/
		long						m_LastTimestamp;

		/**
		*	might also be set to a value
		*/
		bool						m_IsSet;
		TimestampedData				m_SetValue;

		/**
		*	fired when ready
		*/
		Poco::BasicEvent< std::pair< long, long > >		m_DataReceived;

	};

}