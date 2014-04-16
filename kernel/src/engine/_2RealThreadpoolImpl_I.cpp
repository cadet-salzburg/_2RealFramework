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

#include "engine/_2RealThreadpoolImpl_I.h"
#include "engine/_2RealFifoThreadpool.h"
#include "engine/_2RealUniqueThreadpool.h"

namespace _2Real
{

	std::shared_ptr< ThreadpoolImpl_I > ThreadpoolImpl_I::create( const ThreadpoolPolicy p )
	{
		switch ( p )
		{
		case ThreadpoolPolicy::FIFO:
			return std::shared_ptr< ThreadpoolImpl_I >( new FifoThreadpool( 8 ) );
		case ThreadpoolPolicy::DEDICATED:
			return std::shared_ptr< ThreadpoolImpl_I >( new UniqueThreadpool );
		default:
			return std::shared_ptr< ThreadpoolImpl_I >();
		}
	}

}
