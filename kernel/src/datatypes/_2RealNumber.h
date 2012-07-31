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

#include <istream>
#include <ostream>

namespace _2Real
{
	class Number
	{

		friend std::istream & operator>>( std::istream &in, Number &number );
		friend std::ostream & operator<<( std::ostream &out, Number const& number );

	public:

		Number();
		explicit Number( const char c );
		explicit Number( const unsigned char c );
		explicit Number( const short s );
		explicit Number( const unsigned short s );
		explicit Number( const int i );
		explicit Number( const unsigned int i );
		explicit Number( const long l );
		explicit Number( const unsigned long l );
		explicit Number( const float f );
		explicit Number( const double d );

		Number( Number const& src );
		Number & operator=( Number const& src );
		bool operator==( Number const& rhs ) const;

		Number operator+( Number const& rhs ) const;
		Number operator-( Number const& rhs ) const;
		Number operator*( Number const& rhs ) const;
		Number operator/( Number const& rhs ) const;

		Number & operator+=( Number const& rhs );
		Number & operator-=( Number const& rhs );
		Number & operator*=( Number const& rhs );
		Number & operator/=( Number const& rhs );

		operator char() const;
		operator unsigned char() const;
		operator short() const;
		operator unsigned short() const;
		operator int() const;
		operator unsigned int() const;
		operator long() const;
		operator unsigned long() const;
		operator float() const;
		operator double() const;

	private:

		double			m_Val;

	};
}
