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
#include "helpers/_2RealHelpers.h"

namespace _2Real
{

	template< typename TData >
	class Deleter
	{
	public:
		virtual void safeDelete( TData *& data) const = 0;
		virtual ~Deleter() {}
	};

	template< typename TData >
	class NullDeleter : public Deleter< TData >
	{
	public:
		void safeDelete( TData *& data) const {}
	};

	template< typename TData >
	class ArrayDeleter : public Deleter< TData >
	{
	public:
		void safeDelete( TData *& data) const { delete [] data; data = nullptr; }
	};

	class ImageType
	{

	public:

		enum IMAGE_TYPE { UNSIGNED_BYTE, UNSIGNED_SHORT, FLOAT, DOUBLE };

		ImageType( const IMAGE_TYPE imageType ) :
			m_ImageType( imageType )
		{
			switch ( imageType )
			{
			case UNSIGNED_BYTE:
				m_ByteSize = sizeof( unsigned char );
				break;
			case UNSIGNED_SHORT:
				m_ByteSize = sizeof( unsigned short );
				break;
			case FLOAT:
				m_ByteSize = sizeof( float );
				break;
			case DOUBLE:
				m_ByteSize = sizeof( double );
				break;
			}
		}

		ImageType( ImageType const& src ) :
			m_ImageType( src.m_ImageType ),
			m_ByteSize( src.m_ByteSize )
		{
		}

		ImageType& operator=( ImageType const& src )
		{
			m_ImageType = src.m_ImageType;
			m_ByteSize = src.m_ByteSize;
			return *this;
		}

		unsigned char	getByteSize() const { return m_ByteSize; }
		int				getDatatype() const { return m_ImageType; }

		const bool operator==( ImageType const& src ) const { return m_ImageType == src.m_ImageType; }

	private:

		IMAGE_TYPE		m_ImageType;
		size_t			m_ByteSize;

	};

	class Image
	{

		friend std::ostream& operator<<( std::ostream& out, Image const& image );
		friend std::istream& operator>>( std::istream& in, Image &image );

	public:

		Image() :
			m_Data( nullptr ),
			m_Size( 0 ),
			m_Width( 0 ),
			m_Height( 0 ),
			m_ChannelOrder( ImageChannelOrder::R ),
			m_ImageType( ImageType::UNSIGNED_BYTE ),
			m_Deleter( new NullDeleter< unsigned char >() )
		{
		}

		~Image()
		{
			m_Deleter->safeDelete( m_Data );
			delete m_Deleter;
		}

		Image( Image const& src ) :
			m_Size( src.m_Size ),
			m_Width( src.m_Width ),
			m_Height( src.m_Height ),
			m_ChannelOrder( src.m_ChannelOrder ),
			m_ImageType( src.m_ImageType )
		{
			if ( src.m_Data != nullptr )
			{
				m_Deleter = new ArrayDeleter< unsigned char >();
				m_Data = new unsigned char[ m_Size ];
				memcpy( m_Data, src.m_Data, m_Size );
			}
			else
			{
				m_Data = nullptr;
				m_Deleter = new NullDeleter< unsigned char >();
			}
		}

		Image& operator=( Image const& src )
		{
			if ( this == &src )
			{
				return *this;
			}

			m_Size = src.m_Size;
			m_Width = src.m_Width;
			m_Height = src.m_Height;
			m_ChannelOrder = src.m_ChannelOrder;
			m_ImageType = src.m_ImageType;

			m_Deleter->safeDelete( m_Data );
			safeDelete( m_Deleter );

			if ( src.m_Data != nullptr )
			{
				m_Deleter = new ArrayDeleter< unsigned char >();
				m_Data = new unsigned char[ m_Size ];
				memcpy( m_Data, src.m_Data, m_Size );
			}
			else
			{
				m_Data = nullptr;
				m_Deleter = new NullDeleter< unsigned char >();
			}

			return *this;
		}

		Image( unsigned char *data, bool owns, const unsigned int w, const unsigned int h, const ImageChannelOrder o ) :
			m_Data( data ),
			m_Size( w * h * o.getNumberOfChannels() * sizeof( unsigned char ) ),
			m_Width( w ),
			m_Height( h ),
			m_ImageType( ImageType::UNSIGNED_BYTE ),
			m_ChannelOrder( o ),
			m_Deleter( owns ? static_cast< Deleter< unsigned char > * >( new ArrayDeleter< unsigned char > ) : new NullDeleter< unsigned char > )
		{
		}

		Image( unsigned short *data, bool owns, const unsigned int w, const unsigned int h, const ImageChannelOrder o ) :
			m_Data( reinterpret_cast< unsigned char * >( data ) ),
			m_Size( w * h * o.getNumberOfChannels() * sizeof( unsigned short ) ),
			m_Width( w ),
			m_Height( h ),
			m_ImageType( ImageType::UNSIGNED_SHORT ),
			m_ChannelOrder( o ),
			m_Deleter( owns ? static_cast< Deleter< unsigned char > * >( new ArrayDeleter< unsigned char > ) : new NullDeleter< unsigned char > )
		{
		}

