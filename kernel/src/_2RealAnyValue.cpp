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
	AnyValue< T >::AnyValue(std::string const& _name, T &_value) : AbstractValue(_name), m_Value(_value)
	{
	}

	template< typename T >
	AnyValue< T >::AnyValue(AnyValue< T > const& _src) : AbstractValue(_src), m_Value(_src.m_Value)
	{
#ifdef _DEBUG
		std::cout << "any value copy constructor called" << std::endl;
#endif
	}

	template< typename T >
	AnyValue< T >& AnyValue< T >::operator=(AnyValue< T > const& _src)
	{
#ifdef _DEBUG
		std::cout << "any value copy assignment operator called" << std::endl;
#endif

		return *this;
	}

	template< typename T >
	AnyValue< T >::~AnyValue()
	{
	}

	template< typename T >
	void AnyValue< T >::extract(SharedAnyPtr &_anyPtr) throw(...)
	{
		T copy;
		try
		{
			copy = Poco::AnyCast< T >(*_anyPtr.get());
		}
		catch(Poco::BadCastException e)
		{
			//TODO: exception;
		}
		T = copy;
	}

	template< typename T >
	SharedAnyPtr& AnyValue< T >::sharedPtr()
	{
		T copy(m_Value);
		return SharedAnyPtr(new Poco::Any(copy));
	}

	template< typename T >
	T AnyValue< T >::getCopy()
	{
		T copy(m_Value);
		return T;
	}

}
