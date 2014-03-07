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
#include "engine/_2RealStateMachine.h"

#include <future>
#include "enums/_2RealBlockState.h"

namespace _2Real
{

	class SharedServiceMetainfo;
	class AbstractSharedService;
	class UpdateTrigger;
	class Threadpool;

	class Block : public std::enable_shared_from_this< Block >
	{

	public:

		Block( std::shared_ptr< Threadpool > threads, std::shared_ptr< const SharedServiceMetainfo >, std::shared_ptr< AbstractSharedService > );
		~Block();

		void init();

		std::shared_ptr< const SharedServiceMetainfo >	getMetainfo() const;

		/*
		void setup( std::function< void() > const& );
		void startUpdating( std::shared_ptr< UpdateTrigger >, std::function< void() > const& );
		void stopUpdating( std::function< void() > const& );
		void singleUpdate( std::function< void() > const& );
		void shutdown( std::function< void() > const& );
		
		// called by engine
		void destroy( std::function< void() > const& );
		*/

		std::future< BlockState > setup();
		std::future< BlockState > singlestep();
		std::future< BlockState > shutdown();

	private:

		Block( Block const& other );
		Block operator=( Block const& other );

		std::weak_ptr< const SharedServiceMetainfo >		mMetainfo;
		std::shared_ptr< AbstractSharedService >			mInstance;

		StateMachine										mStateMachine;

	};

}