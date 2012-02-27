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

	class DataBuffer : public std::map< long, EngineData >
	{

	public:

		DataBuffer(const unsigned int maxSize);

		const unsigned int getMaxSize() const;
		const bool isFull() const;

	private:

		unsigned int	const m_MaxSize;

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

	class BufferPolicy
	{

	public:

		virtual ~BufferPolicy();
		virtual const bool insertData(TimestampedData const& data, DataBuffer &buffer) = 0;

	};

	inline BufferPolicy::~BufferPolicy()
	{
	}

	//yeah, even i know that's a stupid name for a policy
	class NoInsertOnMaxSize : public BufferPolicy
	{

	public:

		virtual const bool insertData(TimestampedData const& data, DataBuffer &buffer)
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

	};

	class InputSlot : public Parameter, public IOutputListener
	{

	public:

		InputSlot(ParameterMetadata const& metadata, BufferPolicy &policy, const unsigned int bufferSize);
		~InputSlot();

		/*
		*	inserts the value into the data
		*/
		void setData(TimestampedData const& data);

		/*
		*	inserts the value into the data
		*/
		void insertData(TimestampedData const& data);

		/**
		*	deletes all links
		*/
		void resetLinks();

		/**
		*	links this slot w. an output slot
		*/
		void linkWith(OutputSlot &output);

		/**
		*	breaks link w. output, does nothing if no such link exists
		*/
		void breakLink(OutputSlot &output);

		/*
		*	true if there is any link w. an output slot
		*/
		const bool isLinked() const;

		/**
		*	swaps current & received data
		*/
		const bool updateCurrent();

		/**
		*	kills current data
		*/
		void clearCurrent();

		/**
		*	kills received data
		*/
		void clearReceived();

		/**
		*	inserts new data into received
		*/
		void receiveData(Data &data);

		/*
		*	lots of functions... currently only getData is used, and it returns the newest
		*/
		const TimestampedData getNewest() const;
		const TimestampedData getOldest() const;
		const TimestampedData getData() const;

	private:

		mutable Poco::FastMutex		m_Mutex;
		DataBuffer					*m_ReceivedTable;
		DataBuffer					*m_CurrentTable;
		std::list< OutputSlot * >	m_Outputs;
		BufferPolicy				&m_Policy;

	};

}