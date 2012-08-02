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

#include "datatypes/_2RealNumber.h"

#include <istream>
#include <ostream>
#include <string>

#include <numeric>
#include <algorithm>
#include <functional>

namespace _2Real
{
	//TODO: move this somewhere else or remove it completely... 
	template<bool> struct CompileTimeError;
	template<> struct CompileTimeError<true> {};
#define _2REAL_CT_CONDITION( XPR, MSG )							\
	{															\
		_2Real::CompileTimeError<(XPR)> _2REAL_ERROR_##MSG;		\
		(void)_2REAL_ERROR_##MSG;								\
	}

#define _2REAL_RT_CONDITION( EXPRESSION, MESSAGE )					\
	{																\
		if( !( EXPRESSION ) ) 										\
		{															\
			std::stringstream sstr;									\
			sstr << "[vader] Runtime error in " << __FILE__			\
						<< ", line " << __LINE__ << ": "			\
						<< MESSAGE;									\
			throw std::runtime_error( sstr.str() );					\
		}															\
	}



	template<typename T>	T plusAbs( T sum, T val )		{	return sum + std::abs( val );	}
	template<typename T>	T plusSquared( T sum, T val )	{	return sum + val * val;			}
	template<typename T>	T makeAbs( T val )				{	return std::abs( val );			}

	template<typename T, std::size_t s>
	class Vector
	{
	public:
		enum
		{
			Size = s
		};

		typedef Vector<T,s>	_Myt;

		typedef T			value_type;

		typedef T&			reference;
		typedef const T&	const_reference;

		typedef T*			iterator;
		typedef const T*	const_iterator;		

		friend std::istream & operator>>( std::istream &in, Point &number );
		friend std::ostream & operator<<( std::ostream &out, Point const& number );

	public:

		//default constructor, initializes with 0
		Vector()
#ifdef _2REAL_BLAS_DYNAMIC_MEMORY
			: data( new value_type[Size] )
#endif
		{
			std::fill( this->begin(), this->begin() + Size, 0 );
		}

		//initialize all fields with value
		Vector( value_type value )
#ifdef _2REAL_BLAS_DYNAMIC_MEMORY
			: data( new value_type[Size] )
#endif
		{
			std::fill( this->begin(), this->begin() + Size, value );
		}

		//initialize vector of size 2 with x, y
		Vector( value_type v0, value_type v1 )
#ifdef _2REAL_BLAS_DYNAMIC_MEMORY
			: data( new value_type[Size] )
#endif
		{
			_2REAL_CT_CONDITION( Size == 2, Argument_List_length_invalid );
			this->data[0] = v0;	this->data[1] = v1;
		}

		//initialize vector of size 3 with x, y, z
		Vector( value_type v0, value_type v1, value_type v2 )
#ifdef _2REAL_BLAS_DYNAMIC_MEMORY
			: data( new value_type[Size] )
#endif
		{
			_2REAL_CT_CONDITION( Size == 3, Argument_List_length_invalid );
			this->data[0] = v0;	this->data[1] = v1;	this->data[2] = v2;
		}

		//initialize vector of size 4 with x, y, z, w
		Vector( value_type v0, value_type v1, value_type v2, value_type v3 )
#ifdef _2REAL_BLAS_DYNAMIC_MEMORY
			: data( new value_type[Size] )
#endif
		{
			_2REAL_CT_CONDITION( Size == 4, Argument_List_length_invalid );
			this->data[0] = v0;	this->data[1] = v1;	this->data[2] = v2;	this->data[3] = v3;
		}

		//copy constructor
		Vector( const Vector &v )
#ifdef _2REAL_BLAS_DYNAMIC_MEMORY
			: data( new value_type[Size] )
#endif
		{
			std::copy( v.begin(), v.end(), this->data );
		}

		~Vector()
		{
#ifdef _2REAL_BLAS_DYNAMIC_MEMORY
			delete [] this->data;
#endif
		}

		iterator 		begin()			{	return this->data;					}
		iterator 		end() 			{	return this->data + Size;			}

		const_iterator	begin() const	{	return this->data;					}
		const_iterator	end() const 	{	return this->data + Size;			}

		reference		front() 		{	return *( this->data );				}
		reference		back() 			{	return *( this->data + Size - 1 );	}

		const_reference	front() const	{	return *( this->data );				}
		const_reference	back() const	{	return *( this->data + Size - 1 );	}

		std::size_t		size() const	{	return Size;	}

		//set all fields to value
		void set( value_type value )
		{
			std::fill( this->begin(), this->end(), value );
		}

		//copy all fields from v
		void set( const Vector &v )
		{
			std::copy( v.begin(), v.end(), this->data );
		}

		value_type	operator[] ( std::size_t index ) const
		{
			_2REAL_RT_CONDITION( index < Size, "vector bounds violation" );
			return this->data[index];
		}

		reference	operator[] ( std::size_t index )
		{
			_2REAL_RT_CONDITION( index < Size, "vector bounds violation" );
			return this->data[index];
		}

