#pragma once

#include "helpers/_2RealException.h"
#include "helpers/_2RealHelpers.h"
#include "helpers/_2RealTypeHolder.h"

#include <typeinfo>
#include <sstream>

namespace _2Real
{

	class EngineData
	{

		template< typename Datatype >
		friend Datatype & extractFrom( EngineData &data );

		template< typename Datatype >
		friend Datatype const& extractFrom( EngineData const& data );

		friend std::ostream& operator<<( std::ostream& out, EngineData const& data );
		friend std::istream& operator>>( std::istream& in, EngineData &data );

	public:

		EngineData();
		EngineData( EngineData const& src );
		EngineData& operator=( EngineData const& src );

		template< typename Datatype >
		explicit EngineData( Datatype const& value )
		{
			DataHolder< Datatype > *holder = new DataHolder< Datatype >( value );
			m_Content.reset( holder );
		}

		template< typename T >
		bool isDatatype() const
		{
			return m_Content->getTypename() == typeinfo( T ).name();
		}

		bool isEmpty() const;
		const std::string getTypename() const;

		void cloneFrom( EngineData const& src );
		void createNew( EngineData const& src );

	private:

		void writeTo( std::ostream &out ) const;
		void readFrom( std::istream &in );

		std::shared_ptr< AbstractDataHolder >	m_Content;

	};

	template< typename Datatype >
	Datatype & extractFrom( EngineData &data )
	{
		if ( data.getTypename() == typeid( Datatype ).name() )
		{
			AbstractDataHolder *ptr = data.m_Content.get();
			DataHolder< Datatype > &holder = dynamic_cast< DataHolder< Datatype > & >( *ptr );
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
	Datatype const& extractFrom( EngineData const& data )
	{
		if ( data.getTypename() == typeid( Datatype ).name() )
		{
			AbstractDataHolder *ptr = data.m_Content.get();
			DataHolder< Datatype > &holder = dynamic_cast< DataHolder< Datatype > & >( *ptr );
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