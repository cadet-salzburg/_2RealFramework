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

#include "_2RealException.h"

#include <sstream>
#include <stdint.h>
#include <memory>
#include <map>

namespace _2Real
{

	class AudioBuffer
	{

	public:

		/* these 4 functions need to be implemented */

		AudioBuffer();
		AudioBuffer( AudioBuffer const& src ); /* when this function is called, a deep copy of the data should be made */
		AudioBuffer& operator=( AudioBuffer const& src ); /* when this function is called, a deep copy of the data should be made */
		~AudioBuffer(); /* be careful of who owns the data when deleting */

		/* in addition, add anything else you need, i.e. getter and setter functions */

		/* these are going to be needed by the framework in the future, but they don't need to be implemented right now */
		friend std::ostream& operator<<( std::ostream& out, AudioBuffer const& buffer );
		friend std::istream& operator>>( std::istream& in, AudioBuffer &buffer );

	private:

		int						m_SampleRate;
		int						m_Channels;
		int						m_SamplesCount;
		long					m_SizeInBytes;
		int						m_BitResolution;
		double					m_Pts;
		unsigned char			*m_Data;

	};

	std::ostream& operator<<( std::ostream& out, AudioBuffer const& buffer )
	{
		/* TODO */
		out << " TODO ";
		return out;
	}

	std::istream& operator>>( std::istream& in, AudioBuffer &buffer )
	{
		/* TODO */
		return in;
	}

}