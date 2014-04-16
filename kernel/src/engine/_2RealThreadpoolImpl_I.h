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
#include "common/_2RealThreadpoolPolicy.h"

namespace _2Real
{
	class ThreadpoolImpl_I
	{

	public:

		static std::shared_ptr< ThreadpoolImpl_I > create( const ThreadpoolPolicy p );

		struct Id { virtual ~Id() = default; };

		typedef std::function< void() > JobPtr;
		typedef std::function< void() > CbPtr;

		ThreadpoolImpl_I() = default;
		virtual ~ThreadpoolImpl_I() = default;
		ThreadpoolImpl_I( ThreadpoolImpl_I const& other ) = delete;
		ThreadpoolImpl_I( ThreadpoolImpl_I && other ) = delete;
		ThreadpoolImpl_I& operator=( ThreadpoolImpl_I const& other ) = delete;
		ThreadpoolImpl_I& operator=( ThreadpoolImpl_I && other ) = delete;

		virtual Id createId() = 0;
		virtual void enqueueJob( ThreadpoolImpl_I::Id const&, JobPtr, CbPtr ) = 0;

	};
}