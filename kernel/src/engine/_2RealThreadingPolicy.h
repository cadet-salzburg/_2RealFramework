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

#include <string>

namespace _2Real
{
	class ThreadingPolicy
	{

	public:

		enum Policy
		{
			ANY_THREAD,
			SAME_THREAD,
			DEDICATED_THREAD,
			INVALID
		};

		ThreadingPolicy( Policy const& p ) : m_Policy( p ) {}
		Policy getPolicy() const { return m_Policy; }
		bool operator==( ThreadingPolicy const& rhs ) const { return m_Policy == rhs.m_Policy; }
		bool operator!=( ThreadingPolicy const& rhs ) const { return m_Policy != rhs.m_Policy; }

		static const std::string getPolicyAsString( Policy const& p )
		{
			if ( p == ANY_THREAD )					return "any_thread";
			else if ( p == SAME_THREAD )			return "same_thread";
			else if ( p == DEDICATED_THREAD )		return "dedicated_thread";
			else									return "invalid";
		}

		static const Policy getPolicyFromString( std::string const& p )
		{
			if ( p == "any_thread" )				return ANY_THREAD;
			else if ( p == "same_thread" )			return SAME_THREAD;
			else if ( p == "dedicated_thread" )		return DEDICATED_THREAD;
			else									return INVALID;
		}

	private:

		Policy		m_Policy;

	};
}