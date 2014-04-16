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

#include "common/_2RealInletPolicy.h"
#include "engine/_2RealId.h"
#include "engine/_2RealBlockImpl.h"
#include "engine/_2RealInletImpl_I.h"
#include "engine/_2RealInletImpl.h"
#include "engine/_2RealMultiInletImpl.h"
#include "engine/_2RealUpdatePolicyMetainfoImpl.h"

namespace _2Real
{
	std::shared_ptr< UpdatePolicyMetainfoImpl > UpdatePolicyMetainfoImpl::make( std::shared_ptr< const MetainfoId > id, const std::vector< InletDeclaration > inlets )
	{
		std::shared_ptr< const MetainfoId > policyId = MetainfoId::create( id, MetainfoType::POLICY, "policy" );
		std::shared_ptr< UpdatePolicyMetainfoImpl > policyInfo( new UpdatePolicyMetainfoImpl( policyId ) );

		for ( auto const& it : inlets )
			policyInfo->mInlets.insert( std::make_pair( it.mName, it ) );

		return policyInfo;
	}

	UpdatePolicyMetainfoImpl::UpdatePolicyMetainfoImpl( std::shared_ptr< const MetainfoId > id ) :
		mId( id ),
		mInlets(),
		mPolicy()
	{
	}

	void UpdatePolicyMetainfoImpl::set( const DefaultUpdatePolicy policy )
	{
		mPolicy.clear();
		std::vector< InletPolicyMetainfo > tmp;
		switch( policy )
		{
		case DefaultUpdatePolicy::ALL:
			for ( auto it : mInlets )
			{
				InletPolicyMetainfo info;
				info.mName = it.second.mIsMulti;
				info.mIsMulti = it.second.mIsMulti;
				info.mPolicy = it.second.mIsMulti ? SubinletPolicy::ALL : SubinletPolicy::DISABLED;
				tmp.push_back( info );
			}
			mPolicy.push_back( tmp );
			break;
		case DefaultUpdatePolicy::ANY:
			for ( auto it : mInlets )
			{
				tmp.clear();
				InletPolicyMetainfo info;
				info.mName = it.second.mIsMulti;
				info.mIsMulti = it.second.mIsMulti;
				info.mPolicy = it.second.mIsMulti ? SubinletPolicy::ANY : SubinletPolicy::DISABLED;
				tmp.push_back( info );
				mPolicy.push_back( tmp );
			}
			break;
		case DefaultUpdatePolicy::DISABLED:
			break;
		}
	}

	std::vector< std::vector< UpdatePolicyMetainfoImpl::InletPolicyMetainfo > > UpdatePolicyMetainfoImpl::getInternalRep() const
	{
		return mPolicy;
	}

	std::shared_ptr< const MetainfoId >	UpdatePolicyMetainfoImpl::getId() const
	{
		return mId;
	}
}
