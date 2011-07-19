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

#include "_2RealFramework.h"

namespace _2Real
{
	class _2RealData
	{

		typedef std::map<std::string, Poco::Any> Values;
		typedef std::pair<std::string, Poco::Any> Value;

	public:
		
		_2RealData() 
		{
		};

		_2RealData(const _2RealData& _src)
		{
			m_ValueData = _src.m_ValueData;
		}

		_2RealData& operator= (const _2RealData& _src)
		{
			if (this == &_src)
			{
				return *this;
			}
 
			m_ValueData = _src.m_ValueData;
	 
			return *this;
		}

		template<class T>
		void setData(std::string _name, T _data)
		{
			Poco::Any data(_data);
			m_ValueData.insert(Value(_name, data));
		}

		template<class T>
		const T getData(std::string _name)
		{
			T returnValue;

			Values::iterator it = m_ValueData.find(_name);
			
			if (it != m_ValueData.end())
			{
				try
				{
					T anyValue = Poco::AnyCast<T>(it->second);
					returnValue = anyValue;
				}
				catch (Poco::BadCastException e)
				{
					return returnValue;
				}
			}
			
			return returnValue;
		};

	private:

		//_2RealMetaData		m_MetaData;
		Values					m_ValueData;

	};
}