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

#pragma warning( disable : 4996 )
#pragma warning( disable : 4702 )

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
	struct InfoCmp : public std::unary_function< bool, UpdatePolicyImpl::ConnectionInfo >
	{
		explicit InfoCmp( std::shared_ptr< const InletImpl_I > obj ) : mBaseline( obj ) { assert( mBaseline ); }

		bool operator()( UpdatePolicyImpl::ConnectionInfo const& info )
		{
			return mBaseline.get() == info.mInlet.get();
		}

		std::shared_ptr< const InletImpl_I > mBaseline;
	};

	struct PolicyCmp : public std::unary_function< bool, UpdatePolicyImpl::InletPolicy >
	{
		explicit PolicyCmp( std::shared_ptr< const InletImpl_I > obj ) : mBaseline( obj ) { assert( mBaseline ); }

		bool operator()( UpdatePolicyImpl::InletPolicy const& policy )
		{
			return mBaseline.get() == policy.mInlet.get();
		}

		std::shared_ptr< const InletImpl_I > mBaseline;
	};

	std::shared_ptr< UpdatePolicyImpl > UpdatePolicyImpl::createFromMetainfo( std::shared_ptr< BlockImpl > parent, std::shared_ptr< const UpdatePolicyMetainfoImpl > meta, std::vector< std::shared_ptr< InletImpl_I > > const& inlets )
	{
		std::shared_ptr< const InstanceId > id = InstanceId::create( meta->getId(), parent->getId(), InstanceType::POLICY, "update policy" );
		std::shared_ptr< UpdatePolicyImpl > policy( new UpdatePolicyImpl( parent, meta, id ) );
		
		std::vector< UpdatePolicyImpl::ConnectionInfo > connections;
		for ( auto inlet : inlets )
		{
			ConnectionInfo info;
			info.mInlet = inlet;

			if ( inlet->isMultiInlet() )
			{
				auto multiinlet = std::static_pointer_cast< MultiInletImpl >( inlet );
				info.mAddedConnection = multiinlet->registerToSubinletAdded( std::bind( &UpdatePolicyImpl::subinletAdded, policy.get(), std::placeholders::_1 ) );
				info.mRemovedConnection = multiinlet->registerToSubinletRemoved( std::bind( &UpdatePolicyImpl::subinletRemoved, policy.get(), std::placeholders::_1 ) );
			}
			else
			{
				auto regularinlet = std::static_pointer_cast< InletImpl >( inlet );
				info.mUpdatedConnection = regularinlet->registerToValueUpdated( std::bind( &UpdatePolicyImpl::inletUpdated, policy.get(), std::placeholders::_1 ) );
			}

			policy->mConnections.push_back( info );
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
		mPolicy()
	{
	}

	UpdatePolicyImpl::~UpdatePolicyImpl()
	{
		for ( auto &it : mConnections )
		{
			it.mAddedConnection.disconnect();
			it.mRemovedConnection.disconnect();
			it.mUpdatedConnection.disconnect();
			for ( auto &subIt : it.mSubinlets )
			{
				subIt.mAddedConnection.disconnect();
				subIt.mRemovedConnection.disconnect();
				subIt.mUpdatedConnection.disconnect();
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

	void UpdatePolicyImpl::subinletAdded( std::shared_ptr< const InletImpl > subinlet )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		auto parent = std::find_if( mConnections.begin(), mConnections.end(), InfoCmp( subinlet->getParentInlet() ) );
		assert ( parent != mConnections.end() );

		ConnectionInfo subinletInfo;
		subinletInfo.mInlet = subinlet;
		subinletInfo.mUpdatedConnection = subinlet->registerToValueUpdated( std::bind( &UpdatePolicyImpl::inletUpdated, this, std::placeholders::_1 ) );
		parent->mSubinlets.push_back( subinletInfo );

		for ( auto &andTerm : mPolicy )
		{
			auto parentIter = std::find_if( andTerm.begin(), andTerm.end(), PolicyCmp( subinlet->getParentInlet() ) );
			if ( parentIter == andTerm.end() )
				continue;

			InletPolicy policy;
			policy.mInlet = subinlet;
			policy.mWasUpdated = false;
			policy.mSubInletPolicy = SubinletPolicy::DISABLED;
			parentIter->mSubInlets.push_back( policy );
		}
	}

	void UpdatePolicyImpl::subinletRemoved( std::shared_ptr< const InletImpl > subinlet )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		auto parent = std::find_if( mConnections.begin(), mConnections.end(), InfoCmp( subinlet->getParentInlet() ) );
		assert ( parent != mConnections.end() );
		auto info = std::find_if( parent->mSubinlets.begin(), parent->mSubinlets.end(), InfoCmp( subinlet ) );
		assert( info != parent->mSubinlets.end() );
		info->mUpdatedConnection.disconnect();
		
		for ( auto &andTerm : mPolicy )
		{
			auto parentIter = std::find_if( andTerm.begin(), andTerm.end(), PolicyCmp( subinlet->getParentInlet() ) );
			if ( parentIter == andTerm.end() )
				continue;

			auto inletIter = std::find_if( parentIter->mSubInlets.begin(), parentIter->mSubInlets.end(), PolicyCmp( subinlet ) );
			if ( inletIter == parentIter->mSubInlets.end() )
				continue;

			parentIter->mSubInlets.erase( inletIter );
		}
	}

	void UpdatePolicyImpl::inletUpdated( std::shared_ptr< const InletImpl > inlet )
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
			// 1st, reset
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

			// 2nd, notify
			mReady();
		}
	}

	void UpdatePolicyImpl::set( const DefaultUpdatePolicy code )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		mPolicy.clear();
		std::vector< InletPolicy > tmp;
		switch( code )
		{
		case DefaultUpdatePolicy::ALL:
			for ( auto const& info : mConnections )
			{
				InletPolicy policy;
				policy.mInlet = info.mInlet;
				policy.mWasUpdated = false;

				if ( info.mInlet->isMultiInlet() )
				{
					auto multiinlet = std::static_pointer_cast< const MultiInletImpl >( info.mInlet );
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
			for ( auto const& info : mConnections )
			{
				tmp.clear();

				InletPolicy policy;
				policy.mInlet = info.mInlet;
				policy.mWasUpdated = false;

				if ( info.mInlet->isMultiInlet() )
				{
					auto multiinlet = std::static_pointer_cast< const MultiInletImpl >( info.mInlet );
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

	boost::signals2::connection UpdatePolicyImpl::registerToUpdate( boost::signals2::signal< void() >::slot_type listener ) const
	{
		return mReady.connect( listener );
	}

}
