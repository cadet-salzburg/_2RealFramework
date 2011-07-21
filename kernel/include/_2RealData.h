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

	public:
		
		_2RealData();
		_2RealData(const _2RealData& _src);
		_2RealData& operator= (const _2RealData& _src);

		template<class T>
		void insert(std::string _name, T _data)
		{
			Poco::Any data(_data);
			m_Values.insert(Value(_name, data));
		}

		const bool remove(std::string _name)
		{
			Values::iterator it = m_Values.find(_name);
		
			if (it == m_Values.end())
			{
				return false;
			}

			m_Values.erase(it);
			return true;
		};

		template<class T>
		const bool get(std::string _name, T& _value)
		{
			Values::iterator it = m_Values.find(_name);
			
			if (it != m_Values.end())
			{
				try
				{
					T value = Poco::AnyCast<T>(it->second);
					_value = value;
					return true;
				}
				catch (Poco::BadCastException e)
				{
					std::cout << e.what() << std::endl;
					return false;
				}
			}

			return false;
		};

	private:

		typedef std::map<std::string, Poco::Any> Values;
		typedef std::pair<std::string, Poco::Any> Value;

		Values		m_Values;

	};
}