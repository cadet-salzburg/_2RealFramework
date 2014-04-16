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

#include "engine/_2RealUpdatePolicyImpl.h"
#include "engine/_2RealUpdatePolicyMetainfoImpl.h"
#include "common/_2RealInletPolicy.h"
#include "engine/_2RealId.h"
#include "engine/_2RealBlockImpl.h"
#include "engine/_2RealInletImpl_I.h"
#include "engine/_2RealInletImpl.h"
#include "engine/_2RealMultiInletImpl.h"

namespace _2Real
{
	std::shared_ptr< UpdatePolicyImpl > UpdatePolicyImpl::createFromMetainfo( std::shared_ptr< BlockImpl > parent, std::shared_ptr< const UpdatePolicyMetainfoImpl > meta, std::vector< std::shared_ptr< InletImpl_I > > const& inlets )
	{
		std::shared_ptr< const InstanceId > id = InstanceId::create( meta->getId(), parent->getId(), InstanceType::POLICY, "update policy" );
		std::shared_ptr< UpdatePolicyImpl > policy( new UpdatePolicyImpl( parent, meta, id ) );
		
		policy->mInlets = inlets;

		for ( auto inlet : inlets )
		{
			if ( inlet->isMultiInlet() )
			{
				auto multiinlet = std::static_pointer_cast< MultiInletImpl >( inlet );
				multiinlet->registerToSubinletAdded( policy.get(), &UpdatePolicyImpl::subinletAdded );
				multiinlet->registerToSubinletRemoved( policy.get(), &UpdatePolicyImpl::subinletRemoved );
			}
		}

		std::vector< std::vector< UpdatePolicyMetainfoImpl::InletPolicyMetainfo > > condition = meta->getInternalRep();

		for ( auto const& andTerm : condition )
		{
			std::vector< UpdatePolicyImpl::InletPolicy > tmp;
			for ( auto const& inletRep : andTerm  )
			{
				std::shared_ptr< InletImpl_I > inlet;
				for ( auto actualInlet : inlets )
				{
					if ( actualInlet->getId()->getName() == inletRep.mName )
					{
						inlet = actualInlet;
						break;
					}
				}

				UpdatePolicyImpl::InletPolicy inletPolicy;
				inletPolicy.mInlet = inlet;
				inletPolicy.mWasUpdated = false;
				inletPolicy.mSubInletPolicy = inletRep.mPolicy;

				tmp.push_back( inletPolicy );
			}

			policy->mPolicy.push_back( tmp );
		}

		return policy;
	}

	UpdatePolicyImpl::UpdatePolicyImpl( std::shared_ptr< BlockImpl > parent, std::shared_ptr< const UpdatePolicyMetainfoImpl > meta, std::shared_ptr< const InstanceId > id ) :
		mParent( parent ),
		mMetainfo( meta ),
		mId( id ),
		mPolicy(),
		mIsAlreadyFulfilled( false ),
		mIsEnabled( true )
	{
	}

	UpdatePolicyImpl::~UpdatePolicyImpl()
	{
		for ( auto inlet : mInlets )
		{
			if ( inlet->isMultiInlet() )
			{
				auto multiinlet = std::static_pointer_cast< MultiInletImpl >( inlet );
				multiinlet->unregisterFromSubinletAdded( this, &UpdatePolicyImpl::subinletAdded );
				multiinlet->unregisterFromSubinletRemoved( this, &UpdatePolicyImpl::subinletRemoved );
			}
		}
	}

	std::shared_ptr< const InstanceId > UpdatePolicyImpl::getId() const
	{
		return mId;
	}

	std::shared_ptr< BlockImpl > UpdatePolicyImpl::getParent()
	{
		return mParent.lock();
	}

	void UpdatePolicyImpl::enable()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		mIsAlreadyFulfilled = false;
		mIsEnabled = true;

		// first, reset the policy
		for ( auto &andTerm : mPolicy )
		{
			for ( auto &inletPolicy : andTerm )
			{
				if ( inletPolicy.mInlet->isMultiInlet() )
				{
					for ( auto &subinletPolicy : inletPolicy.mSubInlets )
						subinletPolicy.mWasUpdated = false;
				}
				else
					inletPolicy.mWasUpdated = false;
			}
		}

