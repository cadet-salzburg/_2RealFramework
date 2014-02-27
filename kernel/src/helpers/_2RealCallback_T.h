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

#ifdef _WIN32
	#define _2REAL_CALLBACK __cdecl
	#define _2REAL_MEMBER_CALLBACK __thiscall
#elif _WIN64
	#define _2REAL_CALLBACK __cdecl
	#define _2REAL_MEMBER_CALLBACK __thiscall
#else
	#define _2REAL_CALLBACK
	#define _2REAL_MEMBER_CALLBACK
#endif

namespace _2Real
{
	template< typename TArg >
	class AbstractCallback_T
	{

	public:

		AbstractCallback_T( void *target ) :
			mTarget( target )
		{
		}

		virtual ~AbstractCallback_T() {};
		virtual void invoke( TArg arg ) = 0;

		bool operator<( AbstractCallback_T< TArg > const& other )
		{
			return ( mTarget < other.mTarget );
		}

	private:

		void		*mTarget;

	};

	template<>
	class AbstractCallback_T< void >
	{

	public:

		AbstractCallback_T( void *target ) :
			mTarget( target )
		{
		}

		virtual ~AbstractCallback_T() {};
		virtual void invoke() = 0;

		bool operator<( AbstractCallback_T< void > const& other )
		{
			return ( mTarget < other.mTarget );
		}

	private:

		void		*mTarget;

	};

	template< typename TCallable, typename TArg >
	class MemberCallback_T : public AbstractCallback_T< TArg >
	{

	public:

		typedef void ( _2REAL_MEMBER_CALLBACK TCallable::*Callback )( TArg );

		MemberCallback_T( TCallable *callable, Callback method ) :
			AbstractCallback_T< TArg >( callable ),
			mCallable( callable ),
			mMethod( method )
		{
		}

		void invoke( TArg arg )
		{
			( mCallable->*mMethod )( arg );
		}

	private:

		MemberCallback_T( MemberCallback_T const& other );
		MemberCallback_T& operator=( MemberCallback_T const& other );

		TCallable			*mCallable;
		Callback			mMethod;

	};

	template< typename TCallable >
	class MemberCallback_T< TCallable, void > : public AbstractCallback_T< void >
	{

	public:

		typedef void ( _2REAL_MEMBER_CALLBACK TCallable::*Callback )( void );

		MemberCallback_T( TCallable *callable, Callback method ) :
			AbstractCallback_T< void >( callable ),
			mCallable( callable ),
			mMethod( method )
		{
		}

		void invoke()
		{
			( mCallable->*mMethod )();
		}

	private:

		MemberCallback_T( MemberCallback_T const& other );
		MemberCallback_T& operator=( MemberCallback_T const& other );

		TCallable			*mCallable;
		Callback			mMethod;

	};

	template< typename TArg >
	class FunctionCallback_T : public AbstractCallback_T< TArg >
	{

	public:

		typedef void ( _2REAL_CALLBACK *CallbackFunction )( void *, TArg );

		FunctionCallback_T( CallbackFunction func, void *userData ) :
			AbstractCallback_T< TArg >( (void*)func ),
			mFunction( func ),
			mUserData( userData )
		{
		}

		void invoke( TArg arg )
		{
			( mFunction )( mUserData, arg );
		}

	private:

		FunctionCallback_T( FunctionCallback_T const& other );
		FunctionCallback_T& operator=( FunctionCallback_T const& other );

		CallbackFunction	mFunction;
		void				*mUserData;

	};

	template<>
	class FunctionCallback_T< void > : public AbstractCallback_T< void >
	{

	public:

		typedef void ( _2REAL_CALLBACK *CallbackFunction )( void * );

		FunctionCallback_T( CallbackFunction func, void *userData ) :
			AbstractCallback_T< void >( (void*)func ),
			mFunction( func ),
			mUserData( userData )
		{
		}

		void invoke()
		{
			( mFunction )( mUserData );
		}

	private:

		FunctionCallback_T( FunctionCallback_T const& other );
		FunctionCallback_T& operator=( FunctionCallback_T const& other );

		CallbackFunction	mFunction;
		void				*mUserData;

	};

	template< typename TCallable, typename TArg >
	std::shared_ptr< AbstractCallback_T< TArg > > makeCallback( TCallable *callable, void ( __thiscall TCallable::*callback )( TArg ) )
	{
		return std::shared_ptr< AbstractCallback_T< TArg > >( new MemberCallback_T< TCallable, TArg >( callable, callback ) );
	}

	//template< typename TCallable, void >
	//std::shared_ptr< AbstractCallback_T< void > > makeCallback( TCallable *callable, void ( TCallable::*callback )( void ) )
	//{
	//	return std::shared_ptr< AbstractCallback_t< void > >( new MemberCallback_T< TCallable, void >( callable, callback ) );
	//}

	//template< typename TArg >
	//std::shared_ptr< AbstractCallback_T< TArg > > makeCallback( void ( *callback )( void ), void *userData = nullptr )
	//{
	//	return std::shared_ptr< AbstractCallback_t< TArg > >( new FunctionCallback_T< TArg >( callback, userData ) );
	//}
}
