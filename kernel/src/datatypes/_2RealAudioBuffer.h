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

#include "helpers/_2RealException.h"

#include <memory>

namespace _2Real
{

	// TODO: the audio buffer is not entirely consistent with the image or imageT class with respect to some function names

	class AudioBuffer
	{

	public:

		AudioBuffer() :
			m_SampleRate( 0 ), m_ChannelCount( 0 ), m_SampleCount( 0 ), m_SizeInBytes( 0 ), m_BitResolution( 0 ),
			m_PresentationTimestamp( 0 ), m_Data( nullptr ), m_Deleter( new NullDeleter< unsigned char > )
		{
		}

		AudioBuffer( AudioBuffer const& src ) :
			m_SampleRate( src.m_SampleRate ), m_ChannelCount( src.m_ChannelCount ), m_SampleCount( src.m_SampleCount ),
			m_SizeInBytes( src.m_SizeInBytes ), m_BitResolution( src.m_BitResolution ), m_PresentationTimestamp( src.m_PresentationTimestamp )
		{
			makeCopy( src.m_Data, m_SizeInBytes );
		}

		AudioBuffer& operator=( AudioBuffer const& src )
		{
			if ( this == &src ) return *this;

			m_SampleRate = src.m_SampleRate;
			m_ChannelCount = src.m_ChannelCount;
			m_SampleCount = src.m_SampleCount;
			m_SizeInBytes = src.m_SizeInBytes;
			m_BitResolution= src.m_BitResolution;
			m_PresentationTimestamp = src.m_PresentationTimestamp;

			m_Deleter->safeDelete( m_Data );
			delete m_Deleter;

			makeCopy( src.m_Data, m_SizeInBytes );

			return *this;
		}

		AudioBuffer( unsigned char * data, bool owns, const long size, const unsigned int sampleRate, const unsigned int sampleCount,
			const unsigned int channelCount, const unsigned int bitResolution, double pts ) :
			m_SampleRate( sampleRate ), m_ChannelCount( channelCount ), m_SampleCount( sampleCount ),
			m_SizeInBytes( size ), m_BitResolution( bitResolution ), m_PresentationTimestamp( pts ), m_Data( data ),
			m_Deleter( owns ? static_cast< Deleter< unsigned char > * >( new ArrayDeleter< unsigned char > ) : new NullDeleter< unsigned char > )
		{
		}

		~AudioBuffer()
		{
			m_Deleter->safeDelete( m_Data );
			delete m_Deleter;
		}

		void assign( unsigned char * data, bool owns, const long size, const unsigned int sampleRate, const unsigned int sampleCount,
			const unsigned int channelCount, const unsigned int bitResolution, double pts )
		{
			m_SampleRate = sampleRate;
			m_ChannelCount = sampleCount;
			m_SampleCount = channelCount;
			m_SizeInBytes = size;
			m_BitResolution= bitResolution;
			m_PresentationTimestamp = pts;

			m_Deleter->safeDelete( m_Data );
			delete m_Deleter;

			makeCopy( data, m_SizeInBytes );
		}

		unsigned int getSampleRate() const			{ return m_SampleRate; }
		unsigned int getSampleCount() const			{ return m_SampleCount; }
		unsigned int getChannelCount() const		{ return m_ChannelCount; }
		long getSizeInBytes() const					{ return m_SizeInBytes; }
		unsigned int getBitResolution() const		{ return m_BitResolution; }
		double getPresentationTimestamp() const		{ return m_PresentationTimestamp; }
		unsigned char const* getData() const	    { return m_Data; }  	// NOTE: removed "const" before & to eliminate warnings. Didn't occur necessary to me (ottona)
		unsigned char *& getData()      			{ return m_Data; }

		bool operator==( AudioBuffer const& src ) const
		{
			if ( m_SizeInBytes == src.m_SizeInBytes && m_SampleRate == src.m_SampleRate && m_ChannelCount == src.m_ChannelCount && m_SampleCount == src.m_SampleCount
				&& m_BitResolution == src.m_BitResolution && m_PresentationTimestamp == src.m_PresentationTimestamp ) return true;
			else
			{
				unsigned char *p1 = m_Data;
				unsigned char *p2 = src.m_Data;
				for ( unsigned int i=0; i<m_SizeInBytes; ++i )
				{
					if ( *p1 != *p2 ) return false;
					++p1; ++p2;
				}
			}
			return true;
		}

	private:

		void makeCopy( unsigned char const*const data, const unsigned int sz )
		{
			if ( data != nullptr )
			{
				m_Deleter = new ArrayDeleter< unsigned char >;
				m_Data = new unsigned char[ sz ];
				memcpy( m_Data, data, sz );
			}
			else
			{
				m_Data = nullptr;
				m_Deleter = new NullDeleter< unsigned char >;
			}
		}

		template< typename TData >
		struct Deleter
		{
			virtual void safeDelete( TData *& data) const = 0;
			virtual ~Deleter() {}
		};

		template< typename TData >
		struct NullDeleter : public Deleter< TData >
		{
			void safeDelete( TData *& data) const {}
		};

		template< typename TData >
		struct ArrayDeleter : public Deleter< TData >
		{
			void safeDelete( TData *& data) const { delete [] data; data = nullptr; }
		};

		unsigned int				m_SampleRate;
		unsigned int				m_ChannelCount;
		unsigned int				m_SampleCount;
		long						m_SizeInBytes;
		unsigned int				m_BitResolution;
		double						m_PresentationTimestamp;
		unsigned char				*m_Data;
		Deleter< unsigned char >	*m_Deleter;

	};
}
