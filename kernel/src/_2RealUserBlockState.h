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

#include <string>
#include <sstream>

namespace _2Real
{

	class FunctionBlockStateManager;

	class AbstractFunctionBlockState
	{

	public:

		virtual ~AbstractFunctionBlockState() {}

		// carry out function block setup -> return true if it was carried out
		virtual bool trySetUp( FunctionBlockStateManager &states ) const = 0;
		// enables triggers -> return true if state was setup (?)
		virtual bool tryStart( FunctionBlockStateManager &states ) const = 0;
		// stops update cycle -> retrun true if it was carried out
		virtual bool tryStop( FunctionBlockStateManager &states ) const = 0;
		// all triggers -> ok
		virtual void triggersAreOk( FunctionBlockStateManager &states ) const = 0;
		// uber blocks -> ok
		virtual void uberBlocksAreOk( FunctionBlockStateManager &states ) const = 0;
		// request thread
		virtual void scheduleUpdate( FunctionBlockStateManager &states ) const = 0;
		// received thread -> carry out update
		virtual void beginUpdate( FunctionBlockStateManager &states ) const = 0;
		// update ( no state change )
		virtual void update( FunctionBlockStateManager &states ) const = 0;
		// release thread
		virtual void finishUpdate( FunctionBlockStateManager &states ) const = 0;
		// flag shutdown
		virtual bool prepareForShutDown( FunctionBlockStateManager &states ) const = 0;
		// perform shutdown
		virtual void shutDown( FunctionBlockStateManager &states ) const = 0;

	};

	// after ctor
	class FunctionBlockStateCreated : public AbstractFunctionBlockState
	{

	public:

		bool trySetUp( FunctionBlockStateManager &states ) const;
		bool tryStart( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;
		void triggersAreOk( FunctionBlockStateManager &states ) const;
		void uberBlocksAreOk( FunctionBlockStateManager &states ) const;
		void scheduleUpdate( FunctionBlockStateManager &states ) const;
		void beginUpdate( FunctionBlockStateManager &states ) const;
		void update( FunctionBlockStateManager &states ) const;
		void finishUpdate( FunctionBlockStateManager &states ) const;
		bool prepareForShutDown( FunctionBlockStateManager &states ) const;
		void shutDown( FunctionBlockStateManager &states ) const;

	};

	// after setup has been called the first time
	class FunctionBlockStateSetUp : public AbstractFunctionBlockState
	{

	public:

		bool trySetUp( FunctionBlockStateManager &states ) const;
		bool tryStart( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;
		void triggersAreOk( FunctionBlockStateManager &states ) const;
		void uberBlocksAreOk( FunctionBlockStateManager &states ) const;
		void scheduleUpdate( FunctionBlockStateManager &states ) const;
		void beginUpdate( FunctionBlockStateManager &states ) const;
		void update( FunctionBlockStateManager &states ) const;
		void finishUpdate( FunctionBlockStateManager &states ) const;
		bool prepareForShutDown( FunctionBlockStateManager &states ) const;
		void shutDown( FunctionBlockStateManager &states ) const;

	};

	// after start has been called
	class FunctionBlockStateStarted : public AbstractFunctionBlockState
	{

	public:

		bool trySetUp( FunctionBlockStateManager &states ) const;
		bool tryStart( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;
		void triggersAreOk( FunctionBlockStateManager &states ) const;
		void uberBlocksAreOk( FunctionBlockStateManager &states ) const;
		void scheduleUpdate( FunctionBlockStateManager &states ) const;
		void beginUpdate( FunctionBlockStateManager &states ) const;
		void update( FunctionBlockStateManager &states ) const;
		void finishUpdate( FunctionBlockStateManager &states ) const;
		bool prepareForShutDown( FunctionBlockStateManager &states ) const;
		void shutDown( FunctionBlockStateManager &states ) const;

	};

	// after triggersAreOk -> waiting for uber blocks
	class FunctionBlockStateWaiting : public AbstractFunctionBlockState
	{

	public:

		bool trySetUp( FunctionBlockStateManager &states ) const;
		bool tryStart( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;
		void triggersAreOk( FunctionBlockStateManager &states ) const;
		void uberBlocksAreOk( FunctionBlockStateManager &states ) const;
		void scheduleUpdate( FunctionBlockStateManager &states ) const;
		void beginUpdate( FunctionBlockStateManager &states ) const;
		void update( FunctionBlockStateManager &states ) const;
		void finishUpdate( FunctionBlockStateManager &states ) const;
		bool prepareForShutDown( FunctionBlockStateManager &states ) const;
		void shutDown( FunctionBlockStateManager &states ) const;

	};

	// after uberBlocksAreOk -> waiting for thread
	class FunctionBlockStateScheduled : public AbstractFunctionBlockState
	{

	public:

		bool trySetUp( FunctionBlockStateManager &states ) const;
		bool tryStart( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;
		void triggersAreOk( FunctionBlockStateManager &states ) const;
		void uberBlocksAreOk( FunctionBlockStateManager &states ) const;
		void scheduleUpdate( FunctionBlockStateManager &states ) const;
		void beginUpdate( FunctionBlockStateManager &states ) const;
		void update( FunctionBlockStateManager &states ) const;
		void finishUpdate( FunctionBlockStateManager &states ) const;
		bool prepareForShutDown( FunctionBlockStateManager &states ) const;
		void shutDown( FunctionBlockStateManager &states ) const;

	};

	// after beginUpdate -> has thread
	class FunctionBlockStateUpdating : public AbstractFunctionBlockState
	{

	public:

		bool trySetUp( FunctionBlockStateManager &states ) const;
		bool tryStart( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;
		void triggersAreOk( FunctionBlockStateManager &states ) const;
		void uberBlocksAreOk( FunctionBlockStateManager &states ) const;
		void scheduleUpdate( FunctionBlockStateManager &states ) const;
		void beginUpdate( FunctionBlockStateManager &states ) const;
		void update( FunctionBlockStateManager &states ) const;
		void finishUpdate( FunctionBlockStateManager &states ) const;
		bool prepareForShutDown( FunctionBlockStateManager &states ) const;
		void shutDown( FunctionBlockStateManager &states ) const;

	};

	// after prepareForShutdown
	class FunctionBlockStateStopped : public AbstractFunctionBlockState
	{

	public:

		bool trySetUp( FunctionBlockStateManager &states ) const;
		bool tryStart( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;
		void triggersAreOk( FunctionBlockStateManager &states ) const;
		void uberBlocksAreOk( FunctionBlockStateManager &states ) const;
		void scheduleUpdate( FunctionBlockStateManager &states ) const;
		void beginUpdate( FunctionBlockStateManager &states ) const;
		void update( FunctionBlockStateManager &states ) const;
		void finishUpdate( FunctionBlockStateManager &states ) const;
		bool prepareForShutDown( FunctionBlockStateManager &states ) const;
		void shutDown( FunctionBlockStateManager &states ) const;

	};

	// after shutDown
	class FunctionBlockStateShutDown : public AbstractFunctionBlockState
	{

	public:

		bool trySetUp( FunctionBlockStateManager &states ) const;
		bool tryStart( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;
		void triggersAreOk( FunctionBlockStateManager &states ) const;
		void uberBlocksAreOk( FunctionBlockStateManager &states ) const;
		void scheduleUpdate( FunctionBlockStateManager &states ) const;
		void beginUpdate( FunctionBlockStateManager &states ) const;
		void update( FunctionBlockStateManager &states ) const;
		void finishUpdate( FunctionBlockStateManager &states ) const;
		bool prepareForShutDown( FunctionBlockStateManager &states ) const;
		void shutDown( FunctionBlockStateManager &states ) const;

	};

	// error state
	class FunctionBlockStateError : public AbstractFunctionBlockState
	{

	public:

		bool trySetUp( FunctionBlockStateManager &states ) const;
		bool tryStart( FunctionBlockStateManager &states ) const;
		bool tryStop( FunctionBlockStateManager &states ) const;
		void triggersAreOk( FunctionBlockStateManager &states ) const;
		void uberBlocksAreOk( FunctionBlockStateManager &states ) const;
		void scheduleUpdate( FunctionBlockStateManager &states ) const;
		void beginUpdate( FunctionBlockStateManager &states ) const;
		void update( FunctionBlockStateManager &states ) const;
		void finishUpdate( FunctionBlockStateManager &states ) const;
		bool prepareForShutDown( FunctionBlockStateManager &states ) const;
		void shutDown( FunctionBlockStateManager &states ) const;

	};

}