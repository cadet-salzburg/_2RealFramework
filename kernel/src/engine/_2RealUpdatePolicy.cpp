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
#include "engine/_2RealAbstractInlet.h"
#include "engine/_2RealInlet.h"
#include "engine/_2RealMultiInlet.h"

namespace _2Real
{
	std::shared_ptr< UpdatePolicyMetainfo > UpdatePolicyMetainfo::make( std::shared_ptr< const MetainfoId > id, const std::vector< std::pair< std::string, bool > > inlets )
	{
		std::shared_ptr< const MetainfoId > policyId = MetainfoId::create( id, MetainfoType::POLICY, "policy" );
		std::shared_ptr< UpdatePolicyMetainfo > policyInfo( new UpdatePolicyMetainfo( policyId ) );

		for ( auto const& it : inlets )
			policyInfo->mInlets[ it.first ] = it.second;

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
				info.mName = it.first;
				info.mRequireAll = it.second ? true : false;
				tmp.push_back( info );
			}
			mPolicy.push_back( tmp );
			break;
		case DefaultPolicy::ANY:
			for ( auto it : mInlets )
			{
				tmp.clear();
				InletPolicyMetainfo info;
				info.mName = it.first;
				info.mRequireAll = false;
				tmp.push_back( info );
				mPolicy.push_back( tmp );
			}
			break;
		case DefaultPolicy::DISABLED:
			break;
		}
	}

	void UpdatePolicyMetainfo::set( std::vector< std::vector< InletPolicy > > settings )
	{
		mPolicy.clear();

		for ( auto term : settings )
		{
			std::vector< InletPolicyMetainfo > tmp;

			for ( auto inlet : term )
			{
				bool isMulti = mInlets.at( inlet.mName );

				InletPolicyMetainfo inletPolicy;
				inletPolicy.mName = inlet.mName;
				inletPolicy.mRequireAll = inlet.mRequireAll;
				tmp.push_back( inletPolicy );
			}

			mPolicy.push_back( tmp );
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

	std::shared_ptr< UpdatePolicy > UpdatePolicy::makeFromMetainfo( std::shared_ptr< const UpdatePolicyMetainfo > meta, std::map< std::shared_ptr< const InstanceId >, std::shared_ptr< AbstractInlet > > inlets )
	{
		std::shared_ptr< const InstanceId > id = InstanceId::create( meta->getId(), nullptr, InstanceType::POLICY, "update policy" );
		std::shared_ptr< UpdatePolicy > policy( new UpdatePolicy( id ) );
		
		// first, register to all inlets
		// not sure if giving the update policy access to all inlets is a good idea
		policy->mInlets = inlets;
		for ( auto it : inlets )
		{
			if ( !it.second->isMultiInlet() )
			{
				std::shared_ptr< Inlet > inlet = std::dynamic_pointer_cast< Inlet >( it.second );
				inlet->registerToValueUpdated( policy.get(), &UpdatePolicy::inletUpdated );
			}
		}

		std::vector< std::vector< UpdatePolicyMetainfo::InletPolicyMetainfo > > rep = meta->getInternalRep();

		for ( auto const& term : rep )
		{
			std::vector< UpdatePolicy::InletPolicy > andTerm;
			for ( auto const& inlet : term  )
			{
				std::shared_ptr< const InstanceId > id;
				for ( auto it : inlets )
				{
					if ( it.first->getName() == inlet.mName )
					{
						id = it.first;
						break;
					}
				}

				UpdatePolicy::InletPolicy inletPolicy;
				inletPolicy.mId = id;
				inletPolicy.mWasUpdated = false;
				inletPolicy.mRequireAll = inlet.mRequireAll;
				andTerm.push_back( inletPolicy );
			}
			policy->mPolicy.push_back( andTerm );
		}

		return policy;
	}

	UpdatePolicy::UpdatePolicy( std::shared_ptr< const InstanceId > id ) :
		mId( id ),
		mPolicy()	// starts out as disabled
	{
	}

	void UpdatePolicy::subinletAdded( std::shared_ptr< const InstanceId > inletId, std::shared_ptr< const InstanceId > subinletId )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		//for ( auto &term : mPolicy )
		//{
		//	for ( auto &inlet : term )
		//	{
		//		//if ( inlet.mId == inletId )
		//			// OR or ANY ????
		//			//inlet.mSubInlets.push_back( subinletId );
		//	}
		//}
	}

	void UpdatePolicy::inletUpdated( std::shared_ptr< const InstanceId > id )
	{
		std::lock_guard< std::mutex > lock( mMutex );

		bool shouldFire = true;

		for ( auto &term : mPolicy )
		{
			for ( auto &inlet : term )
			{
				if ( !inlet.mSubInlets.empty() )
				{
					for ( auto &subInlet : inlet.mSubInlets )
					{
						if ( subInlet.mId == id )
							subInlet.mWasUpdated = true;

						shouldFire &= subInlet.mWasUpdated;
					}
				}
				else
				{
					if ( inlet.mId == id )
						inlet.mWasUpdated = true;

					shouldFire &= inlet.mWasUpdated;
				}
			}
		}

		if ( shouldFire )
			UpdateTrigger::fire();
	}

}
