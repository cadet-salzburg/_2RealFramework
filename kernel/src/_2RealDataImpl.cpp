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

#include "_2RealDataImpl.h"
#include "_2RealException.h"

#include <iostream>

namespace _2Real
{

	DataPacket::DataPacket()
	{
	}

	DataPacket::DataPacket(DataPacket const& _src)
	{
		m_Data = _src.m_Data;
	}

	DataPacket& DataPacket::operator=(DataPacket const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		m_Data.clear();
		m_Data = _src.m_Data;

		return *this;
	}

	DataPacket::~DataPacket()
	{
	}

	void DataPacket::remove(unsigned int const& _id)
	{
		AnyMap::iterator it = m_Data.find(_id);
		
		if (it != m_Data.end())
		{
			m_Data.erase(it);
		}
	}

	const bool DataPacket::contains(unsigned int const& _id) const
	{
		AnyMap::const_iterator it = m_Data.find(_id);
		
		if (it == m_Data.end())
		{
			return false;
		}

		return true;
	}

	DataPacket::SharedAny DataPacket::getAny(unsigned int const& _id) const
	{
		AnyMap::const_iterator it = m_Data.find(_id);
			
		if (it != m_Data.end())
		{
			return it->second;
		}
		return SharedAny();
	}

	void DataPacket::insertAny(unsigned int const& _id, DataPacket::SharedAny &_any)
	{
		m_Data.insert(NamedAny(_id, _any));
	}

	void DataPacket::merge(DataPacket const& _src)
	{
		m_Data.insert(_src.m_Data.begin(), _src.m_Data.end());
	}

	const unsigned int DataPacket::size() const
	{
		return m_Data.size();
	}

}