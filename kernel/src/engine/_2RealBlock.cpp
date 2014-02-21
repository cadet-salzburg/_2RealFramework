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

// add
#include "helpers/_2RealEvent_T.h"

namespace _2Real
{

	class Condition
	{

	public:

		virtual ~Condition() {}

		template< typename TCallable >
		void registerToFulfilled( TCallable &callable, void ( TCallable::*callback )( void ) )
		{
			std::shared_ptr< AbstractCallback_T< void > listener( new MemberCallback_T< TCallable, void >( callable, callback ) );
			mFulfilledEvent.addListener( listener )
		}

		template< typename TCallable >
		void unregisterFromFulfilled( TCallable &callable, void ( TCallable::*callback )( void ) )
		{
			std::shared_ptr< AbstractCallback_T< void > listener( new MemberCallback_T< TCallable, void >( callable, callback ) );
			mFulfilledEvent.addListener( listener )
		}

	protected:

		void isFulfilled()
		{
			mFulfilledEvent.notify();
		}

	private:

		Event_T< void >	mFulfilledEvent;

	};

	class TimerCondition : public Condition
	{

	public:

		void timerSignalReceived()
		{
			// check conds

			Condition::isFulfilled();
		}

	};

	class UserCondition : public Condition
	{

	public:

		void userInputReceived()
		{
			// check conds

			Condition::isFulfilled();
		}

	};

	class ValuesCondition : public Condition
	{

	public:

		void ValuesCondition::init( std::vector< std::shared_ptr< InSlot > > inputs, std::vector< std::vector< std::string > > &disjunction )
		{
			mFlags.resize( inputs.size(), false );

			mDisjunction.reserve( disjunction.size() );
			for ( auto conjunction : disjunction )
			{
				std::vector< unsigned int > tmp; tmp.reserve( conjunction.size() );
				for ( auto name : conjunction )
					for ( unsigned int id=0; id<inputs.size(); ++id )
						if ( inputs[ id ]->getName() == name )
							tmp.push_back( id );
				mDisjunction.push_back( tmp );
			}

			for ( auto in : inputs )
				in->registerToValueUpdated( *this, &ValuesCondition::newValueReceived );
		}

		void ValuesCondition::cleanup( std::vector< std::shared_ptr< InSlot > > inputs )
		{
			for ( auto in : inputs )
				in->unregisterFromValueUpdated( *this, &ValuesCondition::newValueReceived );
		}

		void newValueReceived( const unsigned int id )
		{
			if ( mFlags[ id ] ) return;
			mFlags[ id ] = true;

			bool isDisjunctionTrue = false;
			for ( auto conjunction : mDisjunction )
			{
				bool isConjunctionTrue = true;
				for ( auto var : conjunction )
				{
					isConjunctionTrue &= mFlags[ var ];
					if ( !isConjunctionTrue ) break;
				}

				isDisjunctionTrue |= isConjunctionTrue;
				if ( isDisjunctionTrue ) break;
			}

			if ( isDisjunctionTrue )
				Condition::isFulfilled();
		}

	private:

		std::vector< std::vector< unsigned int > >	mDisjunction;
		std::vector< bool >							mFlags;

	};

	class ConditionFactory
	{

	public:

		// timer: register to timer
		static std::shared_ptr< Condition > createTimerCondition();
		// user input: register to func
		static std::shared_ptr< Condition > createUserInputCondition();

		static std::shared_ptr< Condition > createValuesCondition( std::vector< std::shared_ptr< InSlot > > inputs, std::vector< std::vector< std::string > > &disjunction )
		{
			std::shared_ptr< ValuesCondition > c;
			c->init( inputs, disjunction );
			return c;
		}

		static void removeValuesCondition( std::shared_ptr< Condition > condition, std::vector< std::shared_ptr< InSlot > > inputs )
		{
			std::shared_ptr< ValuesCondition > c = std::static_pointer_cast< ValuesCondition >( condition );
			c->cleanup( inputs );
		}
	};

	class StateChangeTrigger
	{

	public:

		void addCondition( std::shared_ptr< Condition > condition )
		{
			// possible mutex

			mConditions.insert( condition );
		}

		void removeCondition( std::shared_ptr< Condition > condition )
		{
			// possible mutex

			mConditions.insert( condition );
		}

