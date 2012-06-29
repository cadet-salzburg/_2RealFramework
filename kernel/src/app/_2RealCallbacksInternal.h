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

	class BlockError;

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
		typedef std::set< AbstractExceptionCallbackHandler *, AExcCbCmp >	ExceptionCallbackHandlers;
	}
}