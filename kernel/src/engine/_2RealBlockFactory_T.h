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

#include "common/_2RealStdIncludes.h"

#include "engine/_2RealBlockFactory_I.h"

#include "bundle/_2RealBlockIo.h"
#include "bundle/_2RealBlock.h"

namespace _2Real
{
	template< typename TBlock >
	class BlockFactory_T : public BlockFactory_I
	{

	public:

		std::shared_ptr< bundle::Block > create( bundle::BlockIo const& io, std::vector< std::shared_ptr< bundle::Block > > const& dependencies ) const
		{
			return std::shared_ptr< bundle::Block >( new TBlock( io, dependencies ) );
		}

	};
}