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

#include "helpers/_2RealStdIncludes.h"
#include "helpers/_2RealCallback_T.h"

namespace _2Real
{
	class UpdateTrigger;

	class Action
	{
	public:
		enum Id { DO_SETUP, DO_UPDATE, DO_SHUTDOWN, DO_NOTHING };		
	};

	class Signal
	{
	public:
		enum Id { SETUP, SHUTDOWN, UPDATE, START_RUNNING, STOP_RUNNING, ENGINE_SHUTDOWN };
	};

	class State
	{
	public:
		enum Id { PRE_SETUP, POST_SETUP, POST_SETUP_RUNNING, POST_SHUTDOWN };
	};

	typedef std::shared_ptr< AbstractCallback_T< void > > CbPtr;

	struct SignalResponse
	{
		Action::Id							action;
		State::Id							followupState;
		bool								shouldWait;		// alternative: ignore
		CbPtr								onFininshed;	
		std::shared_ptr< UpdateTrigger >	updateTrigger;
	};

	////////////////////////////////////////////
	// Thread!!!
	////////////////////////////////////////////

	class Thread
	{
	};

	class Threadpool
	{

	public:

		// nonblocking
		void requestThread( std::shared_ptr< AbstractCallback_T< std::shared_ptr< Thread > > > );

	};

	class ActionRequest
	{

	public:

		void start( /*service and function*/ std::shared_ptr< Threadpool >, CbPtr );
		void onThreadReady( std::shared_ptr< Thread > );

	private:

		// oh dear, now we need the fucking threadpool

		Action::Id						mAction;
		CbPtr							mCallback;
		std::shared_ptr< Thread >		mThread;

	};

}