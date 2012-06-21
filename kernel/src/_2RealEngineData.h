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

	public:
	
		template< typename Datatype >
		friend Datatype * extractFrom( EngineData const& data );
		friend std::ostream& operator<<( std::ostream& out, EngineData const& data );
		friend std::istream& operator>>( std::istream& in, EngineData &data );

		EngineData();
		EngineData( EngineData const& src );
		EngineData& operator=( EngineData const& src );
		~EngineData();

		template< typename Datatype >
		EngineData(Datatype *const value);

		template< typename Datatype >
		EngineData(Datatype const& value);

		EngineData( AbstractDataHolder *holder );

		bool isEmpty() const;
		const std::string getTypename() const;
		std::type_info const& getTypeinfo() const;
		void clone( EngineData const& src );
		void create( EngineData const& src );

	private:

		std::shared_ptr< AbstractDataHolder >	m_Content;

		void writeTo( std::ostream &out ) const;
		void readFrom( std::istream &in );

	};

	inline std::ostream& operator<<( std::ostream& out, EngineData const& data )
	{
		data.writeTo( out );
		return out;
	}

	inline std::istream& operator>>( std::istream& in, EngineData &data )
	{
		data.readFrom( in );
		return in;
	}

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
	Datatype * extractFrom( EngineData const& data )
	{
		if ( data.getTypeinfo() == typeid( Datatype ) )
		{
			AbstractDataHolder const* ptr = data.m_Content.get();
			return ( static_cast< DataHolder< Datatype > const* >( ptr ) )->m_Data;
		}
		else
		{
			std::ostringstream msg;
			msg << "type of data " << data.getTypeinfo().name() << " does not match template parameter " << typeid( Datatype ).name() << std::endl;
			throw TypeMismatchException( msg.str() );
		}
	}

	inline EngineData::EngineData( AbstractDataHolder *holder )
	{
		m_Content.reset( holder );
	}

	template< typename Datatype >
	EngineData::EngineData( Datatype *const value )
	{
		DataHolder< Datatype > *holder = new DataHolder< Datatype >( value );
		m_Content.reset( holder );
	}

	template< typename Datatype >
	EngineData::EngineData( Datatype const& value )
	{
		Datatype *newData = new Datatype( value );
		DataHolder< Datatype > *holder = new DataHolder< Datatype >( newData );
		m_Content.reset(holder);
	}

	inline EngineData::EngineData() :
		m_Content()
	{
	}

	inline EngineData::EngineData( EngineData const& src ) :
		m_Content( src.m_Content )
	{
	}

	inline EngineData& EngineData::operator=( EngineData const& src )
	{
		if (this == &src)
		{
			return *this;
		}

		m_Content = src.m_Content;

		return *this;
	}

	inline EngineData::~EngineData()
	{
		m_Content.reset();
	}

	inline bool EngineData::isEmpty() const
	{
		return m_Content.get() == nullptr;
	}

	inline void EngineData::writeTo(std::ostream &out) const
	{
		if ( !isEmpty() )
		{
			m_Content->writeTo(out);
		}
	}

	inline void EngineData::readFrom(std::istream &in)
	{
		if ( !isEmpty() )
		{
			m_Content->readFrom(in);
		}
	}

	inline std::type_info const& EngineData::getTypeinfo() const
	{
		if ( isEmpty() )
		{
			return typeid( void );
		}
		else
		{
			return m_Content->getTypeinfo();
		}
	}

	inline const std::string EngineData::getTypename() const
	{
		if ( isEmpty() )
		{
			return typeid( void ).name();
		}
		else
		{
			return m_Content->getTypename();
		}
	}

	inline void EngineData::clone( EngineData const& src )
	{
		m_Content.reset( src.m_Content->clone() );
	}

	inline void EngineData::create( EngineData const& src )
	{
		m_Content.reset( src.m_Content->create() );
	}

}