		//scale all fields
		Vector<T,s> &operator *= ( value_type scale )
		{
			std::transform( this->begin(), this->end(), this->begin(), std::bind1st( std::multiplies<T>(), scale ) );
			return *this;
		}

		//divide all fields
		Vector<T,s> &operator /= ( value_type div )
		{
			*this *= 1.0 / div;
			return *this;
		}

		//element-wise add
		Vector<T,s> &operator += ( const Vector<T,s> &v )
		{
			std::transform( this->begin(), this->end(), v.begin(), this->begin(), std::plus<T>() );
			return *this;
		}

		//element-wise subtract
		Vector<T,s> &operator -= ( const Vector<T,s> &v )
		{
			std::transform( this->begin(), this->end(), v.begin(), this->begin(), std::minus<T>() );
			return *this;
		}

		//set to cross product: lhs = lhs x rhs
		Vector<T,s> &operator ^= ( const Vector<T,s> &v )
		{
			_2REAL_CT_CONDITION( Size == 3, Cross_product_only_defined_in_3D_space );

			value_type temp[3];
			temp[0] = this->data[1] * v.data[2] - this->data[2] * v.data[1];
			temp[1] = this->data[2] * v.data[0] - this->data[0] * v.data[2];
			temp[2] = this->data[0] * v.data[1] - this->data[1] * v.data[0];

			std::memcpy( this->data, temp, sizeof( temp ) );
			return *this;
		}

		//return manhattan norm
		value_type	norm1() const
		{
			return std::accumulate( this->begin(), this->end(), 0, plusAbs<T> );
		}

		//return euclidian norm
		value_type	norm2() const
		{
			return sqrt( this->norm2Squared() );
		}

		//return squared euclidian norm
		value_type	norm2Squared() const
		{
			return std::accumulate( this->begin(), this->end(), 0, plusSquared<T> );
		}

		//get max element
		value_type	maximum() const
		{
			return *( std::max_element( this->begin(), this->end() ) );
		}

		//get min element
		value_type	minimum() const
		{
			return *( std::min_element( this->begin(), this->end() ) );
		}

		//return vector with absolute value in each field
		Vector<T,s> abs() const
		{
			Vector<T,s> ret( *this );
			std::transform( this->begin(), this->end(), ret.begin(), makeAbs<T> );	//cannot use std::abs here, need template (?)
			return ret;
		}

		//make to unit vector
		const Vector<T,s> &normalize()
		{
			*this /= this->norm2();
			return *this;
		}

		//get static zero vector
		static
		const Vector<T,s> &zero()
		{
			static Vector<T,s> z( 0.0 );
			return z;
		}

		//get static vector with fields of 1
		static
		const Vector<T,s> &one()
		{
			static Vector<T,s> o( 1.0 );
			return o;
		}

		template<typename T, std::size_t s>	friend std::ostream &operator << ( std::ostream &ostr, const Vector<T,s> &v );

		template<typename T, std::size_t s> 			friend Vector<T,s> 	operator - ( const Vector<T,s> &v );
		template<typename T, std::size_t s, typename F>	friend Vector<T,s> 	operator * ( F f, const Vector<T,s> &v );
		template<typename T, std::size_t s, typename F>	friend Vector<T,s> 	operator * ( const Vector<T,s> &v, F f );
		template<typename T, std::size_t s, typename F>	friend Vector<T,s>	operator / ( const Vector<T,s> &v, F f );
		template<typename T, std::size_t s>				friend Vector<T,s> 	operator + ( const Vector<T,s> &lhs, const Vector<T,s> &rhs );
		template<typename T, std::size_t s>				friend Vector<T,s> 	operator - ( const Vector<T,s> &lhs, const Vector<T,s> &rhs );
		template<typename T, std::size_t s>				friend T 		operator * ( const Vector<T,s> &lhs, const Vector<T,s> &rhs );
		template<typename T, std::size_t s>				friend Vector<T,s> 	operator ^ ( const Vector<T,s> &lhs, const Vector<T,s> &rhs );

		template<typename T, std::size_t s>				friend bool		operator < ( const Vector<T,s> &lhs, const Vector<T,s> &rhs );
		template<typename T, std::size_t s>				friend bool		operator > ( const Vector<T,s> &lhs, const Vector<T,s> &rhs );
		template<typename T, std::size_t s>				friend bool		operator <= ( const Vector<T,s> &lhs, const Vector<T,s> &rhs );
		template<typename T, std::size_t s>				friend bool		operator >= ( const Vector<T,s> &lhs, const Vector<T,s> &rhs );
		template<typename T, std::size_t s>				friend bool		operator == ( const Vector<T,s> &lhs, const Vector<T,s> &rhs );

	private:
#ifdef _2REAL_BLAS_DYNAMIC_MEMORY
		value_type	*data;
#else
		value_type	data[Size];
#endif

	};