		void onConditionFulfilled( std::shared_ptr< Condition > cond )
		{
			// here, it is possible to to sth with conditions

			mFulfilledEvent.notify();
		}

		template< typename TCallable >
		void registerToConditionsFulfilled( TCallable &callable, void ( TCallable::*callback )( void ) )
		{
			std::shared_ptr< AbstractCallback_T< void > > listener( new MemberCallback_T< TCallable, void >( callable, callback ) );
			mFulfilledEvent.addListener( listener );
		}

		template< typename TCallable >
		void unregisterFromConditionsFulfilled( TCallable &callable, void ( TCallable::*callback )( void ) )
		{
			std::shared_ptr< AbstractCallback_T< void > > listener( new MemberCallback_T< TCallable, void >( callable, callback ) );
			mFulfilledEvent.removeListener( listener );
		}

	private:

		std::set< std::shared_ptr< Condition > >	mConditions;
		Event_T< void >								mFulfilledEvent;

	};

	class ThreadPool
	{
	};

	class AbstractState
	{

	public:

		virtual ~AbstractState();

		virtual void init() = 0;

		virtual std::shared_ptr< AbstractState > onThreadAvailable() = 0;
		virtual std::shared_ptr< AbstractState > onSetupSignalReceived() = 0;
		virtual std::shared_ptr< AbstractState > onUpdateSignalReceived() = 0;
		virtual std::shared_ptr< AbstractState > onDestroySignalReceived() = 0;

	};

	class PreSetupState;
	class PreSetupState_NeedsThread;
	class NeedsInput;
	class SetupState_NeedsThread;
	class UpdateState_NeedsThread;
	class ShutdownState_NeedsThread;
	class PostShutdownState;

	class StateData
	{
	public:
		void registerToThreadAvailable();
		void registerToUserInput();
		void registerToUpdateCondition();
		void registerToSetupCondition();
	private:
	};

	// init must either receive a signal to setup or to kill itself
	class PreSetupState : public AbstractState
	{

	public:

		PreSetupState( std::shared_ptr< StateData > data ) : AbstractState(), mData( data )
		{
		}

		void init()
		{
			mData->registerToSetupCondition();
		}

		std::shared_ptr< AbstractState > onThreadAvailable()	
		{
			// debug assert
		}

		std::shared_ptr< AbstractState > onSetupSignalReceived()
		{
			std::shared_ptr< PreSetupState_NeedsThread > nextState( new PreSetupState_NeedsThread( mData ) );

			// this should only happen once the next state has been set !!
			// move to an init method?
			mThreadPool.lock()->registerToThreadAvailable( nextState, &PreSetupState_NeedsThread::onThreadAvailable );

			return nextState;
		}

		std::shared_ptr< AbstractState > onUpdateSignalReceived()
		{
			// user error!
		}

		std::shared_ptr< AbstractState > onDestroySignalReceived()
		{
			std::shared_ptr< PreSetupState_NeedsThread > nextState( new PostShutdownState( mData ) );
		}

	private:

		std::shared_ptr< StateData >	mData;

	};

	// this state is in essence waiting for the thread to carry out the very first setup
	class PreSetupState_NeedsThread : public AbstractState
	{
	public:

		std::shared_ptr< AbstractState > onThreadReady()	
		{
			// setup
			// unregister from thread
			// no go into default state

			// register to all kinds of shit
		}

		std::shared_ptr< AbstractState > onSetupSignalReceived()
		{
			// that's weird
		}

		std::shared_ptr< AbstractState > onUpdateSignalReceived()
		{
			// exception. user input error?
		}

		std::shared_ptr< AbstractState > onDestroySignalReceived()
		{
			// go directly to kill state
		}
	};

	// waiting for inputs
	class DefaultState : public AbstractState
	{
	public:

		std::shared_ptr< AbstractState > onThreadReady()	
		{
			// exception
		}

		std::shared_ptr< AbstractState > onSetupSignalReceived()
		{
			// well, let's set ourselves up

			// unregister from setup / update signals
			// request a thread
			// go to setup state
		}

		std::shared_ptr< AbstractState > onUpdateSignalReceived()
		{
			// well, let's update ourselves

			// unregister from setup / update signals
			// request a thread
			// go to update
		}

