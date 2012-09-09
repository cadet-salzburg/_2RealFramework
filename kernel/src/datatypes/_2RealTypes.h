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

#include "datatypes/_2RealImage.h"
#include "datatypes/_2RealImageT.h"
#include "datatypes/_2RealPoint.h"
#include "datatypes/_2RealSpace.h"
#include "datatypes/_2RealVector.h"
#include "datatypes/_2RealNumber.h"
#include "datatypes/_2RealMatrix.h"
#include "datatypes/_2RealSkeleton.h"
#include "datatypes/_2RealRigidBody.h"
#include "datatypes/_2RealQuaternion.h"
#include "datatypes/_2RealFilePath.h"
#include "datatypes/_2RealAudioBuffer.h"

#include <vector>
#include <list>
#include <string>
#include <sstream>

#if defined _UNIX || defined TARGET_OS_MAC
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

	template< typename T >
	struct traits< std::vector< T, Eigen::aligned_allocator< T > > >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( std::vector< T, Eigen::aligned_allocator< T > > ), Type::VECTOR, "vector", traits< T >::createTypeDescriptor() );
		}
	};

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
	struct traits< Number >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Number ), Type::NUMBER, "number", TypeCategory::ARITHMETHIC );
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

	template< >
	struct traits< FilePath >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( FilePath ), Type::FILEPATH, "filepath", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< Image8U >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Image8U ), Type::IMAGE, "image", traits< unsigned char >::createTypeDescriptor() );
		}
	};

	template< >
	struct traits< Image16U >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Image16U ), Type::IMAGE, "image", traits< unsigned short >::createTypeDescriptor() );
		}
	};

	template< >
	struct traits< Image32F >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Image32F ), Type::IMAGE, "image", traits< float >::createTypeDescriptor() );
		}
	};

	template< >
	struct traits< Image64F >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Image64F ), Type::IMAGE, "image", traits< double >::createTypeDescriptor() );
		}
	};

	template< >
	struct traits< Image >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Image ), Type::IMAGE, "image", traits< Number >::createTypeDescriptor() );
		}
	};

	template< >
	struct traits< Point >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Point ), Type::POINT, "point", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< Skeleton >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Skeleton ), Type::SKELETON, "skeleton", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< Quaternion >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Quaternion ), Type::QUATERNION, "quaternion", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< RigidBody >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( RigidBody ), Type::RIGIDBODY, "rigid body", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< Vec2 >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Vec2 ), Type::VECTOR2D, "vector 2D", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< Vec3 >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Vec3 ), Type::VECTOR3D, "vector 3D", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< Vec4 >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Vec4 ), Type::VECTOR4D, "vector 4D", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< Mat3 >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Mat3 ), Type::VECTOR2D, "3x3 matrix", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< Mat4 >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Mat4 ), Type::VECTOR3D, "4x4 matrix", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< Space2D >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Space2D ), Type::SPACE2D, "space 2D", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< Space3D >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Space3D ), Type::SPACE3D, "space 3D", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< Space4D >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( Space4D ), Type::SPACE4D, "space 4D", TypeCategory::ARITHMETHIC );
		}
	};

	template< >
	struct traits< FaceDesc >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( FaceDesc ), Type::FACEDESC, "face description", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< FaceCast >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( FaceCast ), Type::FACECAST, "face depth cast", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< AudioBuffer >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( AudioBuffer ), Type::AUDIO, "audio buffer", TypeCategory::UNIQUE );
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
