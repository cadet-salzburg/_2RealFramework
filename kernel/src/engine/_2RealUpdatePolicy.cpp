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

#include "engine/_2RealUpdatePolicy.h"
#include "engine/_2RealInletPolicy.h"
#include "engine/_2RealId.h"
#include "engine/_2RealBlock.h"
#include "engine/_2RealAbstractInlet.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealMultiInlet.h"

namespace _2Real
{
	std::shared_ptr< UpdatePolicyMetainfo > UpdatePolicyMetainfo::make( std::shared_ptr< const MetainfoId > id, const std::vector< InletDeclaration > inlets )
	{
		std::shared_ptr< const MetainfoId > policyId = MetainfoId::create( id, MetainfoType::POLICY, "policy" );
		std::shared_ptr< UpdatePolicyMetainfo > policyInfo( new UpdatePolicyMetainfo( policyId ) );

		for ( auto const& it : inlets )
			policyInfo->mInlets.insert( std::make_pair( it.mName, it ) );

		return policyInfo;
	}

	UpdatePolicyMetainfo::UpdatePolicyMetainfo( std::shared_ptr< const MetainfoId > id ) :
		mId( id ),
		mInlets(),
		mPolicy()
	{
	}

	void UpdatePolicyMetainfo::set( const DefaultPolicy policy )
	{
		mPolicy.clear();
		std::vector< InletPolicyMetainfo > tmp;
		switch( policy )
		{
		case DefaultPolicy::ALL:
			for ( auto it : mInlets )
			{
				InletPolicyMetainfo info;
				info.mName = it.second.mIsMulti;
				info.mIsMulti = it.second.mIsMulti;
				info.mPolicy = it.second.mIsMulti ? SubInletPolicy::ALL : SubInletPolicy::DISABLED;
				tmp.push_back( info );
			}
			mPolicy.push_back( tmp );
			break;
		case DefaultPolicy::ANY:
			for ( auto it : mInlets )
			{
				tmp.clear();
				InletPolicyMetainfo info;
				info.mName = it.second.mIsMulti;
				info.mIsMulti = it.second.mIsMulti;
				info.mPolicy = it.second.mIsMulti ? SubInletPolicy::ANY : SubInletPolicy::DISABLED;
				tmp.push_back( info );
				mPolicy.push_back( tmp );
			}
			break;
		case DefaultPolicy::DISABLED:
			break;
		}
	}

	std::vector< std::vector< UpdatePolicyMetainfo::InletPolicyMetainfo > > UpdatePolicyMetainfo::getInternalRep() const
	{
		return mPolicy;
	}

	std::shared_ptr< const MetainfoId >	UpdatePolicyMetainfo::getId() const
	{
		return mId;
	}

	std::shared_ptr< UpdatePolicy > UpdatePolicy::createFromMetainfo( std::shared_ptr< Block > parent, std::shared_ptr< const UpdatePolicyMetainfo > meta, std::vector< std::shared_ptr< AbstractInlet > > const& inlets )
	{
		std::shared_ptr< const InstanceId > id = InstanceId::create( meta->getId(), parent->getId(), InstanceType::POLICY, "update policy" );
		std::shared_ptr< UpdatePolicy > policy( new UpdatePolicy( parent, meta, id ) );
		
		policy->mInlets = inlets;

		for ( auto inlet : inlets )
		{
			if ( inlet->isMultiInlet() )
			{
				auto multiinlet = std::dynamic_pointer_cast< MultiInlet >( inlet );
				multiinlet->registerToSubinletAdded( policy.get(), &UpdatePolicy::subinletAdded );
				multiinlet->registerToSubinletRemoved( policy.get(), &UpdatePolicy::subinletRemoved );
			}
		}

		std::vector< std::vector< UpdatePolicyMetainfo::InletPolicyMetainfo > > condition = meta->getInternalRep();

		for ( auto const& andTerm : condition )
		{
			std::vector< UpdatePolicy::InletPolicy > tmp;
			for ( auto const& inletRep : andTerm  )
			{
				std::shared_ptr< AbstractInlet > inlet;
				for ( auto actualInlet : inlets )
				{
					if ( actualInlet->getId()->getName() == inletRep.mName )
					{
						inlet = actualInlet;
						break;
					}
				}

				UpdatePolicy::InletPolicy inletPolicy;
				inletPolicy.mInlet = inlet;
				inletPolicy.mWasUpdated = false;
				inletPolicy.mSubInletPolicy = inletRep.mPolicy;

				tmp.push_back( inletPolicy );
			}

			policy->mPolicy.push_back( tmp );
		}

		return policy;
	}

	UpdatePolicy::UpdatePolicy( std::shared_ptr< Block > parent, std::shared_ptr< const UpdatePolicyMetainfo > meta, std::shared_ptr< const InstanceId > id ) :
		mParent( parent ),
		mMetainfo( meta ),
		mId( id ),
		mPolicy(),
		mIsAlreadyFulfilled( false ),
		mIsEnabled( true )
	{
	}

	UpdatePolicy::~UpdatePolicy()
	{
		for ( auto inlet : mInlets )
		{
			if ( inlet->isMultiInlet() )
			{
				auto multiinlet = std::dynamic_pointer_cast< MultiInlet >( inlet );
				multiinlet->unregisterFromSubinletAdded( this, &UpdatePolicy::subinletAdded );
				multiinlet->unregisterFromSubinletRemoved( this, &UpdatePolicy::subinletRemoved );
			}
		}
	}

