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

#include <memory>

namespace _2Real
{

	//template< typename T, typename... TCtorArgs >
	//std::shared_ptr< T > clone( std::shared_ptr< T > other, TCtorArgs... ctorArgs )
	//{
	//	assert( other != nullptr );

	//	std::shared_ptr< T > result( new T( ctorArgs... ) );
	//	result->cloneFrom( *( other.get() ) );
	//	return result;
	//}

	//template< typename T, typename... TCtorArgs >
	//std::shared_ptr< T > clone( std::shared_ptr< const T > other, TCtorArgs... ctorArgs )
	//{
	//	assert( other != nullptr );

	//	std::shared_ptr< T > result( new T( ctorArgs... ) );
	//	result->cloneFrom( *( other.get() ) );
	//	return result;
	//}

	///*
	//*	base class for all metainfo exported by dlls
	//*	assignemnt & move -> disabled, cloneFrom must be implemented
	//*/
	//template< typename T >
	//class CloneableMetainfo
	//{

	//public:

	//	CloneableMetainfo() = default;
	//	virtual ~CloneableMetainfo() = default;

	//	CloneableMetainfo( CloneableMetainfo< T > const& other ) = delete;
	//	CloneableMetainfo( CloneableMetainfo< T > && other ) = delete;
	//	CloneableMetainfo& operator=( CloneableMetainfo< T > const& other ) = delete;
	//	CloneableMetainfo& operator=( CloneableMetainfo< T > && other ) = delete;

	//	virtual void cloneFrom( T const& other ) = 0;

	//};

}