		for ( auto inlet : mInlets )
		{
			if ( inlet->isMultiInlet() )
			{
				auto multiinlet = std::static_pointer_cast< MultiInletImpl >( inlet );

				for ( uint32_t idx = 0; idx < multiinlet->getSize(); ++idx )
				{
					auto subinlet = multiinlet->operator[]( idx );
					subinlet->registerToValueUpdated( this, &UpdatePolicyImpl::inletUpdated );
				}
			}
			else
			{
				auto regularinlet = std::static_pointer_cast< InletImpl >( inlet );
				regularinlet->registerToValueUpdated( this, &UpdatePolicyImpl::inletUpdated );
			}
		}
	}

	void UpdatePolicyImpl::disable()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		for ( auto inlet : mInlets )
		{
			if ( inlet->isMultiInlet() )
			{
				auto multiinlet = std::static_pointer_cast< MultiInletImpl >( inlet );

				for ( uint32_t idx = 0; idx < multiinlet->getSize(); ++idx )
				{
					auto subinlet = multiinlet->operator[]( idx );
					subinlet->unregisterFromValueUpdated( this, &UpdatePolicyImpl::inletUpdated );
				}
			}
			else
			{
				auto regularinlet = std::static_pointer_cast< InletImpl >( inlet );
				regularinlet->unregisterFromValueUpdated( this, &UpdatePolicyImpl::inletUpdated );
			}
		}
	}

	bool UpdatePolicyImpl::reset()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		// could already have been fulfilled
		// in that case, we don't even need to re-enable
		if ( mIsAlreadyFulfilled )
		{
			mIsAlreadyFulfilled = false;
			return true;
		}
		else
		{
			mIsEnabled = true;
			return false;
		}
	}

	void UpdatePolicyImpl::subinletAdded( std::shared_ptr< InletImpl > subinlet )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		subinlet->registerToValueUpdated( this, &UpdatePolicyImpl::inletUpdated );

		for ( auto &andTerm : mPolicy )
		{
			auto parentIter = std::find_if( andTerm.begin(), andTerm.end(), PtrCmp( subinlet->getParentInlet() ) );
			if ( parentIter == andTerm.end() )
				continue;

			InletPolicy policy;
			policy.mInlet = subinlet;
			policy.mWasUpdated = false;
			policy.mSubInletPolicy = SubinletPolicy::DISABLED;
			parentIter->mSubInlets.push_back( policy );
		}
	}

	void UpdatePolicyImpl::subinletRemoved( std::shared_ptr< InletImpl > subinlet )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		subinlet->unregisterFromValueUpdated( this, &UpdatePolicyImpl::inletUpdated );

		for ( auto &andTerm : mPolicy )
		{
			auto parentIter = std::find_if( andTerm.begin(), andTerm.end(), PtrCmp( subinlet->getParentInlet() ) );
			if ( parentIter == andTerm.end() )
				continue;

			auto inletIter = std::find_if( parentIter->mSubInlets.begin(), parentIter->mSubInlets.end(), PtrCmp( subinlet ) );
			if ( inletIter == parentIter->mSubInlets.end() )
				continue;

			parentIter->mSubInlets.erase( inletIter );
		}
	}

	void UpdatePolicyImpl::inletUpdated( std::shared_ptr< InletImpl > inlet )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		// ----one of the terms must be fulfilled ( init to false and logical OR )
		bool shouldFire = false;
		for ( auto &andTerm : mPolicy )
		{
			// ----every inlet in the term must fulfill the condition ( init to true and logical AND )
			bool termFulfilled = true;
			for ( auto &inletPolicy : andTerm )
			{
				if ( inletPolicy.mInlet->isMultiInlet() )
				{
					// ----multi inlet: check subinlets ( can only be any or all )
					if ( inletPolicy.mSubInletPolicy == SubinletPolicy::ANY )
					{
						bool subinletsOk = false;
						for ( auto &subinletPolicy : inletPolicy.mSubInlets )
						{
							if ( subinletPolicy.mInlet == inlet )
								subinletPolicy.mWasUpdated = true;

							subinletsOk |= true;
						}
						termFulfilled &= subinletsOk;
					}
					else if ( inletPolicy.mSubInletPolicy == SubinletPolicy::ALL )
					{
						bool subinletsOk = true;
						for ( auto &subinletPolicy : inletPolicy.mSubInlets )
						{
							if ( subinletPolicy.mInlet == inlet )
								subinletPolicy.mWasUpdated = true;

							subinletsOk &= subinletPolicy.mWasUpdated;
						}
						termFulfilled &= subinletsOk;
					}
					// ----multi inlet: check subinlets ( can only be any or all )
				}
				else
				{
					// ----regular inlet: easy
					if ( inletPolicy.mInlet == inlet )
						inletPolicy.mWasUpdated = true;
					termFulfilled &= inletPolicy.mWasUpdated;
					// ----regular inlet: easy
				}
			}
			// ----every inlet in the term must fulfill the condition ( init to true and logical AND )

			shouldFire |= termFulfilled;
		}
		// ----one of the terms must be fulfilled ( init to false and logical OR )

		if ( shouldFire )
		{
			// first, reset the policy
			for ( auto &andTerm : mPolicy )
			{
				for ( auto &inletPolicy : andTerm )
				{
					if ( inletPolicy.mInlet->isMultiInlet() )
					{
						for ( auto &subinletPolicy : inletPolicy.mSubInlets )
							subinletPolicy.mWasUpdated = false;
					}
					else
						inletPolicy.mWasUpdated = false;
				}
			}

			// now deal with the notification
			if ( mIsEnabled )
			{
				// keep recording updates, but don't trigger
				mIsEnabled = false;
				mEvent.notify();
			}
			else if ( !mIsEnabled )
			{
				// store that an update is ready
				mIsAlreadyFulfilled = true;
			}
		}
	}

	void UpdatePolicyImpl::set( const DefaultUpdatePolicy code )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		mIsAlreadyFulfilled = false;

		mPolicy.clear();
		std::vector< InletPolicy > tmp;
		switch( code )
		{
		case DefaultUpdatePolicy::ALL:
			for ( auto inlet : mInlets )
			{
				InletPolicy policy;
				policy.mInlet = inlet;
				policy.mWasUpdated = false;

				if ( inlet->isMultiInlet() )
				{
					std::shared_ptr< MultiInletImpl > multiinlet = std::static_pointer_cast< MultiInletImpl >( inlet );
					for ( uint32_t idx = 0; idx < multiinlet->getSize(); ++idx )
					{
						InletPolicy subinletPolicy;
						subinletPolicy.mInlet = multiinlet->operator[]( idx );
						subinletPolicy.mWasUpdated = false;
						subinletPolicy.mSubInletPolicy = SubinletPolicy::DISABLED;
						policy.mSubInlets.push_back( subinletPolicy );
					}
					
					policy.mSubInletPolicy = SubinletPolicy::ALL;
				}
				else
					policy.mSubInletPolicy = SubinletPolicy::DISABLED;

				tmp.push_back( policy );
			}
			mPolicy.push_back( tmp );
			break;
		case DefaultUpdatePolicy::ANY:
			for ( auto inlet : mInlets )
			{
				tmp.clear();

				InletPolicy policy;
				policy.mInlet = inlet;
				policy.mWasUpdated = false;

				if ( inlet->isMultiInlet() )
				{
					std::shared_ptr< MultiInletImpl > multiinlet = std::static_pointer_cast< MultiInletImpl >( inlet );
					for ( uint32_t idx = 0; idx < multiinlet->getSize(); ++idx )
					{
						InletPolicy subinletPolicy;
						subinletPolicy.mInlet = multiinlet->operator[]( idx );
						subinletPolicy.mWasUpdated = false;
						subinletPolicy.mSubInletPolicy = SubinletPolicy::DISABLED;
						policy.mSubInlets.push_back( subinletPolicy );
					}
					
					policy.mSubInletPolicy = SubinletPolicy::ALL;
				}
				else
					policy.mSubInletPolicy = SubinletPolicy::DISABLED;

				tmp.push_back( policy );
				mPolicy.push_back( tmp );
			}
			break;
		case DefaultUpdatePolicy::DISABLED:
			break;
		}
	}

	void UpdatePolicyImpl::registerToUpdate( std::shared_ptr< AbstractCallback_T< void > > cb )
	{
		mEvent.addListener( cb );
	}

	void UpdatePolicyImpl::unregisterFromUpdate( std::shared_ptr< AbstractCallback_T< void > > cb )
	{
		mEvent.removeListener( cb );
	}

}
