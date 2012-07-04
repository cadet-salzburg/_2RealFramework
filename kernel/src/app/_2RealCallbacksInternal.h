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

#include "helpers/_2RealPoco.h"

#include <set>
#include <list>

#if defined( _WIN32 )
	#define _2REAL_CALLBACK __cdecl
	#define _2REAL_MEMBER_CALLBACK __thiscall
#elif defined( _WIN64 )
	#define _2REAL_CALLBACK __cdecl
	#define _2REAL_MEMBER_CALLBACK __thiscall
#else
	#define _2REAL_CALLBACK
	#define _2REAL_MEMBER_CALLBACK
#endif

namespace _2Real
{
	template< typename TArg >
	class AbstractCallback
	{

	public:

		AbstractCallback( void *target ) :
			m_Target( target )
		{
		}

		virtual ~AbstractCallback() {};
		virtual void invoke( TArg &arg ) = 0;

		bool operator<( AbstractCallback< TArg > const& other )
		{
			return ( m_Target < other.m_Target );
		}

	private:

		void		*m_Target;

	};

	template< typename TArg >
	class AbstractCallbackCompare
	{

	public:
		bool operator()( AbstractCallback< TArg > *cbA, AbstractCallback< TArg > *cbB )
		{
			return ( *cbA < *cbB );
		}

	};

	template< typename TCallable, typename TArg >
	class MemberCallback : public AbstractCallback< TArg >
	{

	public:

		typedef void ( _2REAL_MEMBER_CALLBACK TCallable::*Callback )( TArg & );

		MemberCallback( TCallable &callable, Callback method ) :
			AbstractCallback( &callable ),
			m_Callable( callable ),
			m_Method( method )
		{
		}

		void invoke( TArg &arg )
		{
			( m_Callable.*m_Method )( arg );
		}

	private:

		TCallable			&m_Callable;
		Callback			m_Method;

	};

	template< typename TArg >
	class FunctionCallback : public AbstractCallback< TArg >
	{

	public:

		typedef void ( _2REAL_CALLBACK *CallbackFunction )( void *, TArg & );

		FunctionCallback( CallbackFunction func, void *userData ) :
			AbstractCallback( func ),
			m_UserData( userData ),
			m_Function( func )
		{
		}

		void invoke( TArg &arg )
		{
			( m_Function )( m_UserData, arg );
		}

	private:

		CallbackFunction	m_Function;
		void				*m_UserData;

	};

	template< typename TArg >
	class CallbackEvent
	{

	public:

		typedef std::set< AbstractCallback< TArg > *, AbstractCallbackCompare< TArg > >								Callbacks;
		typedef typename std::set< AbstractCallback< TArg > *, AbstractCallbackCompare< TArg > >::iterator			CallbackIterator;
		typedef typename std::set< AbstractCallback< TArg > *, AbstractCallbackCompare< TArg > >::const_iterator	CallbackConstIterator;

		CallbackEvent() {}
		~CallbackEvent() { clear(); }

		void clear()
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
			for ( CallbackIterator cbIter = m_Callbacks.begin(); cbIter != m_Callbacks.end(); ++cbIter ) delete *cbIter;
			m_Callbacks.clear();
		}

		void addListener( AbstractCallback< TArg > &callback )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
			CallbackIterator cbIter = m_Callbacks.find( &callback );
			if ( cbIter == m_Callbacks.end() )
			{
				m_Callbacks.insert( &callback );
			}
			else delete &callback;
		}

		void removeListener( AbstractCallback< TArg > &callback )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
			CallbackIterator cbIter = m_Callbacks.find( &callback );
			if ( cbIter != m_Callbacks.end() )
			{
				delete *cbIter;
				m_Callbacks.erase( cbIter );
			}
			delete &callback;
		}

		void notify( TArg &arg )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
			for ( CallbackIterator cbIter = m_Callbacks.begin(); cbIter != m_Callbacks.end(); ++cbIter ) ( *cbIter )->invoke( arg );
		}

	private:

		mutable Poco::FastMutex		m_Access;
		Callbacks					m_Callbacks;

	};
}