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

namespace _2Real
{

	class RunnableError;
	class Data;

	typedef void (*ExceptionCallback)(RunnableError &error);
	typedef void (*DataCallback)(Data &data);
	typedef void (*StateChangeCallback)(unsigned int &state);

	class AbstractExceptionCallbackHandler
	{
		AbstractExceptionCallbackHandler() {};
		virtual ~AbstractExceptionCallbackHandler() {} ;
		virtual void invoke(RunnableError &error) const = 0;
	};

	template< typename Callable >
	class ExceptionCallbackHandler : public AbstractExceptionCallbackHandler
	{

	public:

		typedef void (Callable::*Callback)(RunnableError&);

		ExceptionCallbackHandler(Callable &callable, Callback method) :
			m_Callable(callable),
			m_Method(method)
		{
		}

		~ExceptionCallbackHandler() {}

		void invoke(RunnableError &error) const
		{
			(m_Callable.*m_Method)(error);
		}

	private:

		Callable			&m_Callable;
		Callback			m_Method;

	};

	class AbstractDataCallbackHandler
	{
		AbstractDataCallbackHandler() {};
		virtual ~AbstractDataCallbackHandler() {} ;
		virtual void invoke(Data &data) const = 0;
	};

	template< typename Callable >
	class DataCallbackHandler : public AbstractDataCallbackHandler
	{

	public:

		typedef void (Callable::*Callback)(Data&);

		DataCallbackHandler(Callable &callable, Callback method) :
			m_Callable(callable),
			m_Method(method)
		{
		}

		~DataCallbackHandler() {}

		void invoke(Data &data) const
		{
			(m_Callable.*m_Method)(data);
		}

	private:

		Callable			&m_Callable;
		Callback			m_Method;

	};

	class AbstractStateChangeCallbackHandler
	{
		AbstractStateChangeCallbackHandler() {};
		virtual ~AbstractStateChangeCallbackHandler() {} ;
		virtual void invoke(unsigned int &state) const = 0;
	};

	template< typename Callable >
	class StateChangeCallbackHandler : public AbstractSateChangeCallbackHandler
	{

	public:

		typedef void (Callable::*Callback)(unsigned int&);

		StateChangeCallbackHandler(Callable &callable, Callback method) :
			m_Callable(callable),
			m_Method(method)
		{
		}

		~StateChangeCallbackHandler() {}

		void invoke(unsigned int &state) const
		{
			(m_Callable.*m_Method)(state);
		}

	private:

		Callable			&m_Callable;
		Callback			m_Method;

	};

}