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

#include "_2RealCallbacks.h"

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

	class BlockError;
	class OutputData;

	class OutletFunctionCallback
	{

	public:

		OutletFunctionCallback( OutletCallback cb, void *userData ) :
			m_Callback( cb ),
			m_UserData( userData )
		{
		}

		bool operator<( OutletFunctionCallback const& src ) const
		{
			return ( m_Callback < src.m_Callback &&  m_UserData < src.m_UserData );
		}

		void invoke( OutputData const& data )
		{
			m_Callback( m_UserData, data );
		}

	private:

		OutletCallback		m_Callback;
		void				*m_UserData;

	};

	// what a nice name!
	struct OutletCbCmp
	{
		bool operator()( OutletFunctionCallback *rhs, OutletFunctionCallback *lhs )
		{
			return (*rhs < *lhs);
		}
	};

	class OutputFunctionCallback
	{

	public:

		OutputFunctionCallback( OutputCallback cb, void *userData ) :
			m_Callback( cb ),
			m_UserData( userData )
		{
		}

		bool operator<( OutputFunctionCallback const& src ) const
		{
			return ( m_Callback < src.m_Callback &&  m_UserData < src.m_UserData );
		}

		void invoke( std::list< OutputData > const& data )
		{
			m_Callback( m_UserData, data );
		}

	private:

		OutputCallback					m_Callback;
		void							*m_UserData;

	};

	// sigh
	struct OutputCbCmp
	{
		bool operator()( OutputFunctionCallback *rhs, OutputFunctionCallback *lhs )
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

		void invoke( BlockError const& error )
		{
			m_Callback( m_UserData, error );
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

	typedef std::set< OutputFunctionCallback *, OutputCbCmp >	OutputFunctionCallbacks;
	typedef std::set< OutletFunctionCallback *, OutletCbCmp >	OutletFunctionCallbacks;
	typedef std::set< ExceptionFunctionCallback *, ExcCbCmp >	ExceptionFunctionCallbacks;

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
		bool operator()( AbstractOutletCallbackHandler *rhs, AbstractOutletCallbackHandler *lhs )
		{
			return (*rhs < *lhs);
		}
	};

	// aaaaaaaaaaaaaaaaaaargh
	struct AOutputCbCmp
	{
		bool operator()( AbstractOutputCallbackHandler *rhs, AbstractOutputCallbackHandler *lhs )
		{
			return (*rhs < *lhs);
		}
	};

	typedef std::set< AbstractOutputCallbackHandler *, AOutputCbCmp >	OutputCallbackHandlers;
	typedef std::set< AbstractOutletCallbackHandler *, AOutletCbCmp >	OutletCallbackHandlers;
	typedef std::set< AbstractExceptionCallbackHandler *, AExcCbCmp >	ExceptionCallbackHandlers;

}