		std::shared_ptr< AbstractState > onDestroySignalReceived()
		{
			// well, let's shutdown ourselves

			// unregister from setup / update / shutdown signals
			// request a thread
			// go to shutdown state
		}
	};

	class SetupState : public AbstractState
	{
	public:

		std::shared_ptr< AbstractState > onThreadReady()	
		{
		}

		std::shared_ptr< AbstractState > onSetupSignalReceived()
		{
		}

		std::shared_ptr< AbstractState > onUpdateSignalReceived()
		{
		}

		std::shared_ptr< AbstractState > onDestroySignalReceived()
		{
		}
	};

	class UpdateState : public AbstractState
	{
	public:

		std::shared_ptr< AbstractState > onThreadReady()	
		{
		}

		std::shared_ptr< AbstractState > onSetupSignalReceived()
		{
		}

		std::shared_ptr< AbstractState > onUpdateSignalReceived()
		{
		}

		std::shared_ptr< AbstractState > onDestroySignalReceived()
		{
		}
	};

	class ShutdownState : public AbstractState
	{
	public:

		std::shared_ptr< AbstractState > onThreadReady()	
		{
		}

		std::shared_ptr< AbstractState > onSetupSignalReceived()
		{
		}

		std::shared_ptr< AbstractState > onUpdateSignalReceived()
		{
		}

		std::shared_ptr< AbstractState > onDestroySignalReceived()
		{
		}
	};

	class PostShutdownState : public AbstractState
	{
	public:

		std::shared_ptr< AbstractState > onThreadReady()	
		{
		}

		std::shared_ptr< AbstractState > onSetupSignalReceived()
		{
		}

		std::shared_ptr< AbstractState > onUpdateSignalReceived()
		{
		}

		std::shared_ptr< AbstractState > onDestroySignalReceived()
		{
		}
	};


	class StateMachine
	{

	public:

		void init();

		void onThreadReady()		// internal
		{
			std::shared_ptr< AbstractState > followUpState = mCurrentState->onThreadReady();
		}

		void onSetupSignalReceived()
		{
		}

		void onUpdateSignalReceived()
		{
			std::shared_ptr< AbstractState > followUpState = mCurrentState->onUpdateSignalReceived();
		}

		void onShutdownSignalReceived()
		{
		}

	private:

		std::shared_ptr< AbstractState >		mCurrentState;

	};

	Block::Block( std::shared_ptr< const SharedServiceMetainfo > meta, std::shared_ptr< AbstractSharedService > instance ) :
		enable_shared_from_this< Block >(),
		mMetainfo( meta ), mInstance( instance )
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

		// build inlets
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

		// move to members
		StateMachine *mStateMachine = new StateMachine;

		// problem is, i could change the update policy shit during runtime

		// ------ policies

		std::vector< std::vector< std::string > > updatePolicy; mMetainfo.lock()->getDefaultUpdatePolicy( updatePolicy );
		std::vector< std::vector< std::string > > setupPolicy; mMetainfo.lock()->getDefaultUpdatePolicy( setupPolicy );

		std::shared_ptr< StateChangeTrigger > updateTrigger( new StateChangeTrigger );
		updateTrigger->registerToConditionsFulfilled( *mStateMachine, &StateMachine::onUpdateSignalReceived );

		updateTrigger->addCondition( ConditionFactory::createValuesCondition( inletSlots, updatePolicy ) );
		updateTrigger->addCondition( ConditionFactory::createUserInputCondition( ) );

		std::shared_ptr< StateChangeTrigger > setupTrigger( new StateChangeTrigger );
		setupTrigger->registerToConditionsFulfilled( *mStateMachine, &StateMachine::onSetupSignalReceived );

		setupTrigger->addCondition( ConditionFactory::createValuesCondition( paramSlots, setupPolicy ) );
		setupTrigger->addCondition( ConditionFactory::createUserInputCondition() );

		std::shared_ptr< StateChangeTrigger > shutdownTrigger( new StateChangeTrigger );
		shutdownTrigger->registerToConditionsFulfilled( *mStateMachine, &StateMachine::onShutdownSignalReceived );

		shutdownTrigger->addCondition( ConditionFactory::createUserInputCondition() );

		// ok, the fucking state machine....

	}

	std::shared_ptr< const SharedServiceMetainfo > Block::getMetainfo() const
	{
		return mMetainfo.lock();
	}

}
