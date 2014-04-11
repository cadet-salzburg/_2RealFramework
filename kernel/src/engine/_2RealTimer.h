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
#include "helpers/_2RealBoost.h"
#include "engine/_2RealUpdateTrigger.h"

#include <boost/asio.hpp>
#include <atomic>

namespace _2Real
{
	class Timer : public UpdateTrigger
	{

	public:

		Timer( boost::asio::io_service &service, const uint64_t period );
		~Timer();

		void start();		
		void stop();

		void update();

		void enable() {}
		void disable() {}
		bool reset() { return false; }

	private:

		uint64_t						mPeriod;
		boost::asio::deadline_timer		mTimer;
		bool							mShouldUpdate;
		mutable std::mutex				mMutex;

	};
}
