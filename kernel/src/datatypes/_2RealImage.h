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

#include "datatypes/_2RealImageT.h"

#undef min
#undef max
#include <limits>
#include <numeric>
#include <iostream>

namespace _2Real
{
	//template< typename T >
	//class Deleter
	//{
	//public:
	//	Deleter( T *data ) : m_Data( data ) {};
	//	virtual ~Deleter() {}
	//	T *m_Data;
	//};

	//template< typename T >
	//class NullDeleter : public Deleter< T >
	//{
	//public:
	//	NullDeleter( T *data ) : Deleter( data ) {};
	//};

	//template< typename T >
	//class ArrayDeleter : public Deleter< T >
	//{
	//public:
	//	ArrayDeleter( T *data ) : Deleter( data ) {};
	//	~ArrayDeleter() { delete [] m_Data; }
	//};

	//template< typename T >
	//struct static_cast_func
	//{
	//	template < typename TOther >
	//	T const& operator()( TOther const& val ) const { return static_cast< T const& >( val ); }
	//};

	//class ImageChannel
	//{

	//	template< typename TData >
	//	friend TData* extractData( ImageChannel &channel );

	//	template< typename TData >
	//	friend TData *const extractData( ImageChannel const& channel );

	//private:

	//	template< typename T >
	//	class ChannelObjectT;

	//	class AbstractChannelObject
	//	{

	//	public:

	//		AbstractChannelObject( const unsigned int width, const unsigned int height, const unsigned int bytesPerRow, const unsigned char inc );
	//		virtual ~AbstractChannelObject();

	//		virtual void copyInto( AbstractChannelObject *other ) const = 0;

	//		unsigned int				m_Width, m_Height, m_RowPitch;
	//		unsigned char				m_Increment;

	//	};

	//	template< typename TData >
	//	class ChannelObjectT : public AbstractChannelObject
	//	{

	//	public:

	//		ChannelObjectT( TData *data, const bool ownsData, const unsigned int width, const unsigned int height, const unsigned int bytesPerRow, const unsigned char inc ) :
	//			AbstractChannelObject( width, height, bytesPerRow, inc ),
	//			m_Data( data ),
	//			m_Deleter( ownsData ? ( Deleter< TData > )ArrayDeleter< TData >( data ) : NullDeleter( data ) )
	//		{
	//		}

	//		void copyInto( AbstractChannelObject *other ) const
	//		{
	//		}

	//		TData						*m_Data;
	//		Deleter< TData >			m_Deleter;

	//	};

	//	template< >
	//	class ChannelObjectT< unsigned char > : public AbstractChannelObject
	//	{

	//	public:

	//		ChannelObjectT( unsigned char *data, const bool ownsData, const uint32_t width, const uint32_t height, const uint32_t bytesPerRow, const uint8_t inc ) :
	//			AbstractChannelObject( width, height, bytesPerRow, inc ),
	//			m_Data( data ),
	//			m_Deleter( ownsData ? ( Deleter< unsigned char > )ArrayDeleter< unsigned char >( data ) : NullDeleter< unsigned char >( data ) )
	//		{
	//		}

	//		void copyInto( AbstractChannelObject *other ) const
	//		{
	//		}

	//		unsigned char				*m_Data;
	//		Deleter< unsigned char >	m_Deleter;

	//	};

	//	typedef ChannelObjectT< unsigned char >		Channel_uchar;
	//	typedef ChannelObjectT< unsigned short >	Channel_ushort;
	//	typedef ChannelObjectT< float >				Channel_float;
	//	typedef ChannelObjectT< double >			Channel_double;

	//	AbstractChannelObject	*m_ChannelObject;

	//public:

	//	ImageChannel() :
	//		m_ChannelObject( nullptr )
	//	{
	//	};

	//	~ImageChannel()
	//	{
	//		delete m_ChannelObject;
	//	}

	//	ImageChannel( ImageChannel const& src ) :
	//		m_ChannelObject( nullptr )
	//	{
	//		src.m_ChannelObject->copyInto( m_ChannelObject );
	//	}

	//};

	//template< typename TData >
	//class ImageSource
	//{
	//	ImageSource( const unsigned int w, const unsigned int h, const unsigned int bpr, const unsigned char inc, ImageChannelOrder const& order );
	//	ImageSource( TData *data, const unsigned int w, const unsigned int h, const unsigned int bpr, const unsigned char inc, ImageChannelOrder const& order );
	//};

	//class Image
	//{

	//	friend std::ostream& operator<<( std::ostream& out, Image const& image );
	//	friend std::istream& operator>>( std::istream& in, Image &image );

