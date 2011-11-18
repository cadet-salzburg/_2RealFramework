#pragma once

#include "_2RealException.h"

#include "Poco\SharedPtr.h"

#include <iostream>
#include <sstream>
#include <vector>

/**
*
*/

namespace _2Real
{

	class PixelbufferException;

	/**
	*	convenience class for storing images
	*	this class can not be sent by framework, but can be copied into a pixelbuffer
	*	which then takes ownership of the data
	*/
	template< typename T >
	class Imagebuffer
	{

	public:

		Imagebuffer(unsigned int const& width, unsigned int const& height, unsigned int const& channels) :
			m_Width(width),
			m_Height(height),
			m_Channels(channels),
			m_BytesPerPixel(sizeof(T)*channels),
			m_Size(width*height*channels),
			m_ByteSize(m_Size*sizeof(T))
		{
			//m_Data = Poco::SharedPtr< T >(new T[m_Size]);
			m_Data.resize(m_Size);
		}

		~Imagebuffer()
		{
		}

		void set(T const& val)
		{
			std::fill(&m_Data[0], &m_Data[m_Size], val);
		}

		unsigned int const& width() const
		{
			return m_Width;
		}

		unsigned int const& height() const
		{
			return m_Height;
		}

		unsigned int const& bytesPerPixel() const
		{
			return m_BytesPerPixel;
		}

		unsigned int const& channels() const
		{
			return m_Channels;
		}

		unsigned int const& size() const
		{
			return m_Size;
		}

		unsigned int const& byteSize() const
		{
			return m_ByteSize;
		}

		T const& operator() (unsigned int const& row, unsigned int const& col, unsigned int const& channel) const
		{
			if (row >= m_Height)
			{
				std::stringstream txt;
				txt << "row index " << row << " too high (imagebuffer height: " << m_Height << ")";
				throw ImagebufferException(txt.str());
			}
			else if (col >= m_Width)
			{
				std::stringstream txt;
				txt << "col index " << col << " too high (imagebuffer width: " << m_Width << ")";
				throw ImagebufferException(txt.str());
			}
			else if (channel >= m_Channels)
			{
				std::stringstream txt;
				txt << "channel index " << channel << " too high (imagebuffer has " << m_Height << " channels)";
				throw ImagebufferException(txt.str());
			}

			unsigned int index = row * m_Width * m_Channels + col * m_Channels + channel;
			//return m_Data.get()[index];
			return m_Data[index];
		}

		T& operator() (unsigned int const& row, unsigned int const& col, unsigned int const& channel)
		{
			if (row >= m_Height)
			{
				std::stringstream txt;
				txt << "row index " << row << " too high (imagebuffer height: " << m_Height << ")";
				throw ImagebufferException(txt.str());
			}
			else if (col >= m_Width)
			{
				std::stringstream txt;
				txt << "col index " << col << " too high (imagebuffer width: " << m_Width << ")";
				throw ImagebufferException(txt.str());
			}
			else if (channel >= m_Channels)
			{
				std::stringstream txt;
				txt << "channel index " << channel << " too high (imagebuffer has " << m_Height << " channels)";
				throw ImagebufferException(txt.str());
			}

			unsigned int index = row * m_Width * m_Channels + col * m_Channels + channel;
			//return m_Data.get()[index];
			return m_Data[index];
		}

		//Imagebuffer< DataType >::iterator

	private:

		template< typename T >
		friend class Pixelbuffer;

		template< typename T >
		friend class Iterator;

		//Poco::SharedPtr< T > data() const
		//{
		//	return m_Data[0];
		//}

		T const* const rawData() const
		{
			return &m_Data[0];
		}

		//class Iterator
		//{

		//public:

		//	Iterator(Imagebuffer< T > *buffer) :
		//		m_Buffer(buffer),
		//		m_Ptr(buffer->rawData())
		//	{
		//	}

		//	operator++

		//private:

		//	T							*m_Ptr;
		//	Imagebuffer< T >			m_Buffer;

		//};

		unsigned int					m_Width;
		unsigned int					m_Height;
		unsigned int					m_Channels;
		unsigned int					m_Size;
		unsigned int					m_ByteSize;
		unsigned int					m_BytesPerPixel;
		//Poco::SharedPtr< T >			m_Data;
		std::vector< T >				m_Data;

	};

	class ImagebufferException : public Exception
	{

		template< typename T >
		friend class Imagebuffer;

		ImagebufferException(std::string const& message) :
			Exception(message)
		{
		}

	};

	typedef Imagebuffer< float >			Image2D_float;
	typedef Imagebuffer< double >			Image2D_double;
	typedef Imagebuffer< unsigned char >	Image2D_uchar;
	typedef Imagebuffer< unsigned int >		Image2D_uint;

	template< typename T >
	class Pixelbuffer
	{

	public:

		Pixelbuffer() :
			m_Width(0),
			m_Height(0),
			m_Channels(0),
			m_BytesPerPixel(0),
			m_Size(0),
			m_ByteSize(0),
			m_Data(NULL),
			m_OwnedData(NULL),
			m_IsSharedData(true)
		{
		}

		Pixelbuffer(Pixelbuffer const& src) :
			m_Width(src.m_Width),
			m_Height(src.m_Height),
			m_Channels(src.m_Channels),
			m_BytesPerPixel(src.m_BytesPerPixel),
			m_Size(src.m_Size),
			m_ByteSize(src.m_ByteSize),
			m_Data(src.m_Data),
			m_OwnedData(src.m_OwnedData),
			m_IsSharedData(src.m_IsSharedData)
		{
		}

