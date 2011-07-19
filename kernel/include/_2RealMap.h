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

#include "_2RealFramework.h"

namespace _2Real
{
	template <unsigned int N>
	class _2RealMapResolution
	{
	public:

		unsigned int m_Resolution[N];

		_2RealMapResolution() { for (unsigned int i=0; i<N; i++) m_Resolution[i] = 0; }
		_2RealMapResolution(const _2RealMapResolution<N>& _resolution) { for (unsigned int i=0; i<N; i++) m_Resolution[i] = _resolution.m_Resolution[i]; }
	};

	template <>
	class _2RealMapResolution<2>
	{
	public:

		unsigned int m_Resolution[2];

		_2RealMapResolution(unsigned int _x, unsigned int _y) { m_Resolution[0] = _x; m_Resolution[1] = _y; };
		_2RealMapResolution() { for (unsigned int i=0; i<2; i++) m_Resolution[i] = 0; }
		_2RealMapResolution(const _2RealMapResolution<2>& _resolution) { for (unsigned int i=0; i<2; i++) m_Resolution[i] = _resolution.m_Resolution[i]; }
		const unsigned int& X() { return m_Resolution[0]; }
		const unsigned int& Y() { return m_Resolution[1]; }
	};

	typedef _2RealMapResolution<2> _2Real2DResolution;

	template <>
	class _2RealMapResolution<3>
	{
	public:

		unsigned int m_Resolution[3];

		_2RealMapResolution(unsigned int _x, unsigned int _y, unsigned int _z) { m_Resolution[0] = _x; m_Resolution[1] = _y; m_Resolution[2] = _z; };
		_2RealMapResolution() { for (unsigned int i=0; i<3; i++) m_Resolution[i] = 0; }
		_2RealMapResolution(const _2RealMapResolution<3>& _resolution) { for (unsigned int i=0; i<3; i++) m_Resolution[i] = _resolution.m_Resolution[i]; }
		const unsigned int& X() { return m_Resolution[0]; }
		const unsigned int& Y() { return m_Resolution[1]; }
		const unsigned int& Z() { return m_Resolution[2]; }
	};

	typedef _2RealMapResolution<3> _2Real3DResolution;

	template <typename T, unsigned int N>
	class _2RealMap
	{

	public:

		_2RealMap();
		_2RealMap(const T* _data, const _2RealMapResolution<N>& _resolution);
		~_2RealMap();
	
		T* data();
		void setData(T* _data);
		_2RealMapResolution<N> resolution() const;
		void setResolution(const _2RealMapResolution<N>& _resolution);

	private:
	
		T*						m_Data;
		_2RealMapResolution<N>	m_Resolution;
	};

	template<typename T, unsigned int N>
	_2RealMap<T, N>::_2RealMap() : m_Data(NULL)
	{
	}

	template<typename T, unsigned int N>
	_2RealMap<T, N>::_2RealMap(const T* _data, const _2RealMapResolution<N>& _resolution) : m_Data(_data), m_Resolution(_resolution)
	{
	}

	template<typename T, unsigned int N>
	_2RealMap<T, N>::~_2RealMap()
	{
		m_Data = NULL;
	}

	template<typename T, unsigned int N>
	void _2RealMap<T, N>::setData(T* _data)
	{
		m_Data = _data;
	}

	template<typename T, unsigned int N>
	T* _2RealMap<T, N>::data()
	{
		return m_Data;
	}

	template<typename T, unsigned int N>
	void _2RealMap<T, N>::setResolution(const _2RealMapResolution<N>& _resolution)
	{
		m_Resolution = _resolution;
	}

	template<typename T, unsigned int N>
	_2RealMapResolution<N> _2RealMap<T, N>::resolution() const
	{
		return m_Resolution;
	}
}

