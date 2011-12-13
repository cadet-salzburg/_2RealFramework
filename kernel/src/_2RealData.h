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

#include "_2RealEngineData.h"

namespace _2Real
{

	class Data
	{

	public:

		Data();
		Data(EngineData const& data, long timestamp);

		template< typename Datatype >
		Datatype const&			getData();
		long					getTimestamp() const;
		EngineData const&		data() const;

	private:

		EngineData				m_Data;
		long					m_Timestamp;

	};

	inline Data::Data() :
		m_Data(),
		m_Timestamp(0)
	{
	}

	inline Data::Data(EngineData const& data, long timestamp) :
		m_Data(data),
		m_Timestamp(timestamp)
	{
	}

	template< typename DataType >
	DataType const& Data::getData()
	{
		Poco::SharedPtr< DataType > ptr = Extract< DataType >(m_Data);
		return *ptr.get();
	}

	inline long Data::getTimestamp() const
	{
		return m_Timestamp;
	}

	inline EngineData const& Data::data() const
	{
		return m_Data;
	}

}