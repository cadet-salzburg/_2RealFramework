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

#include "_2RealMetaData.h"

namespace _2Real
{
	_2RealMetaData::_2RealMetaData()
	{
	}

	_2RealMetaData::_2RealMetaData(const _2RealMetaData& _src)
	{
		m_Data = _src.m_Data;
	}

	_2RealMetaData& _2RealMetaData::operator= (const _2RealMetaData& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		m_Data = _src.m_Data;
 
		return *this;
	}

	void _2RealMetaData::set(std::string _key, std::string _value)
	{
		m_Data.insert(std::make_pair<std::string, std::string>(_key, _value));
	}

	const std::string _2RealMetaData::get(std::string _key)
	{
		std::map<std::string, std::string>::iterator it = m_Data.find(_key);
		if (it != m_Data.end())
		{
			return it->second;
		}

		return std::string();
	}
}