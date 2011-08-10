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

#include <map>

#include "Poco/Any.h"

namespace _2Real
{
	class Data
	{

	public:

		typedef std::map<std::string, Poco::Any> DataItems;
		typedef std::pair<std::string, Poco::Any> NamedDataItem;
		
		Data();
		Data(const Data& _src);
		Data& operator= (const Data& _src);
		~Data();

		template<class T>
		void insert(std::string _name, T _data)
		{
			Poco::Any data(_data);
			m_Data.insert(NamedDataItem(_name, data));
		}

		const bool remove(std::string _name)
		{
			DataItems::iterator it = m_Data.find(_name);
		
			if (it == m_Data.end())
			{
				return false;
			}

			std::cout << "found " << _name << std::endl;
			m_Data.erase(it);
			return true;
		}

		template<class T>
		const bool get(std::string _name, T& _value) const
		{
			DataItems::const_iterator it = m_Data.find(_name);
			
			if (it != m_Data.end())
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
				}
			}

			return false;
		}

		const bool getAny(std::string _name, Poco::Any &_any)
		{
			DataItems::const_iterator it = m_Data.find(_name);
			
			if (it != m_Data.end())
			{
				_any = it->second;
				return true;
			}

			return false;
		}

		void insertAny(std::string _name, Poco::Any& _any)
		{
			m_Data.insert(NamedDataItem(_name, _any));
		}

		const DataItems::const_iterator begin() const
		{
			return m_Data.begin();
		}

		const DataItems::const_iterator end() const
		{
			return m_Data.end();
		}

		const unsigned int size() const 
		{ 
			return m_Data.size();
		}

	private:

		DataItems			m_Data;

	};
}