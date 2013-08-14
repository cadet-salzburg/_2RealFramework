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

#include "policies/_2RealUpdatePolicy.h"
#include "engine/_2RealAbstractUpdateManager.h"
#include "helpers/_2RealPocoIncludes.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{

	class Inlet;
	class BasicInletIO;
	class OutletIO;
	class FunctionBlockIOManager;
	class FunctionBlockStateManager;
	class EngineImpl;
	class FunctionBlock;
	class Timer;
	class ADataCondition;

	class FunctionBlockUpdateManager : public AbstractUpdateManager
	{

	public:

		FunctionBlockUpdateManager( EngineImpl *engine, FunctionBlock *owner );
		~FunctionBlockUpdateManager();

		std::string const&							getFullHumanReadableName() const;
		std::string const&							getHumanReadableName() const;
		//std::string const&							getCode() const;

		void inletAdded( std::shared_ptr< BasicInletIO >, UpdatePolicy const& );
		void inletRemoved( std::shared_ptr< BasicInletIO > );
		void policyChanged( std::shared_ptr< BasicInletIO >, UpdatePolicy const& );
		void setUpdateTimer( std::shared_ptr< Timer > );

		void timerSignal( long );
		void inletChanged();

		void enableUpdates( const bool enabled );

	private:

		void resetInlets();
		bool areInletsOk();
		bool areUpdatesEnabled() const;

		friend class FunctionBlock;

		EngineImpl									*const mEngineImpl;
		FunctionBlock								*const mOwningBlock;

		std::weak_ptr< FunctionBlockStateManager >		mStateManager;

		mutable Poco::FastMutex															mUpdateAccess;
		bool																			mAreUpdatesEnabled;
		mutable Poco::FastMutex															mAccess;
		std::map< std::shared_ptr< BasicInletIO >, std::shared_ptr< ADataCondition > >	mInlets;
		std::shared_ptr< Timer >														mUpdateTimer;
		bool																			mAreInletsOk;
		bool																			mIsTimerOk;

	};
}
