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

#include "helpers/_2RealCallback.h"

#include <vector>

namespace _2Real
{
	class Exception;
	class CustomType;

	namespace app
	{
		class BlockHandle;
		class ContextBlockHandle;

		typedef void ( _2REAL_CALLBACK *BlockExceptionCallback )( void *, std::pair< Exception, BlockHandle > );
		typedef void ( _2REAL_CALLBACK *ContextBlockExceptionCallback )( void *, std::pair< Exception, ContextBlockHandle > );
		typedef void ( _2REAL_CALLBACK *OutletDataCallback )( void *, std::shared_ptr< const CustomType > );
		typedef void ( _2REAL_CALLBACK *BlockDataCallback )( void *, std::vector< std::shared_ptr< const CustomType > > );

		typedef _2Real::AbstractCallback< std::vector< std::shared_ptr< const CustomType > > >			BlockCallback;
		typedef _2Real::AbstractCallback< std::shared_ptr< const CustomType > >							OutletCallback;
		typedef _2Real::AbstractCallback< std::pair< Exception, BlockHandle > >					BlockExcCallback;
		typedef _2Real::AbstractCallback< std::pair< Exception, ContextBlockHandle > >			ContextBlockExcCallback;
	}
}