	std::shared_ptr< const InstanceId > UpdatePolicy::getId() const
	{
		return mId;
	}

	std::shared_ptr< Block > UpdatePolicy::getParent()
	{
		return mParent.lock();
	}

	void UpdatePolicy::enable()
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
				auto multiinlet = std::dynamic_pointer_cast< MultiInlet >( inlet );

				for ( uint32_t idx = 0; idx < multiinlet->getSize(); ++idx )
				{
					auto subinlet = multiinlet->operator[]( idx );
					subinlet->registerToValueUpdated( this, &UpdatePolicy::inletUpdated );
				}
			}
			else
			{
				auto regularinlet = std::dynamic_pointer_cast< Inlet >( inlet );
				regularinlet->registerToValueUpdated( this, &UpdatePolicy::inletUpdated );
			}
		}
	}

	void UpdatePolicy::disable()
	{
		std::lock_guard< std::mutex > lock( mMutex );

		for ( auto inlet : mInlets )
		{
			if ( inlet->isMultiInlet() )
			{
				auto multiinlet = std::dynamic_pointer_cast< MultiInlet >( inlet );

				for ( uint32_t idx = 0; idx < multiinlet->getSize(); ++idx )
				{
					auto subinlet = multiinlet->operator[]( idx );
					subinlet->unregisterFromValueUpdated( this, &UpdatePolicy::inletUpdated );
				}
			}
			else
			{
				auto regularinlet = std::dynamic_pointer_cast< Inlet >( inlet );
				regularinlet->unregisterFromValueUpdated( this, &UpdatePolicy::inletUpdated );
			}
		}
	}

	bool UpdatePolicy::reset()
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

	void UpdatePolicy::subinletAdded( std::shared_ptr< Inlet > subinlet )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		subinlet->registerToValueUpdated( this, &UpdatePolicy::inletUpdated );

		for ( auto &andTerm : mPolicy )
		{
			auto parentIter = std::find_if( andTerm.begin(), andTerm.end(), PtrCmp( subinlet->getParentInlet() ) );
			if ( parentIter == andTerm.end() )
				continue;

			InletPolicy policy;
			policy.mInlet = subinlet;
			policy.mWasUpdated = false;
			policy.mSubInletPolicy = SubInletPolicy::DISABLED;
			parentIter->mSubInlets.push_back( policy );
		}
	}

	void UpdatePolicy::subinletRemoved( std::shared_ptr< Inlet > subinlet )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		subinlet->unregisterFromValueUpdated( this, &UpdatePolicy::inletUpdated );

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

	void UpdatePolicy::inletUpdated( std::shared_ptr< Inlet > inlet )
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
					if ( inletPolicy.mSubInletPolicy == SubInletPolicy::ANY )
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
					else if ( inletPolicy.mSubInletPolicy == SubInletPolicy::ALL )
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
				UpdateTrigger::fire();
			}
			else if ( !mIsEnabled )
			{
				// store that an update is ready
				mIsAlreadyFulfilled = true;
			}
		}
	}

	void UpdatePolicy::set( const DefaultPolicy code )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		mIsAlreadyFulfilled = false;

		mPolicy.clear();
		std::vector< InletPolicy > tmp;
		switch( code )
		{
		case DefaultPolicy::ALL:
			for ( auto inlet : mInlets )
			{
				InletPolicy policy;
				policy.mInlet = inlet;
				policy.mWasUpdated = false;

				if ( inlet->isMultiInlet() )
				{
					std::shared_ptr< MultiInlet > multiinlet = std::dynamic_pointer_cast< MultiInlet >( inlet );
					for ( uint32_t idx = 0; idx < multiinlet->getSize(); ++idx )
					{
						InletPolicy subinletPolicy;
						subinletPolicy.mInlet = multiinlet->operator[]( idx );
						subinletPolicy.mWasUpdated = false;
						subinletPolicy.mSubInletPolicy = SubInletPolicy::DISABLED;
						policy.mSubInlets.push_back( subinletPolicy );
					}
					
					policy.mSubInletPolicy = SubInletPolicy::ALL;
				}
				else
					policy.mSubInletPolicy = SubInletPolicy::DISABLED;

				tmp.push_back( policy );
			}
			mPolicy.push_back( tmp );
			break;
		case DefaultPolicy::ANY:
			for ( auto inlet : mInlets )
			{
				tmp.clear();

				InletPolicy policy;
				policy.mInlet = inlet;
				policy.mWasUpdated = false;

				if ( inlet->isMultiInlet() )
				{
					std::shared_ptr< MultiInlet > multiinlet = std::dynamic_pointer_cast< MultiInlet >( inlet );
					for ( uint32_t idx = 0; idx < multiinlet->getSize(); ++idx )
					{
						InletPolicy subinletPolicy;
						subinletPolicy.mInlet = multiinlet->operator[]( idx );
						subinletPolicy.mWasUpdated = false;
						subinletPolicy.mSubInletPolicy = SubInletPolicy::DISABLED;
						policy.mSubInlets.push_back( subinletPolicy );
					}
					
					policy.mSubInletPolicy = SubInletPolicy::ALL;
				}
				else
					policy.mSubInletPolicy = SubInletPolicy::DISABLED;

				tmp.push_back( policy );
				mPolicy.push_back( tmp );
			}
			break;
		case DefaultPolicy::DISABLED:
			break;
		}
	}

}
