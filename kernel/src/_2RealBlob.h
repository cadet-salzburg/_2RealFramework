#pragma once

#include "_2RealException.h"

#include <sstream>

namespace _2Real
{

	DECLARE_EXCEPTION(BlobException, Exception);

	template< typename DataType >
	class Blob
	{

	public:

		Blob();
		Blob(Blob const& src);
		Blob& operator=(Blob const& src);

		void assign(DataType* data, unsigned int size);

		template< typename T >
		friend std::ostream& operator<<(std::ostream& out, typename Blob< T > const& blob);

		template< typename T >
		friend std::istream& operator>>(std::istream& in, Blob< T > &blob);

		const unsigned int size() const;

		DataType const *const data() const;
		const DataType operator[](unsigned int index) const;

	private:

		void copydata(DataType const*data, unsigned int size);

		DataType						const* m_Data;
		unsigned int					m_Size;
	};

	template< typename DataType >
	Blob< DataType >::Blob() :
		m_Size(0),
		m_Data(NULL)
	{
	}

	template< typename DataType >
	Blob< DataType >::Blob(Blob< DataType > const& src) :
		m_Size(0),
		m_Data(NULL)
	{
		copydata(src.m_Data, src.m_Size);
	}

	template< typename DataType >
	Blob< DataType >& Blob< DataType >::operator=(Blob< DataType > const& src)
	{
		if (this == &src)
		{
			return *this;
		}

		copydata(src.m_Data, src.m_Size);

		return *this;
	}

	template< typename DataType >
	void Blob< DataType >::assign(DataType* data, unsigned int size)
	{
		m_Size = size;
		m_Data = data;
	}

	template< typename T >
	std::ostream& operator<<(std::ostream& out, typename Blob< T > const& blob)
	{
		out << blob.m_Size << std::endl;
		return out;
	}

	template< typename T >
	std::istream& operator>>(std::istream& in, typename Blob< T > &blob)
	{
		in >> blob.m_Size;
		return in;
	}

	template< typename DataType >
	const unsigned int Blob< DataType >::size() const
	{
		return m_Size;
	}
	
	template< typename DataType >
	DataType const *const Blob< DataType >::data() const
	{
		return m_Data;
	}

	template< typename DataType >
	const DataType Blob< DataType >::operator[](unsigned int index) const
	{
		if (index >= m_Size)
		{
			std::stringstream txt;
			txt << "index " << index << " too high (Blob size: " << m_Size << ")";
			throw BlobException(txt.str());
		}

		return m_Data[index];
	}

}