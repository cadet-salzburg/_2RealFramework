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

#include "engine/_2RealTimestampedData.h"

namespace _2Real
{

	TimestampedData::TimestampedData() : m_Data(), m_Timestamp( 0 ) {}

	TimestampedData::TimestampedData( EngineData const& data, long timestamp ) :
		m_Data( data ),
		m_Timestamp( timestamp )
	{
	}

	bool TimestampedData::operator>( TimestampedData const& other ) const
	{
		return m_Timestamp > other.m_Timestamp;
	}

	long TimestampedData::getTimestamp() const
	{
		return m_Timestamp;
	}

	EngineData const& TimestampedData::getData() const
	{
		return m_Data;
	}

	EngineData & TimestampedData::getData()
	{
		return m_Data;
	}

	bool TimestampedData::isEmpty() const
	{
		return m_Data.isEmpty();
	}

	void TimestampedData::cloneData( TimestampedData const& src )
	{
		m_Data.cloneFrom( src.getData() );
	}

}