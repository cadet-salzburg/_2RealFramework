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

#include "helpers/_2RealException.h"
#include "helpers/_2RealAnyHolder.h"

#include <typeinfo>
#include <sstream>
#include <memory>
#include <assert.h>

namespace _2Real
{
	class Any
	{

	public:

		Any();
		Any( Any const& src );
		Any& operator=( Any const& src );

		template< typename TData >
		explicit Any( TData const& value )
		{
			AnyHolder< TData > *holder = new AnyHolder< TData >( value );
			m_Content.reset( holder );
			m_Typename = typeid( TData ).name();
			m_TypeCode = typeid( TData ).hash_code();
		}

		template< typename TData >
		bool isDatatype() const
		{
			return m_Typename == typeid( TData ).name();
		}

		bool isEmpty() const;

		bool isEqualTo( Any const& any ) const;
		bool isLessThan( Any const& any ) const;

		void cloneFrom( Any const& src );
		void createNew( Any const& src );

		void writeTo( std::ostream &out ) const;
		void readFrom( std::istream &in );

		std::string const& getTypename() const;

		template< typename TData >
		TData & extract()
		{
			std::string name = typeid( TData ).name();
			if ( name == m_Typename )
			{
				AbstractAnyHolder *ptr = m_Content.get();
				AnyHolder< TData > &holder = dynamic_cast< AnyHolder< TData > & >( *ptr );
				return holder.m_Data;
			}
			else
			{
				std::ostringstream msg;
				msg << "type of data " << m_Typename << " does not match template parameter " << name << std::endl;
				throw TypeMismatchException( msg.str() );
			}
		}

		template< typename TData >
		TData const& extract() const
		{
			std::string name = typeid( TData ).name();
			if ( name == m_Typename )
			{
				AbstractAnyHolder *ptr = m_Content.get();
				AnyHolder< TData > &holder = dynamic_cast< AnyHolder< TData > & >( *ptr );
				return holder.m_Data;
			}
			else
			{
				std::ostringstream msg;
				msg << "type of data " << m_Typename << " does not match template parameter " << name << std::endl;
				throw TypeMismatchException( msg.str() );
			}
		}

	private:

		std::shared_ptr< AbstractAnyHolder >	m_Content;
		std::string								m_Typename;
		size_t									m_TypeCode;

	};
}
