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

#include "helpers/_2RealVectorInitializer.h"

#include <string>
#include <sstream>

namespace _2Real
{

	class DeviceInfo
	{

	friend std::ostream& operator<<( std::ostream& out, DeviceInfo const& info );
	friend std::istream& operator>>( std::istream& in, DeviceInfo &info );

	public:

		DeviceInfo() : m_DeviceId( "undefined" ), m_Description( "undefined" ), m_IsUsed( false ) {}
		DeviceInfo( std::string const& id, std::string const& desc, bool isUsed ) : m_DeviceId( id ), m_Description( desc ), m_IsUsed( isUsed ) {}

		std::string const& getDeviceId() const { return m_DeviceId; }
		std::string const& getDescription() const { return m_Description; }
		bool isUsed() const { return m_IsUsed; }

		bool setUsage( const bool isUsed ) { return (m_IsUsed = isUsed); }

	private:

		std::string				m_DeviceId;
		std::string				m_Description;
		bool					m_IsUsed;
	};

	inline std::ostream& operator<<( std::ostream& out, DeviceInfo const& info )
	{
		out << info.getDeviceId() << " " << info.getDescription() << " " << info.isUsed() << std::endl;
		return out;
	}

	inline std::istream& operator>>( std::istream& in, DeviceInfo &info )
	{
		in >> info.m_DeviceId >> info.m_Description >> info.m_IsUsed;
		return in;
	}

	typedef std::vector< DeviceInfo >						DeviceInfos;
	typedef std::vector< DeviceInfo >::iterator				DeviceInfoIterator;
	typedef std::vector< DeviceInfo >::const_iterator		DeviceInfoConstIterator;
	typedef VectorInitializer< DeviceInfo >					Devices;

}