	//	template< typename TData >
	//	friend TData* extractData( Image &img );

	//	template< typename TData >
	//	friend TData *const extractData( Image const& img );

	//private:

	//	template< typename TData >
	//	class ImageObjectT;

	//	class AbstractImageObject
	//	{

	//	public:

	//		AbstractImageObject( const unsigned int w, const unsigned int h, const unsigned int bpr, const unsigned char inc, ImageChannelOrder const& order ) :
	//			m_Width( w ),
	//			m_Height( h ),
	//			m_RowPitch( bpr ),
	//			m_Increment( inc ),
	//			m_ChannelOrder( order )
	//		{
	//		}

	//		virtual ~AbstractImageObject() {}

	//		bool isEmpty() const
	//		{
	//			return ( m_Width == 0 || m_Height == 0 );
	//		}

	//		virtual void copyInto( AbstractImageObject *other ) const = 0;

	//		virtual void convertTo( ImageT< unsigned char > &img ) const = 0;
	//		virtual void convertTo( ImageT< unsigned short > &img ) const = 0;
	//		virtual void convertTo( ImageT< float > &img ) const = 0;
	//		virtual void convertTo( ImageT< double > &img ) const = 0;

	//		unsigned int		m_Width, m_Height, m_RowPitch;
	//		unsigned char		m_Increment;
	//		ImageChannelOrder	m_ChannelOrder;

	//	};

	//	template< typename TData >
	//	class ImageObjectT : public AbstractImageObject
	//	{

	//	public:

	//		ImageObjectT( TData *data, const bool owns, const unsigned int w, const unsigned int h, const unsigned int bpr, const unsigned char inc, ImageChannelOrder const& order ) :
	//			AbstractImageObject( w, h, bpr, inc, order ),
	//			m_Data( data ),
	//			m_Deleter( owns ? ( Deleter< TData > )ArrayDeleter< TData >( data ) : NullDeleter< TData >( data ) )
	//		{
	//		}

	//		void copyInto( AbstractImageObject *other ) const
	//		{
	//			if ( other != nullptr )
	//			{
	//				delete other;
	//			}

	//			size_t sz = m_Width * m_Height;
	//			TData *data = new TData[ sz ];
	//			memcpy( data, m_Data, sz*sizeof( TData ) );

	//			other = new ImageObjectT< TData >( data, true, m_Width, m_Height, m_RowPitch, m_Increment, m_ChannelOrder );
	//		}

	//		void convertTo( ImageT< unsigned char > &img ) const
	//		{
	//		}

	//		void convertTo( ImageT< unsigned short > &img ) const
	//		{
	//		}

	//		void convertTo( ImageT< float > &img ) const
	//		{
	//		}

	//		void convertTo( ImageT< double > &img ) const
	//		{
	//		}

	//		TData						*m_Data;
	//		Deleter< TData >			m_Deleter;

	//	};

	//	template< >
	//	class ImageObjectT< unsigned char > : public AbstractImageObject
	//	{

	//	public:

	//		ImageObjectT( unsigned char *data, const bool owns, const unsigned int w, const unsigned int h, const unsigned int bpr, const unsigned char inc, ImageChannelOrder const& order ) :
	//			AbstractImageObject( w, h, bpr, inc, order ),
	//			m_Data( data ),
	//			m_Deleter( owns ? ( Deleter< unsigned char > )ArrayDeleter< unsigned char >( data ) : NullDeleter< unsigned char >( data ) )
	//		{
	//		}

	//		void copyInto( AbstractImageObject *other ) const
	//		{
	//			if ( other != nullptr )
	//			{
	//				delete other;
	//			}

	//			size_t sz = m_Width * m_Height;
	//			unsigned char *data = new unsigned char[ sz ];
	//			memcpy( data, m_Data, sz*sizeof( unsigned char ) );

	//			other = new ImageObjectT< unsigned char >( data, true, m_Width, m_Height, m_RowPitch, m_Increment, m_ChannelOrder );
	//		}

	//		void convertTo( ImageT< unsigned char > &img ) const
	//		{
	//		}

	//		void convertTo( ImageT< unsigned short > &img ) const
	//		{
	//			// check: if nullptr
	//			// if size = 0
	//			// if channel mismatch

	//			size_t sz = m_Width * m_Height;
	//			unsigned short *data = new unsigned short[ sz ];
	//			memset( data, 0, sz*sizeof( unsigned short ) );

	//			unsigned char *pUChar = &m_Data[ 0 ];
	//			unsigned short *pUShort = &data[ 0 ];

	//			pUShort += sizeof( unsigned char );

