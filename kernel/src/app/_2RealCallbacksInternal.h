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

#include "app/_2RealCallbacks.h"

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
	namespace app
	{

		class AppData;

		class OutletDataFunctionCallback
		{

		public:

			OutletDataFunctionCallback( OutletDataCallback cb, void *userData ) :
				m_Callback( cb ),
				m_UserData( userData )
			{
			}

			bool operator<( OutletDataFunctionCallback const& src ) const
			{
				return ( m_Callback < src.m_Callback &&  m_UserData < src.m_UserData );
			}

			void invoke( AppData const& data )
			{
				m_Callback( m_UserData, data );
			}

		private:

			OutletDataCallback	m_Callback;
			void				*m_UserData;

		};

		// what a nice name!
		struct OutletCbCmp
		{
			bool operator()( OutletDataFunctionCallback *rhs, OutletDataFunctionCallback *lhs )
			{
				return (*rhs < *lhs);
			}
		};

		class BlockDataFunctionCallback
		{

		public:

			BlockDataFunctionCallback( BlockDataCallback cb, void *userData ) :
				m_Callback( cb ),
				m_UserData( userData )
			{
			}

			bool operator<( BlockDataFunctionCallback const& src ) const
			{
				return ( m_Callback < src.m_Callback &&  m_UserData < src.m_UserData );
			}

			void invoke( std::list< AppData > const& data )
			{
				m_Callback( m_UserData, data );
			}

		private:

			BlockDataCallback				m_Callback;
			void							*m_UserData;

		};

		// sigh
		struct OutputCbCmp
		{
			bool operator()( BlockDataFunctionCallback *rhs, BlockDataFunctionCallback *lhs )
			{
				return (*rhs < *lhs);
			}
		};

		class ExceptionFunctionCallback
		{

		public:

			ExceptionFunctionCallback( ExceptionCallback cb, void *userData ) :
				m_Callback( cb ),
				m_UserData( userData )
			{
			}

			bool operator<( ExceptionFunctionCallback const& src ) const
			{
				return ( m_Callback < src.m_Callback &&  m_UserData < src.m_UserData );
			}

			void invoke( Exception const& e, BlockHandle const& h )
			{
				m_Callback( m_UserData, e, h );
			}

		private:

			ExceptionCallback	m_Callback;
			void				*m_UserData;

		};

		// basically, i suck at naming stuff.
		struct ExcCbCmp
		{
			bool operator()( ExceptionFunctionCallback *rhs, ExceptionFunctionCallback *lhs )
			{
				return (*rhs < *lhs);
			}
		};

		typedef std::set< BlockDataFunctionCallback *, OutputCbCmp >	BlockDataFunctionCallbacks;
		typedef std::set< OutletDataFunctionCallback *, OutletCbCmp >	OutletDataFunctionCallbacks;
		typedef std::set< ExceptionFunctionCallback *, ExcCbCmp >		ExceptionFunctionCallbacks;

		// .....
		struct AExcCbCmp
		{
			bool operator()( AbstractExceptionCallbackHandler *rhs, AbstractExceptionCallbackHandler *lhs )
			{
				return (*rhs < *lhs);
			}
		};

		// at some point, i might redo this :/
		struct AOutletCbCmp
		{
			bool operator()( AbstractOutletDataCallbackHandler *rhs, AbstractOutletDataCallbackHandler *lhs )
			{
				return (*rhs < *lhs);
			}
		};

		// aaaaaaaaaaaaaaaaaaargh
		struct AOutputCbCmp
		{
			bool operator()( AbstractBlockDataCallbackHandler *rhs, AbstractBlockDataCallbackHandler *lhs )
			{
				return (*rhs < *lhs);
			}
		};

		typedef std::set< AbstractBlockDataCallbackHandler *, AOutputCbCmp >	BlockDataCallbackHandlers;
		typedef std::set< AbstractOutletDataCallbackHandler *, AOutletCbCmp >	OutletDataCallbackHandlers;
		typedef std::set< AbstractExceptionCallbackHandler *, AExcCbCmp >		ExceptionCallbackHandlers;

		//template< typename TArg >
		//class AbstractMemberCallback
		//{
		//	virtual ~AbstractMemberCallback() {}
		//	bool opdator 
		//};

		//template< typename TCallable, typename TArg >
		//class MemberCallback
		//{

		//public:

		//	typedef void ( _2REAL_MEMBER_CALLBACK TCallable::*Callback )( TArg const& );

		//	MemberCallback( TCallable &callable, Callback method ) :
		//		m_Callable( callable ),
		//		m_Method( method )
		//	{
		//	}

		//	void invoke( TArg const& arg )
		//	{
		//		( m_Callable.*m_Method )( arg );
		//	}

		//	bool operator<( MemberCallback< TCallable, TArg > const& other )
		//	{
		//		return ( m_Callable < other.m_Callable && m_Method < other.m_Method );
		//	}

		//private:

		//	TCallable			&m_Callable;
		//	Callback			m_Method;

		//};

		//template< typename TArg >
		//class MemberCallbackCompare
		//{

		//public:
		//	bool operator()( MemberCallback< TArg > *cbA, MemberCallback< TArg > *cbB )
		//	{
		//		return ( *cbA < *cbB );
		//	}

		//};

		//template< typename TArg >
		//class FunctionCallback< TArg >
		//{

		//public:

		//	typedef ( _2REAL_CALLBACK *CallbackFunction )( void *, TArg const& );

		//	FunctionCallback( void *userData, CallbackFunction func ) :
		//		m_UserData( userData ),
		//		m_Function( func )
		//	{
		//	}

		//	void invoke( TArg &arg )
		//	{
		//		( m_Function )( m_UserData, arg );
		//	}

		//	bool operator<( FunctionCallback< TArg > const& other )
		//	{
		//		return ( m_Function < other.m_Function && m_UserData < other.m_userData );
		//	}

		//private:

		//	CallbackFunction	m_Function;
		//	void				*m_UserData;

		//};

		//template< typename TArg >
		//class FunctionCallbackCompare
		//{

		//public:
		//	bool operator()( FunctionCallback< TArg > *cbA, FunctionCallback< TArg > *cbB )
		//	{
		//		return ( *cbA < *cbB );
		//	}

		//};

		//// threadsafe callback event
		//// protected against double registration
		//template< typename TArgs >
		//class CallbackEvent
		//{

		//public:

		//	typedef std::set< FunctionCallback< TArg > *, FunctionCallbackCompare< TArg > >					FunctionCallbacks;
		//	typedef std::set< FunctionCallback< TArg > *, FunctionCallbackCompare< TArg > >::iterator		FunctionCallbacksIterator;
		//	typedef std::set< FunctionCallback< TArg > *, FunctionCallbackCompare< TArg > >::const_iterator	FunctionCallbacksConstIterator;

		//	typedef std::set< MemberCallback< TArg > *, MemberCallbackCompare< TArg > >						MemberCallbacks;
		//	typedef std::set< MemberCallback< TArg > *, MemberCallbackCompare< TArg > >::iterator			MemberCallbacksIterator;
		//	typedef std::set< MemberCallback< TArg > *, MemberCallbackCompare< TArg > >::const_iterator		MemberCallbacksConstIterator;

		//	CallbackEvent() {}
		//	~CallbackEvent() { clear(); }

		//	void clear()
		//	{
		//		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		//		for ( FunctionCallbacksIterator cbIter = m_FunctionCallbacks.begin(); cbIter != m_FunctionCallbacks.end(); ++cbIter ) delete *cbIter;
		//		for ( MemberCallbacksIterator cbIter = m_MemberCallbacks.begin(); cbIter != m_MemberCallbacks.end(); ++cbIter ) delete *cbIter;
		//		m_FunctionCallbacks.clear();
		//		m_MemberCallbacks.clear();
		//	}

		//	//void addListener( AbstractCallback< TArgs > const& callback )
		//	//{
		//	//	Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		//	//	CallbacksConstIterator cbIter = m_Callbacks.find( &callback );
		//	//	if ( cbIter == m_Callbacks.end() )
		//	//	{
		//	//		m_Callbacks.insert( &callback );
		//	//	}
		//	//	else delete &callback;
		//	//}

		//	//void removeListener( AbstractCallback< TArgs > const& callback )
		//	//{
		//	//	Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		//	//	CallbacksIterator cbIter = m_Callbacks.find( &callback );
		//	//	if ( cbIter != m_Callbacks.end() )
		//	//	{
		//	//		delete *cbIter;
		//	//		m_Callbacks.erase( cbIter );
		//	//	}
		//	//	delete &callback;
		//	//}

		//	void notify( TArgs &args )
		//	{
		//		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		//		for ( MemberCallbacksIterator cbIter = m_MemberCallbacks.begin(); cbIter != m_MemberCallbacks.end(); ++cbIter ) ( *cbIter )->invoke( args );
		//		for ( FunctionCallbacksIterator cbIter = m_FunctionCallbacks.begin(); cbIter != m_FunctionCallbacks.end(); ++cbIter ) ( *cbIter )->invoke( args );
		//	}

		//private:

		//	mutable Poco::FastMutex		m_Access;
		//	FunctionCallbacks			m_FunctionCallbacks;
		//	MemberCallbacks				m_MemberCallbacks;

		//};
	}
}