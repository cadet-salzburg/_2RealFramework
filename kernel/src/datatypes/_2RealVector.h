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

#define EIGEN_DONT_ALIGN_STATICALLY
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT

#include "Eigen/Dense"
//#include "Eigen/StdVector"
//#include <list>

namespace _2Real
{
#ifdef _2REAL_BLAS_DOUBLE_PRECISION
	typedef Eigen::Vector2d		Vec2;
	typedef Eigen::Vector3d		Vec3;
	typedef Eigen::Vector4d		Vec4;
#else
	typedef Eigen::Vector2f		Vec2;
	typedef Eigen::Vector3f		Vec3;
	typedef Eigen::Vector4f		Vec4;
#endif
	//typedef std::vector< Vec2, Eigen::aligned_allocator< Vec2 > >	Vec2Vector;
	//typedef std::vector< Vec3, Eigen::aligned_allocator< Vec3 > >	Vec3Vector;
	//typedef std::vector< Vec4, Eigen::aligned_allocator< Vec4 > >	Vec4Vector;
	//typedef std::list< Vec2, Eigen::aligned_allocator< Vec2 > >		Vec2List;
	//typedef std::list< Vec3, Eigen::aligned_allocator< Vec3 > >		Vec3List;
	//typedef std::list< Vec4, Eigen::aligned_allocator< Vec4 > >		Vec4List;
}
