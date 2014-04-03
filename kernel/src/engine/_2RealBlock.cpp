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

#include "engine/_2RealBlock.h"
#include "engine/_2RealSharedServiceMetainfo.h"
#include "engine/_2RealSharedServiceIoSlotMetainfo.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealParameter.h"
#include "engine/_2RealMultiInlet.h"

namespace _2Real
{

	void BlockIo::doNothing()
	{
	}

	void BlockIo::doUpdate()
	{
		for ( auto it : mInlets )
		{
			it->update();
		}

		mBlockObj->update();
	}

	void BlockIo::doSetup()
	{
		for ( auto it : mParameters )
			it->update();

		mBlockObj->setup();
	}

	void BlockIo::doShutdown()
	{
		mBlockObj->shutdown();
	}

	Block::Block( std::shared_ptr< const SharedServiceMetainfo > meta, std::shared_ptr< Threadpool > threads, std::shared_ptr< BlockIo > io ) :
		enable_shared_from_this< Block >(),
		mMetainfo( meta ),
		mStateMachine( threads, io ),
		mIo( io )
	{
	}

	Block::~Block()
	{
	}

	void Block::createIo( std::shared_ptr< const SharedServiceMetainfo > info, std::vector< std::shared_ptr< Parameter > > &parameters, std::vector< std::shared_ptr< AbstractInlet > > &inlets, std::vector< std::shared_ptr< Outlet > > &outlets )
	{
		auto inletinfos = info->getInletMetainfos();
		auto outletinfos = info->getOutletMetainfos();
		auto parameterinfos = info->getParameterMetainfos();

		for ( auto it : inletinfos )
			inlets.push_back( it->isMulti() ? std::shared_ptr< AbstractInlet >( new MultiInlet( it ) ) : std::shared_ptr< AbstractInlet >( new Inlet( it ) ) );

		for ( auto it : outletinfos )
			outlets.push_back( std::shared_ptr< Outlet >( new Outlet( it ) ) );

		for ( auto it : parameterinfos )
			parameters.push_back( std::shared_ptr< Parameter >( new Parameter( it ) ) );
	}

	std::shared_ptr< const SharedServiceMetainfo > Block::getMetainfo() const
	{
		return mMetainfo.lock();
	}

	/*
	void Block::destroy( std::function< void() > const& cb )
	{
		// TODO: immediately unregister the state machine from
		// all triggers

		// mUpdateTrigger->unregisterFromSignal()
	}
	*/

	std::future< BlockState > Block::setup()
	{
		return mStateMachine.setup();
	}

	std::future< BlockState > Block::singlestep()
	{
		return mStateMachine.singlestep();
	}

	std::future< BlockState > Block::shutdown()
	{
		return mStateMachine.shutdown();
	}

	std::future< BlockState > Block::startUpdating( std::shared_ptr< UpdateTrigger > trigger )
	{
		return mStateMachine.startRunning( trigger );
	}

	std::future< BlockState > Block::stopUpdating()
	{
		return mStateMachine.stopRunning();
	}

	std::shared_ptr< BlockIo > Block::getBlockIo()
	{
		return mIo;
	}

}
