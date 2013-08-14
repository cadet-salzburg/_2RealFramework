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

#include "engine/_2RealAbstractStateManager.h"
#include "helpers/_2RealSynchronizedBool.h"
#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealException.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	namespace bundle
	{
		class Block;
	}

	class ThreadPool;
	class Logger;
	class Exception;
	class FunctionBlockUpdateManager;
	class AbstractFunctionBlockState;
	class FunctionBlockIOManager;
	class PooledThread;
	class FunctionBlockStateManager;
	class FunctionBlock;

	typedef void ( FunctionBlockStateManager::*FunctionToExecute )();

	struct ThreadExecRequest
	{
		ThreadExecRequest( FunctionBlockStateManager &mgr, FunctionToExecute func ) : block( mgr ), function( func ), event( nullptr ) {}

		FunctionBlockStateManager	&block;
		FunctionToExecute			function;
		Poco::Event					*event;
	};

	class FunctionBlockStateManager : public AbstractStateManager
	{

	public:

		FunctionBlockStateManager( EngineImpl *engine, FunctionBlock *owner, const bool thread );
		~FunctionBlockStateManager();

		std::string const&							getFullHumanReadableName() const;
		std::string const&							getHumanReadableName() const;
		//std::string const&							getCode() const;

		void setUp();
		void start();
		Poco::Event & stop();
		void prepareForShutDown();
		bool shutDown( const long timeout );
		//void singleStep();

		void updateFunctionBlock();
		void singleStepFunctionBlock();
		void setupFunctionBlock();
		void shutdownFunctionBlock();

		void scheduleUpdate();

	private:

		friend class FunctionBlock;

		void handleStateChangeException( Exception &e );

		EngineImpl									*const mEngineImpl;
		FunctionBlock								*const mOwningBlock;

		std::weak_ptr< FunctionBlockIOManager >		mIOManager;
		std::weak_ptr< FunctionBlockUpdateManager >	mUpdateManager;
		std::weak_ptr< bundle::Block >				mFunctionBlock;

		mutable Poco::FastMutex				m_StateAccess;
		AbstractFunctionBlockState			*m_CurrentState;

		SynchronizedBool					m_IsFlaggedForHalting;			// if set, block will stop after current update cycle
		SynchronizedBool					m_IsFlaggedForShutdown;			// if set, block will shut itself doen after current update cycle

		Poco::Event							m_StopEvent;
		Poco::Event							m_ShutdownEvent;

		PooledThread						*m_Thread;

		Poco::FastMutex						m_ExceptionAccess;
		bool								m_HasException;
		Exception							m_Exception;

	};

}