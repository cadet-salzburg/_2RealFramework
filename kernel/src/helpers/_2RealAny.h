#pragma once

#include "helpers/_2RealException.h"
#include "helpers/_2RealHelpers.h"
#include "helpers/_2RealAnyHolder.h"

#include <typeinfo>
#include <sstream>

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
			return m_Content->getTypename() == typeinfo( T ).name();
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