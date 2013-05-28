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

#include "helpers/_2RealException.h"
#include "helpers/_2RealTypeDescriptor.h"

#include <vector>
#include <list>
#include <string>
#include <sstream>

#ifdef _UNIX
	#include <typeinfo>
#else
	#include <typeinfo.h>
#endif

namespace _2Real
{
	struct NullType
	{
		bool operator==( NullType const& other ) const { return false; }
	};

	template< typename T > struct traits;

	template< typename T >
	struct traits< std::vector< T > >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( std::vector< T > ), Type::VECTOR, "vector", traits< T >::createTypeDescriptor() );
		}
	};

	//template< typename T >
	//struct traits< std::vector< T, Eigen::aligned_allocator< T > > >
	//{
	//	static TypeDescriptor *createTypeDescriptor()
	//	{
	//		return new TypeDescriptor( typeid( std::vector< T, Eigen::aligned_allocator< T > > ), Type::VECTOR, "vector", traits< T >::createTypeDescriptor() );
	//	}
	//};

	template< typename T >
	struct traits< std::list< T > >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( std::list< T > ), Type::LIST, "list", traits< T >::createTypeDescriptor() );
		}
	};

	template< >
	struct traits< NullType >
	{
		static TypeDescriptor * createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( NullType ), Type::EMPTY, "void", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< char >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( char ), Type::BYTE, "char", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< unsigned char >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( unsigned char ), Type::UBYTE, "unsigned char", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< short >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( short ), Type::SHORT, "short", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< unsigned short >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( unsigned short ), Type::USHORT, "unsigned short", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< int >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( int ), Type::INT, "int", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< unsigned int >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( unsigned int ), Type::UINT, "unsigned int", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< long >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( long ), Type::LONG, "long", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< unsigned long >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( unsigned long ), Type::ULONG, "unsigned long", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< float >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( float ), Type::FLOAT, "float", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< double >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( double ), Type::DOUBLE, "double", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< bool >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( bool ), Type::BOOL, "bool", TypeCategory::LOGICAL );
		}
	};

	template< >
	struct traits< std::string >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( std::string ), Type::STRING, "string", TypeCategory::UNIQUE );
		}
	};

	//template< typename T >
	//struct traits
	//{
	//	static TypeDescriptor *createTypeDescriptor()
	//	{
	//		std::ostringstream msg;
	//		msg << typeid( T ).name() << " is not a valid type";
	//		throw InvalidTypeException( msg.str() );
	//	}
	//};

	template< typename T >
	TypeDescriptor * createTypeDescriptor()
	{
		return traits< T >::createTypeDescriptor();
	}
}
