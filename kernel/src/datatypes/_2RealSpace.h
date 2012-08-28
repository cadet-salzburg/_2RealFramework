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

#include "datatypes/_2RealVector.h"

#include <istream>
#include <ostream>
#include <iostream>
#include <string>

namespace _2Real
{
	template<typename T, std::size_t d>
	class Space
	{
	public:
		enum
		{
			Dimensions = d
		};

		typedef Space<T,d>	_Myt;

		typedef T								value_type;
		typedef Eigen::Matrix<T, d, 1, 0, d, 1>	vector_type;

	public:
		Space() :
			p0( vector_type::Zero() ), p1( vector_type::Zero() )
		{}

		Space( const vector_type &p0, const vector_type &p1 ) :
			p0( p0 ), p1( p1 )
		{}

		Space( const Space &rhs ) :
			p0( rhs.p0 ), p1( rhs.p1 )
		{}

		void set( const vector_type &p0, const vector_type &p1 )
		{
			this->p0 = p0;
			this->p1 = p1;
		}

		vector_type &getP0()					{	return this->p0;	}
		const vector_type &getP0() const		{	return this->p0;	}

		vector_type &getP1()					{	return this->p1;	}
		const vector_type &getP1() const		{	return this->p1;	}

		vector_type extends() const
		{
			return vector_type( this->p1 - this->p0 );
		}

		//area (2D), volume (3D), hypervolume (4D)...
		T size() const
		{
			vector_type e( this->extends() );
			return std::abs( std::accumulate( e.begin(), e.end(), 1, std::multiplies<T>() ) );
		}

		Space<T,d> &operator = ( Space<T,d> const& rhs )
		{
			this->p0 = rhs.p0;
			this->p1 = rhs.p1;
			return *this;
		}

		bool operator == ( const Space<T,d> &rhs ) const
		{
			return ( this->p0 == rhs.p0 && this->p1 == rhs.p1 );
		}

		bool operator != ( const Space<T,d> &rhs ) const
		{
			return !( *this == rhs );
		}

		//TODO: implement relational operators
		//bool operator < () const;
		//bool operator > () const;
		//bool operator <= () const;
		//bool operator >= () const;

		//TODO: implement boolean operations
		//Space<T,d> operator &= ( const Space<T,d> &rhs ) const;
		//Space<T,d> friend operator & ( const Space<T,d> &lhs, const Space<T,d> &rhs );
		//Space<T,d> operator |= ( const Space<T,d> &rhs ) const;
		//Space<T,d> friend operator | ( const Space<T,d> &lhs, const Space<T,d> &rhs );
		//Space<T,d> operator ^= ( const Space<T,d> &rhs ) const;
		//Space<T,d> friend operator ^ ( const Space<T,d> &lhs, const Space<T,d> &rhs );


	private:

		vector_type	p0;
		vector_type	p1;
	};

#ifdef _2REAL_BLAS_DOUBLE_PRECISION
	typedef Space<double,2>	Space2D;		//aka area
	typedef Space<double,3>	Space3D;		//aka volume
	typedef Space<double,4>	Space4D;		//aka hyperspace
#else
	typedef Space<float,2>	Space2D;		//aka area
	typedef Space<float,3>	Space3D;		//aka volume
	typedef Space<float,4>	Space4D;		//aka hyperspace
#endif
}