	//			for ( unsigned int i=0; i<sz; ++i )
	//			{
	//				*pUShort = *pUChar;
	//				pUShort++;
	//				pUChar++;
	//			}
	//		}

	//		void convertTo( ImageT< float > &img ) const
	//		{
	//		}

	//		void convertTo( ImageT< double > &img ) const
	//		{
	//		}

	//		unsigned char				*m_Data;
	//		Deleter< unsigned char >	m_Deleter;

	//	};

	//	//template< >
	//	//class ImageObjectT< unsigned short > : public AbstractImageObject
	//	//{

	//	//public:

	//	//	ImageObjectT( unsigned short *data, const bool owns, const unsigned int w, const unsigned int h, const unsigned int bpr, const unsigned char inc, ImageChannelOrder const& order ) :
	//	//		AbstractImageObject( w, h, bpr, inc, order ),
	//	//		m_Data( data ),
	//	//		m_Deleter( owns ? ( Deleter< unsigned short > )ArrayDeleter< unsigned short >( data ) : NullDeleter< unsigned short >( data ) )
	//	//	{
	//	//	}

	//	//	void copyInto( AbstractImageObject *other ) const
	//	//	{
	//	//		if ( other != nullptr )
	//	//		{
	//	//			delete other;
	//	//		}

	//	//		size_t sz = m_Width * m_Height;
	//	//		unsigned short *data = new unsigned short[ sz ];
	//	//		memcpy( data, m_Data, sz*sizeof( unsigned short ) );

	//	//		other = new ImageObjectT< unsigned short >( data, true, m_Width, m_Height, m_RowPitch, m_Increment, m_ChannelOrder );
	//	//	}

	//	//	void convertTo( ImageT< unsigned char > &img )
	//	//	{
	//	//	}

	//	//	void convertTo( ImageT< unsigned short > &img )
	//	//	{
	//	//	}

	//	//	void convertTo( ImageT< float > &img )
	//	//	{
	//	//	}

	//	//	void convertTo( ImageT< double > &img )
	//	//	{
	//	//	}

	//	//	unsigned short				*m_Data;
	//	//	Deleter< unsigned short >	m_Deleter;

	//	//};

	//	//typedef ImageObjectT< unsigned char >		Image_uchar;
	//	//typedef ImageObjectT< unsigned short >		Image_ushort;
	//	//typedef ImageObjectT< float >				Image_float;
	//	//typedef ImageObjectT< double >				Image_double;

	//	AbstractImageObject		*m_ImageObject;

	//public:

	//	Image() :
	//		m_ImageObject( nullptr )
	//	{
	//		std::cout << "IMAGE CTOR" << std::endl;
	//	};

	//	template< typename TData >
	//	Image( TData *data, const bool owns, const unsigned int w, const unsigned int h, const unsigned int bpr, const unsigned char inc, ImageChannelOrder const& order ) :
	//		m_ImageObject( new ImageObjectT< TData >( data, owns, w, h, bpr, inc, order ) )
	//	{
	//		std::cout << "IMAGE INIT" << std::endl;
	//	}

	//	//template< typename TData >
	//	//Image( ImageSource< TData > &src, bool owns ) :
	//	//	m_ImageObject( new ImageObjectT< TData >( src.data, owns, src.w, src.h, src.bpr, src.inc, src.order ) )
	//	//{
	//	//}

	//	~Image()
	//	{
	//		std::cout << "IMAGE DTOR" << std::endl;
	//		delete m_ImageObject;
	//	}

	//	Image( Image const& src ) :
	//		m_ImageObject( nullptr )
	//	{
	//		std::cout << "IMAGE COPY" << std::endl;
	//		//src.m_ImageObject->copyInto( m_ImageObject );
	//	}

	//	void copyTo( ImageT< unsigned char > &img ) const
	//	{
	//		m_ImageObject->convertTo( img );
	//	}

	//	void copyTo( ImageT< unsigned short > &img ) const
	//	{
	//		m_ImageObject->convertTo( img );
	//	}

	//	void copyTo( ImageT< float > &img ) const
	//	{
	//		m_ImageObject->convertTo( img );
	//	}

	//	void copyTo( ImageT< double > &img ) const
	//	{
	//		m_ImageObject->convertTo( img );
	//	}

	//};

	//template< typename TData >
	//TData * extractData( Image &image )
	//{
	//	return nullptr;
	//}

	//template< typename TData >
	//TData *const extractData( Image const& image )
	//{
	//	return nullptr;
	//}

	//inline std::ostream& operator<<( std::ostream& out, Image const& image )
	//{
	//	return out;
	//}

	//inline std::istream& operator>>( std::istream& in, Image &image )
	//{
	//	return in;
	//}
}
