#pragma once

#include <vector>

template <typename T, unsigned int N>
class Vec
{

public:

	Vec();
	Vec(const Vec<T, N>& _src);
	Vec<T, N>& operator= (const Vec<T, N>& _src);

	const T prod() const;
	const T sum() const;
	const unsigned int size() const;
	void set(unsigned int _i, T _val);
	const T get(unsigned int _i) const;

private:

	std::vector<T> m_Values;

};

template <typename T, unsigned int N>
Vec<T, N>::Vec()
{
	m_Values.resize(N);
}
	
template <typename T, unsigned int N>
Vec<T, N>::Vec(const Vec<T, N>& _src)
{
	m_Values = _src.m_Values;
}
	
template <typename T, unsigned int N>
Vec<T, N>& Vec<T,N>::operator= (const Vec<T, N>& _src)
{
	if (this == &_src)
	{
		return *this;
	}
		
	m_Values = _src.m_Values;

	return *this;
}

template <typename T, unsigned int N>
const T Vec<T, N>::prod() const
{
	T result = m_Values[0];
	for (unsigned int i=1; i<N; i++)
	{
		result *= m_Values[i];
	}

	return result;
}

template <typename T, unsigned int N>
const T Vec<T, N>::sum() const
{
	T result = m_Values[0];
	for (unsigned int i=1; i<N; i++)
	{
		result *= m_Values[i];
	}

	return result;
}

template <typename T, unsigned int N>
const unsigned int Vec<T, N>::size() const
{
	return m_Values.size();
}

template <typename T, unsigned int N>
void  Vec<T, N>::set(unsigned int _i, T _val)
{
	if (_i < m_Values.size())
	{
		m_Values[_i] = _val;
	}
}

template <typename T, unsigned int N>
const T Vec<T, N>::get(unsigned int _i) const
{
	if (_i < m_Values.size())
	{
		return m_Values[_i];
	}

	return T();
}


template <typename T, unsigned int N>
class Image
{
	public:

		typedef Vec<unsigned int, N> Resolution;

		Image();
		Image(T* _data, Resolution& _res);
		Image(const Image<T, N>& _src);
		Image& operator= (const Image<T, N>& _src);
		~Image();
	
		const T* data() const;
		void setData(T* _data);
		const Resolution resolution() const;
		void setResolution(const Resolution& _res);

	private:
	
		T*					m_Data;
		Resolution			m_Resolution;
	};

template<typename T, unsigned int N>
Image<T, N>::Image() : m_Data(NULL)
{
}

template<typename T, unsigned int N>
Image<T, N>::Image(T* _data, Resolution& _res) : m_Data(_data), m_Resolution(_res)
{
}

template<typename T, unsigned int N>
Image<T, N>::Image(const Image<T, N>& _src) : m_Data(NULL)
{
	m_Resolution = _src.m_Resolution;
	unsigned int sz = m_Resolution.prod();
	
	if (sz > 0)
	{
		m_Data = new T[sz];
		memcpy(m_Data, _src.m_Data, sz);
	}
}

template<typename T, unsigned int N>
Image<T, N>& Image<T, N>::operator= (const Image<T, N>& _src)
{
	if (this == &_src)
	{
		return *this;
	}

	m_Resolution = _src.m_Resolution;
	unsigned int sz = m_Resolution.prod();
	
	if (sz > 0)
	{
		delete m_Data;
		m_Data = new T[sz];
		memcpy(m_Data, _src.m_Data, sz);
	}

	return *this;
}

template<typename T, unsigned int N>
Image<T, N>::~Image()
{
	delete m_Data;
	m_Data = NULL;
}

template<typename T, unsigned int N>
void Image<T, N>::setData(T* _data)
{
	delete m_Data;
	m_Data = _data;
}

template<typename T, unsigned int N>
const T* Image<T, N>::data() const
{
	return m_Data;
}

template<typename T, unsigned int N>
void Image<T, N>::setResolution(const Resolution& _res)
{
	m_Resolution = _res;
}

template<typename T, unsigned int N>
const Vec<unsigned int, N> Image<T, N>::resolution() const
{
	return m_Resolution;
}