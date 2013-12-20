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

#include "engine/_2RealAbstractAnyHolder.h"
#include "engine/_2RealFrameworkCompatibleType.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	template< typename TType >
	class AnyHolder_T : public AbstractAnyHolder
	{

	public:

		// default value of TType
		AnyHolder_T();
		// mData points to value ( used by custom type in data field... )
		explicit AnyHolder_T( std::shared_ptr< TType > value );
		// copies value, new shared ptr
		explicit AnyHolder_T( TType const& value );

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

		AnyHolder_T( AnyHolder_T< TType > const& other );
		AnyHolder_T& operator=( AnyHolder_T< TType > const& other );

	};

	template< typename TType >
	AnyHolder_T< TType >::AnyHolder_T() :
		mData( new TType( FrameworkCompatibleType< TType >::defaultValue() ) )
	{
	}

	template< typename TType >
	AnyHolder_T< TType >::AnyHolder_T( std::shared_ptr< TType > value ) :
		mData( value )
	{
	}

	template< typename TType >
	AnyHolder_T< TType >::AnyHolder_T( TType const& value ) :
		mData( new TType( value ) )
	{
	}

	template< typename TType >
	AnyHolder_T< TType >::AnyHolder_T( AnyHolder_T< TType > const& src ) :
		mData( new TType( *( src.mData.get() ) ) )
	{
	}

	template< typename TType >
	AnyHolder_T< TType >& AnyHolder_T< TType >::operator=( AnyHolder_T< TType > const& src )
	{
		if ( this == &src )
			return *this;
		mData.reset( new TType( *( src.mData.get() ) ) );
		return *this;
	}

	template< typename TType >
	void AnyHolder_T< TType >::set( AbstractAnyHolder const& other )
	{
		AnyHolder_T< TType > const& o = dynamic_cast< AnyHolder_T< TType > const& >( other );
		mData.reset( new TType( *( o.mData.get() ) ) );
	}

	template< typename TType >
	std::type_info const& AnyHolder_T< TType >::getTypeinfo() const
	{
		return typeid( TType );
	}

	template< typename TType >
	std::string AnyHolder_T< TType >::getHumanReadableName() const
	{
		return FrameworkCompatibleType< TType >::humanReadableName();
	}

	template< typename TType >
	bool AnyHolder_T< TType >::isEqualTo( AbstractAnyHolder const& other ) const
	{
		if ( other.getTypeinfo() == this->getTypeinfo() )
		{
			AnyHolder_T< TType > const& holder = dynamic_cast< AnyHolder_T< TType > const& >( other );
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
	bool AnyHolder_T< TType >::isLessThan( AbstractAnyHolder const& other ) const
	{
		if ( other.getTypeinfo() == this->getTypeinfo() )
		{
			AnyHolder_T< TType > const& holder = dynamic_cast< AnyHolder_T< TType > const& >( other );
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
	void AnyHolder_T< TType >::writeTo( std::ostream &out ) const
	{
		FrameworkCompatibleType< TType >::writeTo( out, *( mData.get() ) );
	}

	template< typename TType >
	void AnyHolder_T< TType >::readFrom( std::istream &in )
	{
		FrameworkCompatibleType< TType >::readFrom( in, *( mData.get() ) );
	}

	template< typename TType >
	AbstractAnyHolder * AnyHolder_T< TType >::create() const
	{
		return new AnyHolder_T< TType >();
	}

	template< typename TType >
	AbstractAnyHolder * AnyHolder_T< TType >::clone() const
	{
		return new AnyHolder_T< TType >( *( mData.get() ) );
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
