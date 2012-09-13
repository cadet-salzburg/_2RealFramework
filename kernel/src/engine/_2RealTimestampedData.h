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

#include "helpers/_2RealAny.h"

namespace _2Real
{

	class TimestampedData
	{

	public:

		TimestampedData() : anyValue(), timestamp( -1 ), key( -1 ) {}
		TimestampedData( Any const& a ) : anyValue( a ), timestamp( -1 ), key( -1 ) {}
		TimestampedData( Any const& a, const long t ) : anyValue( a ), timestamp( t ), key( -1 ) {}
		TimestampedData( Any const& a, const long t, const long k ) : anyValue( a ), timestamp( t ), key( k ) {}

		void cloneAnyFrom( TimestampedData const& src ) { anyValue.cloneFrom( src.anyValue ); }
		void createAnyFrom( TimestampedData const& src ) { anyValue.createNew( src.anyValue ); }
		bool operator>( TimestampedData const& other ) const { return timestamp > other.timestamp; }

		Any		anyValue;
		long	timestamp;
		long	key;

	};

}