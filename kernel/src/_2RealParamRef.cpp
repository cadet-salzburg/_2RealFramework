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

#include "_2RealException.h"

namespace _2Real
{
	
	template< typename T >
	ParamRef< T >::ParamRef(T &_value) :
		AbstractRef(),
		m_Value(_value)
	{
	}

	template< typename T >
	ParamRef< T >::ParamRef(ParamRef< T > const& _src) :
		AbstractRef(_src),
		m_Value(_src.m_Value)
	{
		throw Exception::noCopy();
	}

	template< typename T >
	ParamRef< T >& ParamRef< T >::operator=(ParamRef< T > const& _src)
	{
		throw Exception::noCopy();
	}

	template< typename T >
	ParamRef< T >::~ParamRef()
	{
	}

	template< typename T >
	void ParamRef< T >::extractFrom(AbstractRef::SharedAny _any)
	{
		try
		{
			T copy = Poco::AnyCast< T >(*_any.get());
			m_Value = copy;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	template< typename T >
	void ParamRef< T >::extractFrom(Poco::Any const& _any)
	{
		try
		{
			std::cout << "extracting" << std::endl;
			T copy = Poco::AnyCast< T >(_any);
			m_Value = copy;
		}
		catch (...)
		{
			std::cout << "badcast" << std::endl;
			throw Exception::failure();
		}
	}

	template< typename T >
	AbstractRef::SharedAny ParamRef< T >::getAny()
	{
		T copy(m_Value);
		return SharedAny(new Poco::Any(copy));
	}

}
