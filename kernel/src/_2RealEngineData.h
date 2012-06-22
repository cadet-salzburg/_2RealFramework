#pragma once

#include "_2RealException.h"
#include "_2RealHelpers.h"
#include "_2RealTypeHolder.h"

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

		// called from either a bundle or the app
		//template< typename Datatype >
		//void clone( Datatype const& value )
		//{
		//	DataHolder< Datatype > *holder = new DataHolder< Datatype >( value );
		//	m_Content.reset( holder );
		//}

		// called from outside of the framework
		//template< typename Datatype >
		//EngineData( Datatype const& value )
		//{
		//	Datatype *newData = new Datatype( value );
		//	DataHolder< Datatype > *holder = new DataHolder< Datatype >( newData );
		//	m_Content.reset(holder);
		//}

		bool isEmpty() const;
		const std::string getTypename() const;	//argh

		void cloneFrom( EngineData const& src );
		void createNew( EngineData const& src );

	private:

		void writeTo( std::ostream &out ) const;
		void readFrom( std::istream &in );

		std::shared_ptr< AbstractDataHolder >	m_Content;

	};

	//template< typename Datatype >
	//std::shared_ptr< Datatype > const& extractFrom( EngineData const& data )
	//{
	//	if ( data.getTypeinfo() == typeid( Datatype ) )
	//	{
	//		AbstractDataHolder const* ptr = data.m_Content.get();
	//	}
	//	else
	//	{
	//		std::ostringstream msg;
	//		msg << "type of data " << data.getTypeinfo().name() << " does not match template parameter " << typeid( Datatype ).name() << std::endl;
	//		throw TypeMismatchException( msg.str() );
	//	}			return ( static_cast< DataHolder< Datatype > const* >( ptr ) )->m_Data;
	//}

	template< typename Datatype >
	Datatype & extractFrom( EngineData &data )
	{
		if ( data.getTypename() == typeid( Datatype ).name() )
		{
			AbstractDataHolder *ptr = data.m_Content.get();
			DataHolder< Datatype > &holder = dynamic_cast< DataHolder< Datatype > & >( *ptr );
			//return ( static_cast< DataHolder< Datatype > const* >( ptr ) )->m_Data;
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
			//return ( static_cast< DataHolder< Datatype > const* >( ptr ) )->m_Data;
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