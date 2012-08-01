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
#include "helpers/_2RealTypeDescriptor.h"
#include "datatypes/_2RealTypes.h"

#ifdef _UNIX
	#include <typeinfo>
#else
	#include <typeinfo.h>
#endif

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

		template< typename TType >
		explicit Any( TType const& value )
		{
			m_Content.reset( new AnyHolder< TType >( value ) );
			m_TypeDescriptor.reset( createTypeDescriptor< TType >() );
		}

		bool isNull() const;
		Type const& getType() const;
		TypeCategory const& getTypeCategory() const;

		template< typename TType >
		bool isDatatype() const
		{
			return m_TypeDescriptor->getTypeInfo() == typeid( TType );
		}

		bool isEqualTo( Any const& any ) const;
		bool isLessThan( Any const& any ) const;

		void cloneFrom( Any const& src );
		void createNew( Any const& src );

		void writeTo( std::ostream &out ) const;
		void readFrom( std::istream &in );

		template< typename TType >
		TType & extract()
		{
			std::type_info const& info = typeid( TType );
			if ( info == m_TypeDescriptor->getTypeInfo() )
			{
				AbstractAnyHolder *ptr = m_Content.get();
				AnyHolder< TType > &holder = dynamic_cast< AnyHolder< TType > & >( *ptr );
				return holder.m_Data;
			}
			else
			{
				TypeDescriptor *t = createTypeDescriptor< TType >();
				std::ostringstream msg;
				msg << "type of data " << m_TypeDescriptor->getTypename() << " does not match the template parameter " << t->getTypename() << std::endl;
				throw TypeMismatchException( msg.str() );
			}
		}

		template< typename TType >
		TType const& extract() const
		{
			std::type_info const& info = typeid( TType );
			if ( info == m_TypeDescriptor->getTypeInfo() )
			{
				AbstractAnyHolder *ptr = m_Content.get();
				AnyHolder< TType > &holder = dynamic_cast< AnyHolder< TType > & >( *ptr );
				return holder.m_Data;
			}
			else
			{
				TypeDescriptor *t = createTypeDescriptor< TType >();
				std::ostringstream msg;
				msg << "type of data " << m_TypeDescriptor->getTypename() << " does not match the template parameter " << t->getTypename() << std::endl;
				throw TypeMismatchException( msg.str() );
			}
		}

	private:

		std::shared_ptr< TypeDescriptor >		m_TypeDescriptor;
		std::shared_ptr< AbstractAnyHolder >	m_Content;

	};
}