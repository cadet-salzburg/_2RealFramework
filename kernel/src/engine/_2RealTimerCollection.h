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

#include "common/_2RealBoost.h"
#include "common/_2RealStdIncludes.h"

#include <boost/asio.hpp>
#include <thread>

namespace _2Real
{
	class TimerImpl;

	class TimerCollection : public std::enable_shared_from_this< TimerCollection >
	{
	
	public:

		TimerCollection();
		~TimerCollection();

		TimerCollection( TimerCollection const& other ) = delete;
		TimerCollection( TimerCollection && other ) = delete;
		TimerCollection& operator=( TimerCollection const& other ) = delete;
		TimerCollection& operator=( TimerCollection && other ) = delete;

		std::shared_ptr< TimerImpl >	createTimer( const double fps );

		void run();

	private:

		typedef std::set< std::shared_ptr< TimerImpl > > Timers;

		Timers												mTimers;
		boost::asio::io_service								mIoService;
		std::shared_ptr< boost::asio::io_service::work >	mWork;
		std::thread											mThread;

	};
}