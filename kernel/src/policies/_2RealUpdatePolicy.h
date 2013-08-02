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
			ALWAYS,
			OR_NEWER_DATA,
			AND_NEWER_DATA,
			INVALID
		};

		UpdatePolicy( Code const& c ) : mCode( c ) {}

		UpdatePolicy( std::string const& str ) :
			mCode( INVALID )
		{
			if ( str == "valid_data" )				mCode = ALWAYS;
			else if ( str == "or_newer_data" )		mCode = OR_NEWER_DATA;
			else if ( str == "and_newer_data" )		mCode = AND_NEWER_DATA;
			else									mCode = INVALID;
		}

		Code getCode() const { return mCode; }

		operator std::string() const
		{
			if ( mCode == ALWAYS )					return "valid_data";
			else if ( mCode == OR_NEWER_DATA )		return "or_newer_data";
			else if ( mCode == AND_NEWER_DATA )		return "and_newer_data";
			else									return "invalid";
		}

		bool operator==( UpdatePolicy const& other ) const { return mCode == other.mCode; }
		bool operator!=( UpdatePolicy const& other ) const { return mCode != other.mCode; }

		//static const std::string getPolicyAsString( const Code p )
		//{
		//	if ( p == ALWAYS )					return "valid_data";
		//	else if ( p == OR_NEWER_DATA )		return "or_newer_data";
		//	else if ( p == AND_NEWER_DATA )		return "and_newer_data";
		//	else								return "invalid";
		//}

		//static const Code getPolicyFromString( std::string const& p )
		//{
		//	if ( p == "valid_data" )			return ALWAYS;
		//	else if ( p == "or_newer_data" )	return OR_NEWER_DATA;
		//	else if ( p == "and_newer_data" )	return AND_NEWER_DATA;
		//	else								return INVALID;
		//}

	private:

		Code		mCode;

	};
}