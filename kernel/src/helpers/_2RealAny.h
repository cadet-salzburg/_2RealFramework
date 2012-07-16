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

namespace _2Real
{

	class Any
	{

		template< typename Datatype >
		friend Datatype & extractFrom( Any &data );

		template< typename Datatype >
		friend Datatype const& extractFrom( Any const& data );

		friend std::ostream& operator<<( std::ostream& out, Any const& data );
		friend std::istream& operator>>( std::istream& in, Any &data );

	public:

		Any();
		Any( Any const& src );
		Any& operator=( Any const& src );

		template< typename Datatype >
		explicit Any( Datatype const& value )
		{
			AnyHolder< Datatype > *holder = new AnyHolder< Datatype >( value );
			m_Content.reset( holder );
		}

		template< typename T >
		bool isDatatype() const
		{
			return m_Content->getTypename() == typeid( T ).name();
		}

		bool isEmpty() const;
		const std::string getTypename() const;

		void cloneFrom( Any const& src );
		void createNew( Any const& src );

	private:

		void writeTo( std::ostream &out ) const;
		void readFrom( std::istream &in );

		std::shared_ptr< AbstractAnyHolder >	m_Content;

	};

	template< typename Datatype >
	Datatype & extractFrom( Any &data )
	{
		if ( data.getTypename() == typeid( Datatype ).name() )
		{
			AbstractAnyHolder *ptr = data.m_Content.get();
			AnyHolder< Datatype > &holder = dynamic_cast< AnyHolder< Datatype > & >( *ptr );
			return holder.m_Data;
		}
		else
		{
			std::ostringstream msg;
			msg << "type of data " << data.getTypename() << " does not match template parameter " << typeid( Datatype ).name() << std::endl;
			throw TypeMismatchException( msg.str() );
		}
	}

	template< typename Datatype >
	Datatype const& extractFrom( Any const& data )
	{
		if ( data.getTypename() == typeid( Datatype ).name() )
		{
			AbstractAnyHolder *ptr = data.m_Content.get();
			AnyHolder< Datatype > &holder = dynamic_cast< AnyHolder< Datatype > & >( *ptr );
			return holder.m_Data;
		}
		else
		{
			std::ostringstream msg;
			msg << "type of data " << data.getTypename() << " does not match template parameter " << typeid( Datatype ).name() << std::endl;
			throw TypeMismatchException( msg.str() );
		}
	}


}
