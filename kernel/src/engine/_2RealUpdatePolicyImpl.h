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

#include "common/_2RealStdIncludes.h"
#include "engine/_2RealUpdateTrigger_I.h"
#include "common/_2RealDefaultUpdatePolicy.h"
#include "common/_2RealSubinletPolicy.h"
#include "common/_2RealDeclarations.h"

namespace _2Real
{

	class MetainfoId;
	class InstanceId;
	class InletPolicy;
	class InletImpl_I;
	class InletImpl;
	class MultiInletImpl;
	class BlockImpl;
	class UpdatePolicyMetainfoImpl;

	class UpdatePolicyImpl : public UpdateTrigger_I, public std::enable_shared_from_this< UpdatePolicyImpl >
	{

	public:

		class InletPolicy
		{
		public:
			std::shared_ptr< const InletImpl_I >		mInlet;
			bool										mWasUpdated;
			SubinletPolicy								mSubInletPolicy;
			std::vector< InletPolicy >					mSubInlets;
		};

		struct ConnectionInfo
		{
			std::shared_ptr< const InletImpl_I >		mInlet;
			boost::signals2::connection					mAddedConnection;
			boost::signals2::connection					mRemovedConnection;
			boost::signals2::connection					mUpdatedConnection;
			std::vector< ConnectionInfo >				mSubinlets;
		};

		static std::shared_ptr< UpdatePolicyImpl > createFromMetainfo( std::shared_ptr< BlockImpl >, std::shared_ptr< const UpdatePolicyMetainfoImpl >, std::vector< std::shared_ptr< InletImpl_I > > const& );

		UpdatePolicyImpl() = delete;
		UpdatePolicyImpl( UpdatePolicyImpl const& other ) = delete;
		UpdatePolicyImpl( UpdatePolicyImpl && other ) = delete;
		UpdatePolicyImpl& operator=( UpdatePolicyImpl const& other ) = delete;
		UpdatePolicyImpl& operator=( UpdatePolicyImpl && other ) = delete;

		~UpdatePolicyImpl();

		void set( const DefaultUpdatePolicy );

		void inletUpdated( std::shared_ptr< const InletImpl > );
		void subinletAdded( std::shared_ptr< const InletImpl > );
		void subinletRemoved( std::shared_ptr< const InletImpl > );

		std::shared_ptr< const InstanceId > getId() const;
		std::shared_ptr< BlockImpl > getParent();

		boost::signals2::connection registerToUpdate( boost::signals2::signal< void() >::slot_type ) const;

	private:

		UpdatePolicyImpl( std::shared_ptr< BlockImpl >, std::shared_ptr< const UpdatePolicyMetainfoImpl >, std::shared_ptr< const InstanceId > );

		std::weak_ptr< BlockImpl >							mParent;
		std::weak_ptr< const UpdatePolicyMetainfoImpl >		mMetainfo;
		std::shared_ptr< const InstanceId >					mId;

		std::vector< ConnectionInfo >						mConnections;
		std::vector< std::vector< InletPolicy > >			mPolicy;

		mutable	std::mutex									mMutex;
		mutable boost::signals2::signal< void() >			mReady;

	};

}