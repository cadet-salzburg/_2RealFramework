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

namespace _2Real
{
	
	template< typename T >
	Data< T >::Data() 
	{
	}

	template< typename T >
	Data< T >::Data(Data< T > const& _src)
	{
		m_DataMap = _src.m_DataMap;
	}

	template< typename T >
	Data< T >& Data< T >::operator=(Data< T > const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}
 
		m_DataMap = _src.m_DataMap;
	 
		return *this;
	}

	template< typename T >
	Data< T >::~Data()
	{
	}

	template< typename T >
	const bool Data< T >::remove(T const& _name)
	{
		NamedDataMap::iterator it = m_DataMap.find(_name);
		
		if (it == m_DataMap.end())
		{
			return false;
		}

		m_DataMap.erase(it);
		return true;
	}

	template< typename T >
	const bool Data< T >::contains(T const& _name) const
	{
		NamedDataMap::iterator it = m_DataMap.find(_name);
		
		if (it == m_DataMap.end())
		{
			return false;
		}

		return true;
	}

	template< typename T >
	SharedAnyPtr Data< T >::getAny(T const& _name) const
	{
		NamedDataMap::const_iterator it = m_DataMap.find(_name);
			
		if (it != m_DataMap.end())
		{
			return it->second;
		}

		return AnyPtr();
	}

	template< typename T >
	void Data< T >::insertAny(T const& _name, SharedAnyPtr &_anyPtr)
	{
		m_DataMap.insert(NamedData(_name, _anyPtr));
	}

}