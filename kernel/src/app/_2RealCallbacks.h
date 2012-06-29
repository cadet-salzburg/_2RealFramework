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

#include "_2RealAppData.h"

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

		typedef void ( _2REAL_CALLBACK *ExceptionCallback )( void *userData, BlockError const& error );
		typedef void ( _2REAL_CALLBACK *OutletDataCallback )( void *userData, AppData const& data );
		typedef void ( _2REAL_CALLBACK *BlockDataCallback )( void *userData, std::list< AppData > const& data );

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

			typedef void ( _2REAL_MEMBER_CALLBACK Callable::*Callback )( BlockError const& );

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

		class AbstractOutletDataCallbackHandler
		{

		public:

			AbstractOutletDataCallbackHandler( void *object ) : m_Object( object ) {};
			virtual ~AbstractOutletDataCallbackHandler() {} ;
			virtual void invoke( AppData const& data ) = 0;
			bool operator<( AbstractOutletDataCallbackHandler const& other ) { return m_Object < other.m_Object; }

		private:

			void	*m_Object;

		};

		template< typename Callable >
		class OutletDataCallbackHandler : public AbstractOutletDataCallbackHandler
		{

		public:

			typedef void ( _2REAL_MEMBER_CALLBACK Callable::*Callback )( AppData const& data );

			OutletDataCallbackHandler( Callable &callable, Callback method ) :
				AbstractOutletDataCallbackHandler( &callable ),
				m_Callable( callable ),
				m_Method( method )
			{
			}

			void invoke( AppData const& data )
			{
				( m_Callable.*m_Method )( data );
			}

		private:

			Callable			&m_Callable;
			Callback			m_Method;

		};

		class AbstractBlockDataCallbackHandler
		{

		public:

			AbstractBlockDataCallbackHandler( void *object ) : m_Object( object ) {};
			virtual ~AbstractBlockDataCallbackHandler() {} ;
			virtual void invoke( std::list< AppData > const& data ) = 0;
			bool operator<( AbstractBlockDataCallbackHandler const& other ) { return m_Object < other.m_Object; }

		private:

			void	*m_Object;

		};

		template< typename Callable >
		class BlockDataCallbackHandler : public AbstractBlockDataCallbackHandler
		{

		public:

			typedef void ( _2REAL_MEMBER_CALLBACK Callable::*Callback )( std::list< AppData > const& data );

			BlockDataCallbackHandler( Callable &callable, Callback method ) :
				AbstractBlockDataCallbackHandler( &callable ),
				m_Callable( callable ),
				m_Method( method )
			{
			}

			void invoke( std::list< AppData > const& data )
			{
				( m_Callable.*m_Method )( data );
			}

		private:

			Callable						&m_Callable;
			Callback						m_Method;

		};
	}
}