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

#include "_2RealOutputData.h"

#include <set>
#include <list>

#if defined(_WIN32)
	#define _2REAL_CALLBACK __cdecl
	#define _2REAL_MEMBER_CALLBACK __thiscall
#else
	#define _2REAL_CALLBACK
	#define _2REAL_MEMBER_CALLBACK
#endif

#include "Poco/Mutex.h"

namespace _2Real
{

	class BlockError;
	class OutputData;

	typedef void ( _2REAL_CALLBACK *ExceptionCallback )( void *userData, BlockError &error );
	typedef void ( _2REAL_CALLBACK *OutletCallback )( void *userData, OutputData &data );
	typedef void ( _2REAL_CALLBACK *OutputCallback )( void *userData, std::list< OutputData > data );

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

		void invoke( OutputData &data )
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

		void invoke( OutputData &data )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
			m_OutputData.push_back( data );
		}

		void complete()
		{
			m_Callback( m_UserData, m_OutputData );
			m_OutputData.clear();
		}

	private:

		mutable Poco::FastMutex			m_DataAccess;
		std::list< OutputData >			m_OutputData;
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

		void invoke( BlockError &error )
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

	// maybe move to other header?
	typedef std::set< OutputFunctionCallback *, OutputCbCmp >	OutputFunctionCallbacks;
	typedef std::set< OutletFunctionCallback *, OutletCbCmp >	OutletFunctionCallbacks;
	typedef std::set< ExceptionFunctionCallback *, ExcCbCmp >	ExceptionFunctionCallbacks;

	class AbstractExceptionCallbackHandler
	{

	public:

		AbstractExceptionCallbackHandler( void *object ) : m_Object(object) {};
		virtual ~AbstractExceptionCallbackHandler() {};
		virtual void invoke( BlockError &error ) = 0;
		bool operator<( AbstractExceptionCallbackHandler const& other ) { return m_Object < other.m_Object; }

	private:

		void	*m_Object;

	};

	// .....
	struct AExcCbCmp
	{
		bool operator()( AbstractExceptionCallbackHandler *rhs, AbstractExceptionCallbackHandler *lhs )
		{
			return (*rhs < *lhs);
		}
	};

	template< typename Callable >
	class ExceptionCallbackHandler : public AbstractExceptionCallbackHandler
	{

	public:

		typedef void ( _2REAL_MEMBER_CALLBACK Callable::*Callback )( BlockError& );

		ExceptionCallbackHandler(Callable &callable, Callback method) :
			AbstractExceptionCallbackHandler(&callable),
			m_Callable(callable),
			m_Method(method)
		{
		}

		void invoke(BlockError &error)
		{
			(m_Callable.*m_Method)(error);
		}

	private:

		Callable			&m_Callable;
		Callback			m_Method;

	};

	class AbstractOutletCallbackHandler
	{

	public:

		AbstractOutletCallbackHandler( void *object ) : m_Object( object ) {};
		virtual ~AbstractOutletCallbackHandler() {} ;
		virtual void invoke( OutputData &data ) = 0;
		bool operator<( AbstractOutletCallbackHandler const& other ) { return m_Object < other.m_Object; }

	private:

		void	*m_Object;

	};

	// at some point, i might redo this :/
	struct AOutletCbCmp
	{
		bool operator()( AbstractOutletCallbackHandler *rhs, AbstractOutletCallbackHandler *lhs )
		{
			return (*rhs < *lhs);
		}
	};

	template< typename Callable >
	class OutletCallbackHandler : public AbstractOutletCallbackHandler
	{

	public:

		typedef void ( _2REAL_MEMBER_CALLBACK Callable::*Callback )( OutputData& );

		OutletCallbackHandler( Callable &callable, Callback method ) :
			AbstractOutletCallbackHandler( &callable ),
			m_Callable( callable ),
			m_Method( method )
		{
		}

		void invoke( OutputData &data )
		{
			( m_Callable.*m_Method )( data );
		}

	private:

		Callable			&m_Callable;
		Callback			m_Method;

	};

	class AbstractOutputCallbackHandler
	{

	public:

		AbstractOutputCallbackHandler( void *object ) : m_Object( object ) {};
		virtual ~AbstractOutputCallbackHandler() {} ;
		virtual void invoke( OutputData &data ) = 0;
		bool operator<( AbstractOutputCallbackHandler const& other ) { return m_Object < other.m_Object; }
		virtual void complete() = 0;

	private:

		void	*m_Object;

	};

	// aaaaaaaaaaaaaaaaaaargh
	struct AOutputCbCmp
	{
		bool operator()( AbstractOutputCallbackHandler *rhs, AbstractOutputCallbackHandler *lhs )
		{
			return (*rhs < *lhs);
		}
	};

	template< typename Callable >
	class OutputCallbackHandler : public AbstractOutputCallbackHandler
	{

	public:

		typedef void ( _2REAL_MEMBER_CALLBACK Callable::*Callback )( std::list< OutputData > data );

		OutputCallbackHandler( Callable &callable, Callback method ) :
			AbstractOutputCallbackHandler( &callable ),
			m_Callable( callable ),
			m_Method( method )
		{
		}

		void invoke( OutputData &data )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
			m_OutputData.push_back( data );
		}

		void complete()
		{
			( m_Callable.*m_Method )( m_OutputData );
			m_OutputData.clear();
		}

	private:

		mutable Poco::FastMutex			m_DataAccess;
		std::list< OutputData >			m_OutputData;
		Callable						&m_Callable;
		Callback						m_Method;

	};

	// maybe move to other header?
	typedef std::set< AbstractOutputCallbackHandler *, AOutputCbCmp >	OutputCallbackHandlers;
	typedef std::set< AbstractOutletCallbackHandler *, AOutletCbCmp >	OutletCallbackHandlers;
	typedef std::set< AbstractExceptionCallbackHandler *, AExcCbCmp >	ExceptionCallbackHandlers;

}