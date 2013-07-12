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

#include "datatypes/_2RealTypes.h"
#include "datatypes/_2RealAbstractAnyHolder.h"
#include "helpers/_2RealException.h"

namespace _2Real
{
	template< typename TType >
	class AnyHolder : public AbstractAnyHolder
	{

	public:

		AnyHolder();
		explicit AnyHolder( TType const& value );

		std::string getHumanReadableName() const;
		std::type_info const& getTypeinfo() const;

		AbstractAnyHolder * create() const;
		AbstractAnyHolder * clone() const;
		void set( AbstractAnyHolder const& other );

		void writeTo( std::ostream &out ) const;
		void readFrom( std::istream &in );

		bool isEqualTo( AbstractAnyHolder const& other ) const;
		bool isLessThan( AbstractAnyHolder const& other ) const;

		std::shared_ptr< TType >		mData;

	private:

		AnyHolder( AnyHolder< TType > const& src );
		AnyHolder& operator=( AnyHolder< TType > const& src );

	};

	template< typename TType >
	AnyHolder< TType >::AnyHolder() :
		mData( new TType( Init< TType >::defaultValue() ) )
	{
	}

	template< typename TType >
	AnyHolder< TType >::AnyHolder( TType const& value ) :
		mData( new TType( value ) )
	{
	}

	template< typename TType >
	AnyHolder< TType >::AnyHolder( AnyHolder< TType > const& src ) :
		mData( new TType( *( src.mData.get() ) ) )
	{
	}

	template< typename TType >
	AnyHolder< TType >& AnyHolder< TType >::operator=( AnyHolder< TType > const& src )
	{
		if ( this == &src )
			return *this;
		mData.reset( new TType( *( src.mData.get() ) ) );
		return *this;
	}

	template< typename TType >
	void AnyHolder< TType >::set( AbstractAnyHolder const& other )
	{
		AnyHolder< TType > const& o = dynamic_cast< AnyHolder< TType > const& >( other );
		mData.reset( new TType( *( o.mData.get() ) ) );
	}

	template< typename TType >
	std::type_info const& AnyHolder< TType >::getTypeinfo() const
	{
		return typeid( TType );
	}

	template< typename TType >
	std::string AnyHolder< TType >::getHumanReadableName() const
	{
		return Name< TType >::humanReadableName();
	}

	template< typename TType >
	bool AnyHolder< TType >::isEqualTo( AbstractAnyHolder const& other ) const
	{
		if ( other.getTypeinfo() == this->getTypeinfo() )
		{
			AnyHolder< TType > const& holder = dynamic_cast< AnyHolder< TType > const& >( other );
			return *( mData.get() ) == *( holder.mData.get() );
		}
		else
		{
			std::ostringstream msg;
			msg << "type of other " << getHumanReadableName() << " does not match " << this->getHumanReadableName() << std::endl;
			throw TypeMismatchException( msg.str() );
		}
	}

	template< typename TType >
	bool AnyHolder< TType >::isLessThan( AbstractAnyHolder const& other ) const
	{
		if ( other.getTypeinfo() == this->getTypeinfo() )
		{
			AnyHolder< TType > const& holder = dynamic_cast< AnyHolder< TType > const& >( other );
			return *( mData.get() ) < *( holder.mData.get() );
		}
		else
		{
			std::ostringstream msg;
			msg << "type of other " << getHumanReadableName() << " does not match " << this->getHumanReadableName() << std::endl;
			throw TypeMismatchException( msg.str() );
		}
	}

	template< typename TType >
	void AnyHolder< TType >::writeTo( std::ostream &out ) const
	{
		_2Real::writeTo( out, *( mData.get() ) );
	}

	template< typename TType >
	void AnyHolder< TType >::readFrom( std::istream &in )
	{
		_2Real::readFrom( in, *( mData.get() ) );
	}

	template< typename TType >
	AbstractAnyHolder * AnyHolder< TType >::create() const
	{
		return new AnyHolder< TType >();
	}

	template< typename TType >
	AbstractAnyHolder * AnyHolder< TType >::clone() const
	{
		return new AnyHolder< TType >( *( mData.get() ) );
	}

	template< typename TType >
	std::shared_ptr< TType > extract( AbstractAnyHolder &holder )
	{
		try
		{
			AnyHolder< TType > &ref = dynamic_cast< AnyHolder< TType > & >( holder );
			return ref.mData;
		}
		catch ( std::bad_cast const& e )
		{
			( void ) e;

			std::ostringstream msg;
			msg << "type " << holder.getHumanReadableName() << " does not match template parameter " << Name< TType >::humanReadableName() << std::endl;
			throw TypeMismatchException( msg.str() );
		}
	}

	template< typename TType >
	std::shared_ptr< const TType > extract( AbstractAnyHolder const& holder )
	{
		try
		{
			AnyHolder< TType > const& ref = dynamic_cast< AnyHolder< TType > const& >( holder );
			return ref.mData;
		}
		catch ( std::bad_cast const& e )
		{
			( void ) e;

			std::ostringstream msg;
			msg << "type " << holder.getHumanReadableName() << " does not match template parameter " << Name< TType >::humanReadableName() << std::endl;
			throw TypeMismatchException( msg.str() );
		}
	}
}