		Pixelbuffer(T *data, unsigned int const& width, unsigned int const& height, unsigned int const& channels, bool const& owns) :
			m_Width(with),
			m_Height(height),
			m_Channels(channels),
			m_BytesPerPixel(sizeof(T)*channels),
			m_Size(width*height*channels),
			m_ByteSize(sizeof(T)*m_Size),
			m_IsSharedData(owns)
		{
			if (owns)
			{
				m_OwnedData.assign(data);
				m_Data = NULL;
			}
			else
			{
				m_OwnedData.assign(NULL);
				m_Data = data;
			}
		}

		Pixelbuffer& operator=(Pixelbuffer const& src)
		{
			m_Width = src.m_Width;
			m_Height = src.m_Height;
			m_Channels = src.m_Channels;
			m_BytesPerPixel = src.m_BytesPerPixel;
			m_Size = src.m_Size;
			m_ByteSize = src.m_ByteSize;
			m_Data = src.m_Data;
			m_IsSharedData = src.m_IsSharedData;
			m_OwnedData = src.m_OwnedData;

			return *this;
		}

		/*
		*	copies the imagebuffer's contents into the pixelbuffer
		*/
		void copy(Imagebuffer< T > const& src)
		{
			m_Width = src.width();
			m_Height = src.height();
			m_Size = src.size();
			m_BytesPerPixel = src.bytesPerPixel();
			m_ByteSize = src.byteSize();
			m_Channels = src.channels();
			m_IsSharedData = true;
			m_Data = NULL;
			m_OwnedData.assign(NULL);

			T* tmp = new T[m_Size];
			memcpy(tmp, src.rawData(), m_ByteSize);
			m_OwnedData.assign(tmp);
		}

		~Pixelbuffer()
		{
		}

		void assign(T* data, unsigned int const& width, unsigned int const& height, unsigned int const& channels, bool const& owns)
		{
			m_Width = width;
			m_Height = height;
			m_Channels = channels;
			m_BytesPerPixel = sizeof(T)*channels;
			m_Size = width*height*channels;
			m_ByteSize = m_BytesPerPixel*width*height;
			m_IsSharedData = owns;
			if (owns)
			{
				m_OwnedData.assign(data);
				m_Data = NULL;
			}
			else
			{
				m_OwnedData.assign(NULL);
				m_Data = data;
			}
		}

		void assign(Poco::SharedPtr< T > data, unsigned int const& width, unsigned int const& height, unsigned int const& channels)
		{
			m_Width = width;
			m_Height = height;
			m_Channels = channels;
			m_BytesPerPixel = sizeof(T)*channels;
			m_Size = width*height*channels;
			m_ByteSize = m_BytesPerPixel*width*height;
			m_IsSharedData = true;
			m_OwnedData.assign(data);
			m_Data = NULL;
		}

		unsigned int const& width() const
		{
			return m_Width;
		}

		unsigned int const& height() const
		{
			return m_Height;
		}

		unsigned int const& bytesPerPixel() const
		{
			return m_BytesPerPixel;
		}

		unsigned int const& channels() const
		{
			return m_Channels;
		}

		unsigned int const& size() const
		{
			return m_Size;
		}

		unsigned int const& byteSize() const
		{
			return m_ByteSize;
		}

		T const *const rawData() const
		{
			if (m_IsSharedData)
			{
				return m_OwnedData.get();
			}
			else
			{
				return m_Data;
			}
		}

		T const& operator() (unsigned int const& row, unsigned int const& col, unsigned int const& channel) const
		{
			if (row >= m_Height)
			{
				std::stringstream txt;
				txt << "row index " << row << " too high (pixelbuffer height: " << m_Height << ")";
				throw PixelbufferException(txt.str());
			}
			else if (col >= m_Width)
			{
				std::stringstream txt;
				txt << "col index " << col << " too high (pixelbuffer width: " << m_Width << ")";
				throw PixelbufferException(txt.str());
			}
			else if (channel >= m_Channels)
			{
				std::stringstream txt;
				txt << "channel index " << channel << " too high (pixelbuffer has " << m_Height << " channels)";
				throw PixelbufferException(txt.str());
			}

			unsigned int index = row * m_Width * m_Channels + col * m_Channels + channel;
			if (m_IsSharedData)
			{
				return m_OwnedData.get()[index];
			}
			else
			{
				return m_Data[index];
			}
		}

	private:

		bool							m_IsSharedData;
		Poco::SharedPtr<  T >			m_OwnedData;
		T								const* m_Data;
		unsigned int					m_Width;
		unsigned int					m_Height;
		unsigned int					m_Channels;
		unsigned int					m_Size;
		unsigned int					m_ByteSize;
		unsigned int					m_BytesPerPixel;

	};

	class PixelbufferException : public Exception
	{

		template< typename T >
		friend class Pixelbuffer;

		PixelbufferException(std::string const& message) :
			Exception(message)
		{
		}

	};

	typedef Pixelbuffer< float >			Buffer2D_float;
	typedef Pixelbuffer< double >			Buffer2D_double;
	typedef Pixelbuffer< unsigned char >	Buffer2D_uchar;
	typedef Pixelbuffer< unsigned int >		Buffer2D_uint;

}