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

#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class ThreadingPolicy
	{

	public:

		enum Code
		{
			ANY_THREAD,
			SAME_THREAD,
			DEDICATED_THREAD,
			INVALID
		};

		ThreadingPolicy( const Code c ) : mCode( c ) {}

		ThreadingPolicy( std::string const& str ) :
			mCode( INVALID )
		{
			if ( str == "any_thread" )					mCode = ANY_THREAD;
			else if ( str == "same_thread" )			mCode = SAME_THREAD;
			else if ( str == "dedicated_thread" )		mCode = DEDICATED_THREAD;
			else										mCode = INVALID;
		}

		Code getCode() const { return mCode; }
		bool operator==( ThreadingPolicy const& other ) const { return mCode == other.mCode; }
		bool operator!=( ThreadingPolicy const& other ) const { return mCode != other.mCode; }

		operator std::string() const
		{
			if ( mCode == ANY_THREAD )					return "any_thread";
			else if ( mCode == SAME_THREAD )			return "same_thread";
			else if ( mCode == DEDICATED_THREAD )		return "dedicated_thread";
			else										return "invalid";
		}

	private:

		Code mCode;

	};
}