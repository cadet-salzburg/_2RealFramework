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

#include "_2RealFunctionBlockState.h"
#include "_2RealException.h"
#include "_2RealFunctionBlock.h"

#include <iostream>
#include <assert.h>

using std::cout;
using std::endl;

namespace _2Real
{

	bool FunctionBlockStateCreated::trySetUp( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateCreated::tryStart( FunctionBlockStateManager &states ) const
	{
		// is a user error ( setup before start )
		throw InvalidStateChangeException( "state change: created -> start" );
	}

	bool FunctionBlockStateCreated::tryStop( FunctionBlockStateManager &states ) const
	{
		// is a user error ( setup before stop )
		throw InvalidStateChangeException( "state change: created -> stop" );
	}

	void FunctionBlockStateCreated::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: created -> triggersAreOK" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateCreated::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: created -> uberBlocksAreOK" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateCreated::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: created -> scheduleUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateCreated::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: created -> beginUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateCreated::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: created -> update" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateCreated::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: created -> finishUpdate" << endl;
		assert( NULL );
#endif
	}

	bool FunctionBlockStateCreated::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateCreated::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: created -> shutDown" << endl;
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateSetUp::trySetUp( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateSetUp::tryStart( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateSetUp::tryStop( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateSetUp::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: set up -> triggersAreOK" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateSetUp::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: set up -> uberBlocksAreOK" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateSetUp::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: set up -> scheduleUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateSetUp::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: set up -> beginUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateSetUp::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: set up -> update" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateSetUp::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: set up -> finishUpdate" << endl;
		assert( NULL );
#endif
	}

	bool FunctionBlockStateSetUp::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateSetUp::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: set up -> shutDown" << endl;
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateStarted::trySetUp( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateStarted::tryStart( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateStarted::tryStop( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateStarted::triggersAreOk( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateStarted::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: started -> uberBlocksAreOK" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateStarted::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: started -> scheduleUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateStarted::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: started -> beginUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateStarted::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: started -> update" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateStarted::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: started -> finishUpdate" << endl;
		assert( NULL );
#endif
	}

	bool FunctionBlockStateStarted::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateStarted::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: started -> shutDown" << endl;
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateWaiting::trySetUp( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateWaiting::tryStart( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateWaiting::tryStop( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateWaiting::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: waiting -> triggersAreOk" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateWaiting::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateWaiting::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: waiting -> scheduleUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateWaiting::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: waiting -> beginUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateWaiting::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: waiting -> update" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateWaiting::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: waiting -> finishUpdate" << endl;
		assert( NULL );
#endif
	}

	bool FunctionBlockStateWaiting::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateWaiting::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: waiting -> shutDown" << endl;
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateScheduled::trySetUp( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateScheduled::tryStart( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateScheduled::tryStop( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateScheduled::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: scheduled -> triggersAreOk" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateScheduled::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: scheduled -> uberBlocksAreOk" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateScheduled::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: scheduled -> scheduleUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateScheduled::beginUpdate( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateScheduled::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: scheduled -> update" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateScheduled::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: scheduled -> finishUpdate" << endl;
		assert( NULL );
#endif
	}

	bool FunctionBlockStateScheduled::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateScheduled::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: scheduled -> shutDown" << endl;
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateUpdating::trySetUp( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateUpdating::tryStart( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	bool FunctionBlockStateUpdating::tryStop( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateUpdating::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: updating -> triggersAreOk" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateUpdating::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: updating -> uberBlocksAreOk" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateUpdating::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: updating -> scheduleUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateUpdating::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: updating -> beginUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateUpdating::update( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateUpdating::finishUpdate( FunctionBlockStateManager &states ) const
	{
	}

	bool FunctionBlockStateUpdating::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateUpdating::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: updating -> shutDown" << endl;
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateStopped::trySetUp( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateStopped::tryStart( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateStopped::tryStop( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateStopped::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: stopped -> triggersAreOk" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateStopped::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: stopped -> uberBlocksAreOk" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateStopped::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: stopped -> scheduleUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateStopped::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: stopped -> beginUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateStopped::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: stopped -> update" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateStopped::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: stopped -> finishUpdate" << endl;
		assert( NULL );
#endif
	}

	bool FunctionBlockStateStopped::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return false;
	}

	void FunctionBlockStateStopped::shutDown( FunctionBlockStateManager &states ) const
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateShutDown::trySetUp( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> setUp" );
	}

	bool FunctionBlockStateShutDown::tryStart( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> start" );
	}

	bool FunctionBlockStateShutDown::tryStop( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: shut down -> stop" );
	}

	void FunctionBlockStateShutDown::triggersAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: shut down -> triggersAreOk" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateShutDown::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: shut down -> uberBlocksAreOk" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateShutDown::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: shut down -> scheduleUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateShutDown::beginUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: shut down -> beginUpdate" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateShutDown::update( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: shut down -> update" << endl;
		assert( NULL );
#endif
	}

	void FunctionBlockStateShutDown::finishUpdate( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: shut down -> finishUpdate" << endl;
		assert( NULL );
#endif
	}

	bool FunctionBlockStateShutDown::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: shut down -> prepareForShutDown" << endl;
		assert( NULL );
#endif
		return false;
	}

	void FunctionBlockStateShutDown::shutDown( FunctionBlockStateManager &states ) const
	{
#ifdef _DEBUG
		cout << "invalid state transition: shut down -> shutDown" << endl;
		assert( NULL );
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool FunctionBlockStateError::trySetUp( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	bool FunctionBlockStateError::tryStart( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: error -> start" );
	}

	bool FunctionBlockStateError::tryStop( FunctionBlockStateManager &states ) const
	{
		throw InvalidStateChangeException( "state change: error -> stop" );
	}

	void FunctionBlockStateError::triggersAreOk( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateError::uberBlocksAreOk( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateError::scheduleUpdate( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateError::beginUpdate( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateError::update( FunctionBlockStateManager &states ) const
	{
	}

	void FunctionBlockStateError::finishUpdate( FunctionBlockStateManager &states ) const
	{
	}

	bool FunctionBlockStateError::prepareForShutDown( FunctionBlockStateManager &states ) const
	{
		return true;
	}

	void FunctionBlockStateError::shutDown( FunctionBlockStateManager &states ) const
	{
	}

}