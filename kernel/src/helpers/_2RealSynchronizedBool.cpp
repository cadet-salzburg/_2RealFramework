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

#include "helpers/_2RealSynchronizedBool.h"

namespace _2Real
{

	SafeBool::SafeBool( const bool val ) :
		m_Bool( val )
	{
	}

	SafeBool::SafeBool( SafeBool const& src )
	{
	}

	SafeBool& SafeBool::operator=( SafeBool const& src )
	{
		return *this;
	}

	bool SafeBool::isSet() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		return ( m_Bool == true );
	}

	bool SafeBool::isUnset() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		return ( m_Bool == false );
	}

	void SafeBool::set()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_Bool = true;
	}

	void SafeBool::unset()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_Bool = false;
	}

}