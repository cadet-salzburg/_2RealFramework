#pragma once

#include "_2RealException.h"

#include <sstream>

namespace _2Real
{

	DECLARE_EXCEPTION(BlobException, Exception);

	template< typename Datatype >
	class Blob
	{

	public:

		Blob();
		Blob(Blob const& src);
		Blob& operator=(Blob const& src);

		void assign(Datatype* data, unsigned int size);

		template< typename T >
		friend std::ostream& operator<<(std::ostream& out, typename Blob< T > const& blob);

		template< typename T >
		friend std::istream& operator>>(std::istream& in, Blob< T > &blob);

		const unsigned int size() const;

		Datatype const *const data() const;
		const Datatype operator[](unsigned int index) const;

	private:

		void copydata(Datatype const*data, unsigned int size);

		Datatype						const* m_Data;
		unsigned int					m_Size;
	};

	template< typename Datatype >
	Blob< Datatype >::Blob() :
		m_Size(0),
		m_Data(nullptr)
	{
	}

	template< typename Datatype >
	Blob< Datatype >::Blob(Blob< Datatype > const& src) :
		m_Size(0),
		m_Data(nullptr)
	{
		copydata(src.m_Data, src.m_Size);
	}

	template< typename Datatype >
	Blob< Datatype >& Blob< Datatype >::operator=(Blob< Datatype > const& src)
	{
		if (this == &src)
		{
			return *this;
		}

		copydata(src.m_Data, src.m_Size);

		return *this;
	}

	template< typename Datatype >
	void Blob< Datatype >::assign(Datatype* data, unsigned int size)
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

	template< typename Datatype >
	const unsigned int Blob< Datatype >::size() const
	{
		return m_Size;
	}
	
	template< typename Datatype >
	Datatype const *const Blob< Datatype >::data() const
	{
		return m_Data;
	}

	template< typename Datatype >
	const Datatype Blob< Datatype >::operator[](unsigned int index) const
	{
		if (index >= m_Size)
		{
			std::stringstream txt;
			txt << "index " << index << " too high (Blob size: " << m_Size << ")";
			throw BlobException(txt.str());
		}

		return m_Data[index];
	}

	template< typename Datatype >
	void Blob< Datatype >::copydata(Datatype const*data, unsigned int size)
	{
		m_Size = size;
		delete[] m_Data;
		m_Data = nullptr;
		if (m_Size > 0)
		{
			m_Data = new Datatype[m_Size];
			if (m_Data == nullptr)
			{
				std::stringstream txt;
				txt << "memory allocation for " << m_Size << " bytes failed!";
				m_Size = 0;
				throw BlobException(txt.str());
			}
			m_Data = (Datatype const*)memcpy((void*)m_Data, data, m_Size);
		}
	}
}