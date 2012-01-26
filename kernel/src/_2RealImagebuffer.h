#pragma once

#include "_2RealException.h"

#include "Poco/SharedPtr.h"

#include <sstream>

/**
*	this class is still experimental, but i can be used to safely transfer an image out of the kinect test plugin
*	note that this class really just wraps an array - because you can't send pointers through the framework
*	use it like this: for an input slot, if you wish to iterate over the elemets, use the rawData() function
*	for an output slot, if you wish to write an image, create an array of the desired datatype and use assign()
*/

namespace _2Real
{

	DECLARE_EXCEPTION(PixelbufferException, Exception);

	template< typename DataType >
	class Pixelbuffer
	{

	public:

		Pixelbuffer();
		Pixelbuffer(Pixelbuffer const& src);
		Pixelbuffer& operator=(Pixelbuffer const& src);

		void assign(DataType* data, unsigned int width, unsigned int height, unsigned int channels, bool owns);
		void assign(Poco::SharedPtr< DataType > data, unsigned int width, unsigned int height, unsigned int channels);

		template< typename T >
		friend std::ostream& operator<<(std::ostream& out, typename Pixelbuffer< T > const& pb);

		template< typename T >
		friend std::istream& operator>>(std::istream& in, Pixelbuffer< T > &pb);

		const unsigned int width() const;
		const unsigned int height() const;
		const unsigned int bytesPerPixel() const;
		const unsigned int channels() const;
		const unsigned int size() const;
		const unsigned int byteSize() const;

		DataType const *const rawData() const;
		const DataType operator()(unsigned int row, unsigned int col, unsigned int channel) const;
		const DataType operator[](unsigned int index) const;

	private:

		bool							m_IsOwnedData;
		Poco::SharedPtr< DataType >		m_OwnedData;
		DataType						const* m_Data;
		unsigned int					m_Width;
		unsigned int					m_Height;
		unsigned int					m_Channels;
		unsigned int					m_Size;
		unsigned int					m_ByteSize;
		unsigned int					m_BytesPerPixel;

	};

	template< typename DataType >
	Pixelbuffer< DataType >::Pixelbuffer() :
		m_Width(0),
		m_Height(0),
		m_Channels(0),
		m_BytesPerPixel(0),
		m_Size(0),
		m_ByteSize(0),
		m_Data(NULL),
		m_OwnedData(NULL),
		m_IsOwnedData(true)
	{
	}

	template< typename DataType >
	Pixelbuffer< DataType >::Pixelbuffer(Pixelbuffer< DataType > const& src) :
		m_Width(src.m_Width),
		m_Height(src.m_Height),
		m_Channels(src.m_Channels),
		m_BytesPerPixel(src.m_BytesPerPixel),
		m_Size(src.m_Size),
		m_ByteSize(src.m_ByteSize),
		m_Data(src.m_Data),
		m_OwnedData(src.m_OwnedData),
		m_IsOwnedData(src.m_IsOwnedData)
	{
	}

	template< typename DataType >
	Pixelbuffer< DataType >& Pixelbuffer< DataType >::operator=(Pixelbuffer< DataType > const& src)
	{
		if (this == &src)
		{
			return *this;
		}

		m_Width = src.m_Width;
		m_Height = src.m_Height;
		m_Channels = src.m_Channels;
		m_BytesPerPixel = src.m_BytesPerPixel;
		m_Size = src.m_Size;
		m_ByteSize = src.m_ByteSize;
		m_Data = src.m_Data;
		m_OwnedData = src.m_OwnedData;
		m_IsOwnedData = src.m_IsOwnedData;

		return *this;
	}

	template< typename DataType >
	void Pixelbuffer< DataType >::assign(DataType *data, unsigned int width, unsigned int height, unsigned int channels, bool owns)
	{
		m_Width = width;
		m_Height = height;
		m_Channels = channels;
		m_BytesPerPixel = sizeof(DataType)*channels;
		m_Size = width*height*channels;
		m_ByteSize = m_BytesPerPixel*width*height;
		m_IsOwnedData = owns;

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

	template< typename DataType >
	void Pixelbuffer< DataType >::assign(Poco::SharedPtr< DataType > data, unsigned int width, unsigned int height, unsigned int channels)
	{
		m_Width = width;
		m_Height = height;
		m_Channels = channels;
		m_BytesPerPixel = sizeof(DataType)*channels;
		m_Size = width*height*channels;
		m_ByteSize = m_BytesPerPixel*width*height;
		m_IsOwnedData = true;
		m_OwnedData.assign(data);
		m_Data = NULL;
	}

	template< typename T >
	std::ostream& operator<<(std::ostream& out, typename Pixelbuffer< T > const& pb)
	{
		out << pb.m_Width << " " << pb.m_Height << " " << pb.m_Channels << std::endl;
		return out;
	}

	template< typename T >
	std::istream& operator>>(std::istream& in, typename Pixelbuffer< T > &pb)
	{
		char ws;
		in >> pb.m_Width >> ws >> pb.m_Height >> ws >> pb.m_Channels;
		return in;
	}

	template< typename DataType >
	const unsigned int Pixelbuffer< DataType >::width() const
	{
		return m_Width;
	}

	template< typename DataType >
	const unsigned int Pixelbuffer< DataType >::height() const
	{
		return m_Height;
	}

	template< typename DataType >
	const unsigned int Pixelbuffer< DataType >::bytesPerPixel() const
	{
		return m_BytesPerPixel;
	}

	template< typename DataType >
	const unsigned int Pixelbuffer< DataType >::channels() const
	{
		return m_Channels;
	}

	template< typename DataType >
	const unsigned int Pixelbuffer< DataType >::size() const
	{
		return m_Size;
	}

	template< typename DataType >
	const unsigned int Pixelbuffer< DataType >::byteSize() const
	{
		return m_ByteSize;
	}

	template< typename DataType >
	DataType const *const Pixelbuffer< DataType >::rawData() const
	{
		if (m_IsOwnedData)
		{
			return m_OwnedData.get();
		}
		else
		{
			return m_Data;
		}
	}

	template< typename DataType >
	const DataType Pixelbuffer< DataType >::operator() (unsigned int row, unsigned int col, unsigned int channel) const
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

		if (m_IsOwnedData)
		{
			return m_OwnedData.get()[index];
		}
		else
		{
			return m_Data[index];
		}
	}

	template< typename DataType >
	const DataType Pixelbuffer< DataType >::operator[](unsigned int index) const
	{
		if (index >= m_Size)
		{
			std::stringstream txt;
			txt << "index " << index << " too high (pixelbuffer size: " << m_Size << ")";
			throw PixelbufferException(txt.str());
		}

		if (m_IsOwnedData)
		{
			return m_OwnedData.get()[index];
		}
		else
		{
			return m_Data[index];
		}
	}

	typedef Pixelbuffer< float >			Buffer2D_float;
	typedef Pixelbuffer< double >			Buffer2D_double;
	typedef Pixelbuffer< unsigned char >	Buffer2D_uchar;
	typedef Pixelbuffer< unsigned int >		Buffer2D_uint;

}