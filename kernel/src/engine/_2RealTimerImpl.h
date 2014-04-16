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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealBoost.h"
#include "engine/_2RealUpdateTrigger_I.h"

#include <boost/asio.hpp>
#include <atomic>

namespace _2Real
{
	class TimerImpl : public UpdateTrigger_I
	{

	public:

		TimerImpl( boost::asio::io_service &service, const uint64_t period );
		~TimerImpl();

		void start();		
		void stop();

		void update();

		void enable() {}
		void disable() {}
		bool reset() { return false; }

		void registerToUpdate( std::shared_ptr< AbstractCallback_T< void > > );
		void unregisterFromUpdate( std::shared_ptr< AbstractCallback_T< void > > );

	private:

		uint64_t						mPeriod;
		boost::asio::deadline_timer		mTimer;
		bool							mShouldUpdate;
		mutable std::mutex				mMutex;
		Event_T< void >			mEvent;

	};
}
