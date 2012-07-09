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

#include <string>
#include <memory>

#include <iostream>

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

	};

	template< typename Datatype >
	class AnyHolder : public AbstractAnyHolder
	{
	
	public:

		AnyHolder();
		explicit AnyHolder( Datatype const& value );

		const std::string getTypename() const;
		std::type_info const& getTypeinfo() const;
		AbstractAnyHolder * create() const;
		AbstractAnyHolder * clone() const;

		void writeTo( std::ostream &out ) const;
		void readFrom( std::istream &in );

		Datatype		m_Data;

	private:

		AnyHolder( AnyHolder< Datatype > const& src );
		AnyHolder& operator=( AnyHolder< Datatype > const& src );

	};

	template< typename Datatype >
	AnyHolder< Datatype >::AnyHolder() :
		m_Data()
	{
	}

	template< typename Datatype >
	AnyHolder< Datatype >::AnyHolder( Datatype const& value ) :
		m_Data( value )
	{
	}

	template< typename Datatype >
	AnyHolder< Datatype >::AnyHolder( AnyHolder< Datatype > const& src ) :
		m_Data( src.m_Data )
	{
	}

	template< typename Datatype >
	AnyHolder< Datatype >& AnyHolder< Datatype >::operator=( AnyHolder< Datatype > const& src )
	{
		if ( this == &src )
		{
			return *this;
		}

		m_Data = src.m_Data;

		return *this;
	}

	template< typename Datatype >
	std::type_info const& AnyHolder< Datatype >::getTypeinfo() const
	{
		return typeid( Datatype );
	}

	template< typename Datatype >
	const std::string AnyHolder< Datatype >::getTypename() const
	{
		return typeid( Datatype ).name();
	}

	template< typename Datatype >
	void AnyHolder< Datatype >::writeTo( std::ostream &out ) const
	{
		out << m_Data;
	}

	template< typename Datatype >
	void AnyHolder< Datatype >::readFrom( std::istream &in )
	{
		in >> m_Data;
	}

	template< typename Datatype >
	AbstractAnyHolder * AnyHolder< Datatype >::create() const
	{ 
		return new AnyHolder< Datatype >();
	}

	template< typename Datatype >
	AbstractAnyHolder * AnyHolder< Datatype >::clone() const
	{
		return new AnyHolder< Datatype >( m_Data );
	}

}