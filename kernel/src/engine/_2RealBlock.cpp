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
#include "engine/_2RealSharedServiceInletMetainfo.h"
#include "engine/_2RealSharedServiceOutletMetainfo.h"
#include "engine/_2RealSharedServiceParameterMetainfo.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealOutlet.h"
#include "engine/_2RealParameter.h"
#include "engine/_2RealMultiInlet.h"

#include "engine/_2RealConditionFactory.h"

namespace _2Real
{
	Block::Block( std::shared_ptr< Threadpool > threads, std::shared_ptr< const SharedServiceMetainfo > meta, std::shared_ptr< AbstractSharedService > instance ) :
		enable_shared_from_this< Block >(),
		mMetainfo( meta ),
		mInstance( instance ),
		mStateMachine( threads, instance )
	{
	}

	Block::~Block()
	{
	}

	void Block::init()
	{
		std::vector< std::shared_ptr< const SharedServiceInletMetainfo > > inlets; mMetainfo.lock()->getInletMetainfos( inlets );
		std::vector< std::shared_ptr< const SharedServiceOutletMetainfo > > outlets; mMetainfo.lock()->getOutletMetainfos( outlets );
		std::vector< std::shared_ptr< const SharedServiceInletMetainfo > > parameters; mMetainfo.lock()->getParameterMetainfos( parameters );

		std::vector< std::shared_ptr< AbstractInlet > > mInlets;
		std::vector< std::shared_ptr< InSlot > > inletSlots;
		std::vector< std::shared_ptr< Outlet > > mOutlets;
		std::vector< std::shared_ptr< Parameter > > mParameters;
		std::vector< std::shared_ptr< InSlot > > paramSlots;

		for ( auto it : inlets )
		{
			std::shared_ptr< AbstractInlet > inlet;
			if ( it->isMultiInlet() )
				inlet.reset( new MultiInlet( it ) );
			else
				inlet.reset( new Inlet( it ) );

			inlet->init();

			mInlets.push_back( inlet );
			inletSlots.push_back( inlet );
		}

		for ( auto it : outlets )
		{
			std::shared_ptr< Outlet > outlet( new Outlet( it ) );
			outlet->init();

			mOutlets.push_back( outlet );
		}

		for ( auto it : inlets )
		{
			std::shared_ptr< Parameter > param( new Parameter( it ) );
			param->init();

			mParameters.push_back( param );
			paramSlots.push_back( param );
		}

		// ------ functions

		//std::vector< std::vector< std::string > > updatePolicy; mMetainfo.lock()->getDefaultUpdatePolicy( updatePolicy );
		//std::vector< std::vector< std::string > > setupPolicy; mMetainfo.lock()->getDefaultUpdatePolicy( setupPolicy );
		//mSetupTrigger.addCondition( ConditionFactory::createValuesCondition( paramSlots, setupPolicy ) );
		//mUpdateTrigger.addCondition( ConditionFactory::createValuesCondition( inletSlots, updatePolicy ) );
	}

	std::shared_ptr< const SharedServiceMetainfo > Block::getMetainfo() const
	{
		return mMetainfo.lock();
	}

	/*
	void Block::setup( std::function< void() > const& cb )
	{
		mStateMachine.setup( cb );
	}

	void Block::startUpdating( std::shared_ptr< UpdateTrigger > trigger, std::function< void() > const& cb )
	{
		mStateMachine.startRunning( trigger, cb );
	}

	void Block::stopUpdating( std::function< void() > const& cb )
	{
		mStateMachine.stopRunning( cb );
	}

	void Block::singleUpdate( std::function< void() > const& cb )
	{
		mStateMachine.singleUpdate( cb );
	}

	void Block::shutdown( std::function< void() > const& cb )
	{
		mStateMachine.shutdown( cb );
	}

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

}
