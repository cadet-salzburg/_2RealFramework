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

#include <future>
#include "enums/_2RealBlockState.h"

#include "engine/_2RealSharedService.h"

namespace _2Real
{

	class SharedServiceMetainfo;
	class UpdateTrigger;
	class Threadpool;
	class UpdatePolicy;
	class StateMachine;
	class InstanceId;

	class BlockIo
	{

	public:

		std::vector< std::shared_ptr< AbstractInlet > >		mInlets;
		std::vector< std::shared_ptr< Parameter > >			mParameters;
		std::vector< std::shared_ptr< Outlet > >			mOutlets;
		std::shared_ptr< AbstractSharedService >			mBlockObj;	

		void doSetup();
		void doUpdate();
		void doShutdown();
		void doNothing();

	};

	class Block : public std::enable_shared_from_this< Block >
	{

	public:

		~Block() = default;

		static std::shared_ptr< Block > createFromMetainfo( std::shared_ptr< const SharedServiceMetainfo > meta, std::shared_ptr< const InstanceId > id, std::shared_ptr< Threadpool > threads, const std::string name );

		std::future< BlockState > setup();
		std::future< BlockState > singlestep();
		std::future< BlockState > shutdown();

		std::future< BlockState > startUpdating();
		std::future< BlockState > startUpdating( std::shared_ptr< UpdateTrigger > );
		std::future< BlockState > stopUpdating();

		std::shared_ptr< BlockIo > getBlockIo();

	private:

		Block( std::shared_ptr< const InstanceId >, std::shared_ptr< Threadpool >, std::shared_ptr< BlockIo > );

		Block() = delete;
		Block( Block const& other ) = delete;
		Block operator=( Block const& other ) = delete;
		Block( Block && other ) = delete;
		Block& operator=( Block && other ) = delete;

		std::shared_ptr< const InstanceId >					mId;
		std::shared_ptr< UpdatePolicy >						mUpdatePolicy;
		std::shared_ptr< StateMachine >						mStateHandler;
		std::shared_ptr< BlockIo >							mIo;

	};

}