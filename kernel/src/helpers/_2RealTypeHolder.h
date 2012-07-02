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
	class AbstractDataHolder
	{
	
	public:
	
		virtual ~AbstractDataHolder() {}
		virtual const std::string getTypename() const = 0;
		virtual std::type_info const& getTypeinfo() const = 0;
		virtual AbstractDataHolder* clone() const = 0;
		virtual AbstractDataHolder* create() const = 0;
		virtual void writeTo( std::ostream &out ) const = 0;
		virtual void readFrom( std::istream &in ) = 0;

	};

	template< typename Datatype >
	class DataHolder : public AbstractDataHolder
	{
	
	public:

		DataHolder();
		explicit DataHolder( Datatype const& value );

		const std::string getTypename() const;
		std::type_info const& getTypeinfo() const;
		AbstractDataHolder * create() const;
		AbstractDataHolder * clone() const;

		void writeTo( std::ostream &out ) const;
		void readFrom( std::istream &in );

		Datatype		m_Data;

	private:

		DataHolder( DataHolder< Datatype > const& src );
		DataHolder& operator=( DataHolder< Datatype > const& src );

	};

	template< typename Datatype >
	DataHolder< Datatype >::DataHolder() :
		m_Data()
	{
	}

	template< typename Datatype >
	DataHolder< Datatype >::DataHolder( Datatype const& value ) :
		m_Data( value )
	{
	}

	template< typename Datatype >
	DataHolder< Datatype >::DataHolder( DataHolder< Datatype > const& src ) :
		m_Data( src.m_Data )
	{
		std::cout << "XXXXXXXXXXXXXXXCOPYXXXXXXXXXXXX" << std::endl;
	}

	template< typename Datatype >
	DataHolder< Datatype >& DataHolder< Datatype >::operator=( DataHolder< Datatype > const& src )
	{
		if ( this == &src )
		{
			return *this;
		}

		m_Data = src.m_Data;
		std::cout << "XXXXXXXXXXXXASSIGNXXXXXXXXXXXXXXX" << std::endl;

		return *this;
	}

	template< typename Datatype >
	std::type_info const& DataHolder< Datatype >::getTypeinfo() const
	{
		return typeid( Datatype );
	}

	template< typename Datatype >
	const std::string DataHolder< Datatype >::getTypename() const
	{
		return typeid( Datatype ).name();
	}

	template< typename Datatype >
	void DataHolder< Datatype >::writeTo( std::ostream &out ) const
	{
		out << m_Data;
	}

	template< typename Datatype >
	void DataHolder< Datatype >::readFrom( std::istream &in )
	{
		in >> m_Data;
	}

	template< typename Datatype >
	AbstractDataHolder * DataHolder< Datatype >::create() const
	{ 
		return new DataHolder< Datatype >();
	}

	template< typename Datatype >
	AbstractDataHolder * DataHolder< Datatype >::clone() const
	{
		return new DataHolder< Datatype >( m_Data );
	}

}