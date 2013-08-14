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
	class UpdatePolicy
	{

	public:

		enum Code
		{
			VALID_DATA,
			NEWER_DATA,
			INVALID
		};

		UpdatePolicy( Code const& c ) : mCode( c ) {}

		UpdatePolicy( std::string const& str ) :
			mCode( INVALID )
		{
			if ( str == "ValidData" )				mCode = VALID_DATA;
			else if ( str == "NewerData" )			mCode = NEWER_DATA;
			else									mCode = INVALID;
		}

		Code getCode() const { return mCode; }

		operator std::string() const
		{
			if ( mCode == VALID_DATA )				return "ValidData";
			else if ( mCode == NEWER_DATA )			return "NewerData";
			else									return "invalid";
		}

		bool operator==( UpdatePolicy const& other ) const { return mCode == other.mCode; }
		bool operator!=( UpdatePolicy const& other ) const { return mCode != other.mCode; }

	private:

		Code		mCode;

	};
}