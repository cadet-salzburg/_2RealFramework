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

#include <set>

namespace _2Real
{

	class RunnableError;
	class Data;

	typedef void ( *ExceptionCallback )( void *userData, RunnableError &error );
	typedef void ( *DataCallback )( void *userData, Data &data );

	class DataFunctionCallback
	{

	public:

		DataFunctionCallback( DataCallback cb, void *userData ) :
			m_Callback( cb ),
			m_UserData( userData )
		{
		}

		bool operator<( DataFunctionCallback const& src ) const
		{
			return ( m_Callback < src.m_Callback &&  m_UserData < src.m_UserData );
		}

		void invoke( Data &data )
		{
			m_Callback( m_UserData, data );
		}

	private:

		DataCallback		m_Callback;
		void				*m_UserData;

	};

	// what a nice name!
	struct DataCbCmp
	{
		bool operator()( DataFunctionCallback *rhs, DataFunctionCallback *lhs )
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

		void invoke( RunnableError &error )
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

	typedef std::set< DataFunctionCallback *, DataCbCmp >		DataFunctionCallbacks;
	typedef std::set< ExceptionFunctionCallback *, ExcCbCmp >	ExceptionFunctionCallbacks;

	class AbstractExceptionCallbackHandler
	{

	public:

		AbstractExceptionCallbackHandler( void *object ) : m_Object(object) {};
		virtual ~AbstractExceptionCallbackHandler() {};
		virtual void invoke( RunnableError &error ) = 0;
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

		typedef void (Callable::*Callback)(RunnableError&);

		ExceptionCallbackHandler(Callable &callable, Callback method) :
			AbstractExceptionCallbackHandler(&callable),
			m_Callable(callable),
			m_Method(method)
		{
		}

		void invoke(RunnableError &error)
		{
			(m_Callable.*m_Method)(error);
		}

	private:

		Callable			&m_Callable;
		Callback			m_Method;

	};

	class AbstractDataCallbackHandler
	{

	public:

		AbstractDataCallbackHandler( void *object ) : m_Object(object) {};
		virtual ~AbstractDataCallbackHandler() {} ;
		virtual void invoke(Data &data) = 0;
		bool operator<( AbstractDataCallbackHandler const& other ) { return m_Object < other.m_Object; }

	private:

		void	*m_Object;

	};

	// at some point, i might redo this :/
	struct ADataCbCmp
	{
		bool operator()( AbstractDataCallbackHandler *rhs, AbstractDataCallbackHandler *lhs )
		{
			return (*rhs < *lhs);
		}
	};

	template< typename Callable >
	class DataCallbackHandler : public AbstractDataCallbackHandler
	{

	public:

		typedef void (Callable::*Callback)(Data&);

		DataCallbackHandler(Callable &callable, Callback method) :
			AbstractDataCallbackHandler(&callable),
			m_Callable(callable),
			m_Method(method)
		{
		}

		void invoke(Data &data)
		{
			(m_Callable.*m_Method)(data);
		}

	private:

		Callable			&m_Callable;
		Callback			m_Method;

	};

	typedef std::set< AbstractDataCallbackHandler *, ADataCbCmp >		DataCallbackHandlers;
	typedef std::set< AbstractExceptionCallbackHandler *, AExcCbCmp >	ExceptionCallbackHandlers;

}