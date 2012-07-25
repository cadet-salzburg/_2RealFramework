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

#include "_2RealComparisons.h"

#include <string>

#ifdef _UNIX
    #include <sstream>
#endif

namespace _2Real
{
	class AbstractAnyHolder
	{

	public:

		virtual ~AbstractAnyHolder() {}
		virtual const std::string getTypename() const = 0;
		virtual std::type_info const& getTypeinfo() const = 0;
		virtual AbstractAnyHolder* clone() const = 0;
		virtual AbstractAnyHolder* create() const = 0;
		virtual void writeTo( std::ostream &out ) const = 0;
		virtual void readFrom( std::istream &in ) = 0;
		virtual bool isEqualTo( AbstractAnyHolder const& other ) const = 0;
		virtual bool isLessThan( AbstractAnyHolder const& other ) const = 0;

	};

	template< typename TData >
	class AnyHolder : public AbstractAnyHolder
	{

	public:

		AnyHolder();
		explicit AnyHolder( TData const& value );

		const std::string getTypename() const;
		std::type_info const& getTypeinfo() const;
		AbstractAnyHolder * create() const;
		AbstractAnyHolder * clone() const;

		void writeTo( std::ostream &out ) const;
		void readFrom( std::istream &in );

		bool isEqualTo( AbstractAnyHolder const& other ) const;
		bool isLessThan( AbstractAnyHolder const& other ) const;

		TData		m_Data;

	private:

		AnyHolder( AnyHolder< TData > const& src );
		AnyHolder& operator=( AnyHolder< TData > const& src );

	};

	template< typename TData >
	AnyHolder< TData >::AnyHolder() :
		m_Data()
	{
	}

	template< typename TData >
	AnyHolder< TData >::AnyHolder( TData const& value ) :
		m_Data( value )
	{
	}

	template< typename TData >
	AnyHolder< TData >::AnyHolder( AnyHolder< TData > const& src ) :
		m_Data( src.m_Data )
	{
	}

	template< typename TData >
	AnyHolder< TData >& AnyHolder< TData >::operator=( AnyHolder< TData > const& src )
	{
		if ( this == &src )
		{
			return *this;
		}

		m_Data = src.m_Data;

		return *this;
	}

	template< typename TData >
	std::type_info const& AnyHolder< TData >::getTypeinfo() const
	{
		return typeid( TData );
	}

	template< typename TData >
	const std::string AnyHolder< TData >::getTypename() const
	{
		return typeid( TData ).name();
	}

	template< typename TData >
	bool AnyHolder< TData >::isEqualTo( AbstractAnyHolder const& other ) const
	{
		if ( other.getTypename() == this->getTypename() )
		{
			AnyHolder< TData > const& holder = dynamic_cast< AnyHolder< TData > const& >( other );
			return isEqual( m_Data, holder.m_Data );
		}
		else
		{
			std::ostringstream msg;
			msg << "type of data " << other.getTypename() << " does not match type " << this->getTypename() << std::endl;
			throw TypeMismatchException( msg.str() );
		}
	}

	template< typename TData >
	bool AnyHolder< TData >::isLessThan( AbstractAnyHolder const& other ) const
	{
		if ( other.getTypename() == this->getTypename() )
		{
			AnyHolder< TData > const& holder = dynamic_cast< AnyHolder< TData > const& >( other );
			return isLess( m_Data, holder.m_Data );
		}
		else
		{
			std::ostringstream msg;
			msg << "type of data " << other.getTypename() << " does not match type " << this->getTypename() << std::endl;
			throw TypeMismatchException( msg.str() );
		}
	}

	template< typename TData >
	void AnyHolder< TData >::writeTo( std::ostream &out ) const
	{
		out << m_Data;
	}

	template< typename TData >
	void AnyHolder< TData >::readFrom( std::istream &in )
	{
		in >> m_Data;
	}

	template< typename TData >
	AbstractAnyHolder * AnyHolder< TData >::create() const
	{
		return new AnyHolder< TData >();
	}

	template< typename TData >
	AbstractAnyHolder * AnyHolder< TData >::clone() const
	{
		return new AnyHolder< TData >( m_Data );
	}

}
