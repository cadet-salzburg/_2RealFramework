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

	class FunctionBlockStateManager;

	class AbstractFunctionBlockState
	{

	public:

		enum StateCode
		{
			CREATED,
			INITIALIZED,
			UPDATING,
			STOPPED,
			SHUTDOWN,
			EXCEPTION,
		};

		AbstractFunctionBlockState( const StateCode state ) : m_State( state ) {}
		virtual ~AbstractFunctionBlockState() {}

		virtual void setUp( FunctionBlockStateManager &states ) const = 0;
		virtual void start( FunctionBlockStateManager &states ) const = 0;
		virtual bool tryHalt( FunctionBlockStateManager &states ) const = 0;
		virtual bool tryStop( FunctionBlockStateManager &states ) const = 0;

		bool operator==( const StateCode state ) const { return m_State == state; }

	private:

		StateCode		const m_State;

	};

	class FunctionBlockStateCreated : public AbstractFunctionBlockState
	{

	public:

		FunctionBlockStateCreated() : AbstractFunctionBlockState( CREATED ) {}

		void setUp( FunctionBlockStateManager &states ) const;
		void start( FunctionBlockStateManager &states ) const;
		bool tryHalt( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;

	};

	class FunctionBlockStateInitialized : public AbstractFunctionBlockState
	{

	public:

		FunctionBlockStateInitialized() : AbstractFunctionBlockState( INITIALIZED ) {}

		void setUp( FunctionBlockStateManager &states ) const;
		void start( FunctionBlockStateManager &states ) const;
		bool tryHalt( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;

	};

	class FunctionBlockStateUpdating : public AbstractFunctionBlockState
	{

	public:

		FunctionBlockStateUpdating() : AbstractFunctionBlockState( UPDATING ) {}

		void setUp( FunctionBlockStateManager &states ) const;
		void start( FunctionBlockStateManager &states ) const;
		bool tryHalt( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;

	};

	class FunctionBlockStateStopped : public AbstractFunctionBlockState
	{

	public:

		FunctionBlockStateStopped() : AbstractFunctionBlockState( STOPPED ) {}

		void setUp( FunctionBlockStateManager &states ) const;
		void start( FunctionBlockStateManager &states ) const;
		bool tryHalt( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;

	};

	class FunctionBlockStateShutDown : public AbstractFunctionBlockState
	{

	public:

		FunctionBlockStateShutDown() : AbstractFunctionBlockState( SHUTDOWN ) {}

		void setUp( FunctionBlockStateManager &states ) const;
		void start( FunctionBlockStateManager &states ) const;
		bool tryHalt( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;

	};

	class FunctionBlockStateError : public AbstractFunctionBlockState
	{

	public:

		FunctionBlockStateError() : AbstractFunctionBlockState( EXCEPTION ) {}

		void setUp( FunctionBlockStateManager &states ) const;
		void start( FunctionBlockStateManager &states ) const;
		bool tryHalt( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;

	};

}