		Image( float *data, bool owns, const unsigned int w, const unsigned int h, const ImageChannelOrder o ) :
			m_Data( reinterpret_cast< unsigned char * >( data ) ),
			m_Size( w * h * o.getNumberOfChannels() * sizeof( float ) ),
			m_Width( w ),
			m_Height( h ),
			m_ImageType( ImageType::FLOAT ),
			m_ChannelOrder( o ),
			m_Deleter( owns ? static_cast< Deleter< unsigned char > * >( new ArrayDeleter< unsigned char > ) : new NullDeleter< unsigned char > )
		{
		}

		Image( double *data, bool owns, const unsigned int w, const unsigned int h, const ImageChannelOrder o ) :
			m_Data( reinterpret_cast< unsigned char * >( data ) ),
			m_Size( w * h * o.getNumberOfChannels() * sizeof( double ) ),
			m_Width( w ),
			m_Height( h ),
			m_ImageType( ImageType::DOUBLE ),
			m_ChannelOrder( o ),
			m_Deleter( owns ? static_cast< Deleter< unsigned char > * >( new ArrayDeleter< unsigned char > ) : new NullDeleter< unsigned char > )
		{
		}

		void assign( unsigned char *data, bool owns, const unsigned int w, const unsigned int h, const ImageChannelOrder o )
		{
			m_ImageType = ImageType::UNSIGNED_BYTE;
			m_Size = ( w * h * o.getNumberOfChannels() * sizeof( unsigned char ) );
			m_Width = w;
			m_Height = h;
			m_ChannelOrder = o;

			m_Deleter->safeDelete( m_Data );
			safeDelete( m_Deleter );

			if ( data != nullptr )
			{
				m_Deleter = new ArrayDeleter< unsigned char >();
				m_Data = new unsigned char[ m_Size ];
				memcpy( m_Data, data, m_Size );
			}
			else
			{
				m_Data = nullptr;
				m_Deleter = new NullDeleter< unsigned char >();
			}
		}

		void assign( unsigned short *data, bool owns, const unsigned int w, const unsigned int h, const ImageChannelOrder o )
		{
			m_ImageType = ImageType::UNSIGNED_SHORT;
			m_Size = ( w * h * o.getNumberOfChannels() * sizeof( unsigned short ) );
			m_Width = w;
			m_Height = h;
			m_ChannelOrder = o;

			m_Deleter->safeDelete( m_Data );
			safeDelete( m_Deleter );

			if ( data != nullptr )
			{
				m_Deleter = new ArrayDeleter< unsigned char >();
				m_Data = new unsigned char[ m_Size ];
				memcpy( m_Data, data, m_Size );
			}
			else
			{
				m_Data = nullptr;
				m_Deleter = new NullDeleter< unsigned char >();
			}
		}

		void assign( float *data, bool owns, const unsigned int w, const unsigned int h, const ImageChannelOrder o )
		{
			m_ImageType = ImageType::FLOAT;
			m_Size = ( w * h * o.getNumberOfChannels() * sizeof( float ) );
			m_Width = w;
			m_Height = h;
			m_ChannelOrder = o;

			m_Deleter->safeDelete( m_Data );
			safeDelete( m_Deleter );

			if ( data != nullptr )
			{
				m_Deleter = new ArrayDeleter< unsigned char >();
				m_Data = new unsigned char[ m_Size ];
				memcpy( m_Data, data, m_Size );
			}
			else
			{
				m_Data = nullptr;
				m_Deleter = new NullDeleter< unsigned char >();
			}
		}

		void assign( double *data, bool owns, const unsigned int w, const unsigned int h, const ImageChannelOrder o )
		{
			m_ImageType = ImageType::DOUBLE;
			m_Size = ( w * h * o.getNumberOfChannels() * sizeof( double ) );
			m_Width = w;
			m_Height = h;
			m_ChannelOrder = o;

			m_Deleter->safeDelete( m_Data );
			safeDelete( m_Deleter );

			if ( data != nullptr )
			{
				m_Deleter = new ArrayDeleter< unsigned char >();
				m_Data = new unsigned char[ m_Size ];
				memcpy( m_Data, data, m_Size );
			}
			else
			{
				m_Data = nullptr;
				m_Deleter = new NullDeleter< unsigned char >();
			}
		}

		ImageType const&			getImageType() const { return m_ImageType; }
		ImageChannelOrder const&	getChannelOrder() const { return m_ChannelOrder; }
		unsigned int				getNumberOfChannels() const { return m_ChannelOrder.getNumberOfChannels(); }
		size_t						getByteSize() const { return m_Size; }
		size_t						getBitsPerPixel() const { return m_ImageType.getByteSize()*8; }
		unsigned int				getWidth() const { return m_Width; }
		unsigned int				getHeight() const { return m_Height; }
		unsigned char const*		getData() { return m_Data; }
		unsigned char*				getData() const { return m_Data; }

	private:

		unsigned char				*m_Data;
		unsigned int				m_Width;
		unsigned int				m_Height;
		size_t						m_Size;
		ImageType					m_ImageType;
		ImageChannelOrder			m_ChannelOrder;
		Deleter< unsigned char >	*m_Deleter;

	};

	inline std::ostream& operator<<( std::ostream& out, Image const& image )
	{
		return out;
	}

	inline std::istream& operator>>( std::istream& in, Image &image )
	{
		return in;
	}
}
