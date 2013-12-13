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

#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	template< typename THeld >
	class SingletonHolder_T
	{

	public:

		SingletonHolder_T();
		~SingletonHolder_T();

		THeld & instance();
		THeld const& instance() const;

	private:

		THeld				*mHeld;
		std::mutex			mMutex;

	};

	template< typename THeld >
	SingletonHolder_T< THeld >::SingletonHolder_T() : mHeld( nullptr )
	{
	}

	template< typename THeld >
	SingletonHolder_T< THeld >::~SingletonHolder_T()
	{
		delete mHeld;
	}

	template< typename THeld >
	THeld& SingletonHolder_T< THeld >::instance()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		if ( !mHeld )
			mHeld = new THeld();

		return *mHeld;
	}

}

/*
*	Todos: replace this by CRTP ? anyway, low prio...
*	also, is it even useful to have a threadsafe singleton here? :/
*/