	template<typename T, std::size_t s>
	std::ostream &operator << ( std::ostream &ostr, const Vector<T,s> &v )
	{
		ostr << "Vector<" << typeid( T ).name() << ", " << v.size() << ">" << std::endl;

		std::streamsize oldWidth = ostr.width( 10 );
		std::streamsize oldPrecision = ostr.precision( 3 );

		bool first = true;
		for( Vector<T,s>::const_iterator it = v.begin(); it != v.end(); ++it )
		{
			ostr << ( first ? "{" : ", " ) << ( *it );
			first = false;
		}
		ostr << "}";
		ostr.width( oldWidth );
		ostr.precision( oldPrecision );

		return ostr;
	}

	//return negative of v
	template<typename T, std::size_t s> 
	Vector<T,s> operator - ( const Vector<T,s> &v )
	{
		return v * -1.0;
	}

	//return v scaled by f
	template<typename T, std::size_t s, typename F>
	Vector<T,s> operator * ( F f, const Vector<T,s> &v )
	{
		Vector<T,s> result;
		std::transform( v.begin(), v.end(), result.begin(), std::bind1st( std::multiplies<T>(), f ) );

		return result;
	}

	//return v scaled by f
	template<typename T, std::size_t s, typename F>
	Vector<T,s> operator * ( const Vector<T,s> &v, F f )
	{
		Vector<T,s> result;
		std::transform( v.begin(), v.end(), result.begin(), std::bind1st( std::multiplies<T>(), f ) );

		return result;
	}

	//return v divided by f
	template<typename T, std::size_t s, typename F>
	Vector<T,s> operator / ( const Vector<T,s> &v, F f )
	{
		Vector<T,s> result;
		std::transform( v.begin(), v.end(), result.begin(), std::bind1st( std::multiplies<T>(), 1.0 / f ) );

		return result;
	}

	//return elementwise sum
	template<typename T, std::size_t s>
	Vector<T,s> operator + ( const Vector<T,s> &lhs, const Vector<T,s> &rhs )
	{
		Vector<T,s> result;
		std::transform( lhs.begin(), lhs.end(), rhs.begin(), result.begin(), std::plus<T>() );

		return result;
	}

	//return elementwise difference
	template<typename T, std::size_t s>
	Vector<T,s> operator - ( const Vector<T,s> &lhs, const Vector<T,s> &rhs )
	{
		Vector<T,s> result;
		std::transform( lhs.begin(), lhs.end(), rhs.begin(), result.begin(), std::minus<T>() );

		return result;
	}

	//return dot product
	template<typename T, std::size_t s>
	T operator * ( const Vector<T,s> &lhs, const Vector<T,s> &rhs )
	{
		return std::inner_product( lhs.begin(), lhs.end(), rhs.begin(), 0 );
	}

	//return cross product
	template<typename T, std::size_t s>
	Vector<T,s> operator ^ ( const Vector<T,s> &lhs, const Vector<T,s> &rhs )
	{
		_2REAL_CT_CONDITION( s == 3, Cross_product_only_defined_in_3D_space );

		Vector<T,s> result( 
			lhs.data[1] * rhs.data[2] - lhs.data[2] * rhs.data[1],
			lhs.data[2] * rhs.data[0] - lhs.data[0] * rhs.data[2],
			lhs.data[0] * rhs.data[1] - lhs.data[1] * rhs.data[0] );

		return result;
	}

	//return true if length of lhs is smaller than length of rhs
	template<typename T, std::size_t s>
	bool operator < ( const Vector<T,s> &lhs, const Vector<T,s> &rhs )
	{
		return ( lhs.norm2Squared() < rhs.norm2Squared() );
	}

	//return true if length of lhs is greater than length of rhs
	template<typename T, std::size_t s>
	bool operator > ( const Vector<T,s> &lhs, const Vector<T,s> &rhs )
	{
		return ( lhs.norm2Squared() > rhs.norm2Squared() );
	}

	//return true if length of lhs is smaller or equal than length of rhs
	template<typename T, std::size_t s>
	bool operator <= ( const Vector<T,s> &lhs, const Vector<T,s> &rhs )
	{
		return ( lhs.norm2Squared() <= rhs.norm2Squared() );
	}

	//return true if length of lhs is greater or equal than length of rhs
	template<typename T, std::size_t s>
	bool operator >= ( const Vector<T,s> &lhs, const Vector<T,s> &rhs )
	{
		return ( lhs.norm2Squared() >= rhs.norm2Squared() );
	}

	//return true if length of lhs is equal to length of rhs
	template<typename T, std::size_t s>
	bool operator == ( const Vector<T,s> &lhs, const Vector<T,s> &rhs )
	{
		return ( std::abs( lhs.norm2Squared() - rhs.norm2Squared() ) < std::numeric_limits<T>::epsilon() );
	}

#ifdef _2REAL_BLAS_DOUBLE_PRECISION
	typedef Vector<double,2>	Vec2;
	typedef Vector<double,3>	Vec3;
	typedef Vector<double,4>	Vec4;
#else
	typedef Vector<float,2>		Vec2;
	typedef Vector<float,3>		Vec3;
	typedef Vector<float,4>		Vec4;
#endif
}
