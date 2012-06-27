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

	typedef void ( _2REAL_CALLBACK *ExceptionCallback )( void *userData, BlockError const& error );
	typedef void ( _2REAL_CALLBACK *OutletCallback )( void *userData, OutputData const& data );
	typedef void ( _2REAL_CALLBACK *OutputCallback )( void *userData, std::list< OutputData > const& data );

	class AbstractExceptionCallbackHandler
	{

	public:

		AbstractExceptionCallbackHandler( void *object ) : m_Object( object ) {};
		virtual ~AbstractExceptionCallbackHandler() {};
		virtual void invoke( BlockError const& error ) = 0;
		bool operator<( AbstractExceptionCallbackHandler const& other ) { return m_Object < other.m_Object; }

	private:

		void	*m_Object;

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

		void invoke(BlockError const& error)
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
		virtual void invoke( OutputData const& data ) = 0;
		bool operator<( AbstractOutletCallbackHandler const& other ) { return m_Object < other.m_Object; }

	private:

		void	*m_Object;

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

		void invoke( OutputData const& data )
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
		virtual void invoke( std::list< OutputData > const& data ) = 0;
		bool operator<( AbstractOutputCallbackHandler const& other ) { return m_Object < other.m_Object; }

	private:

		void	*m_Object;

	};

	template< typename Callable >
	class OutputCallbackHandler : public AbstractOutputCallbackHandler
	{

	public:

		typedef void ( _2REAL_MEMBER_CALLBACK Callable::*Callback )( std::list< OutputData > &data );

		OutputCallbackHandler( Callable &callable, Callback method ) :
			AbstractOutputCallbackHandler( &callable ),
			m_Callable( callable ),
			m_Method( method )
		{
		}

		void invoke( std::list< OutputData > const& data )
		{
			( m_Callable.*m_Method )( data );
		}

	private:

		Callable						&m_Callable;
		Callback						m_Method;

	};

}