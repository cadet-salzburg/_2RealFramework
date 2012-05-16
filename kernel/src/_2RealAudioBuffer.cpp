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

#include "_2RealAudioBuffer.h"

namespace _2Real
{

	AudioBuffer::AudioBuffer()
	{
	}

	AudioBuffer::AudioBuffer( AudioBuffer const& src ):m_SampleRate(src.m_SampleRate),
		m_Channels(src.m_Channels),
		m_SamplesCount(src.m_SamplesCount),
		m_Pts(src.m_Pts),
		m_SizeInBytes(src.m_SizeInBytes),
		m_BitResolution(src.m_BitResolution)
	{
		delete[] m_Data;
		if(src.m_Data)
		{
		m_Data = new unsigned char [m_SizeInBytes];
		memcpy(m_Data,src.m_Data,m_SizeInBytes);
		}
		else m_Data = NULL;
		
	}

	AudioBuffer& AudioBuffer::operator=( AudioBuffer const& src )
	{
		if(this == &src)
		return *this;
		
		m_SampleRate    = src.m_SampleRate;
		m_Channels      = src.m_Channels;
		m_SamplesCount  = src.m_SamplesCount;
		m_SizeInBytes   = src.m_SizeInBytes;
		m_BitResolution = src.m_BitResolution;
		m_Pts			= src.m_Pts;

		delete[] m_Data;
		
		if(src.m_Data)
		{
			m_Data = new unsigned char [m_SizeInBytes];
			memcpy(m_Data,src.m_Data,m_SizeInBytes);
		}
		else m_Data = NULL;


		return *this;
	}

	AudioBuffer::~AudioBuffer()
	{
	}

}