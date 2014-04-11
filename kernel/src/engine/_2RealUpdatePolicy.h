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
#include "engine/_2RealUpdateTrigger.h"
#include "enums/_2RealDefaultPolicy.h"
#include "engine/_2RealDeclarations.h"

namespace _2Real
{

	class MetainfoId;
	class InstanceId;
	class InletPolicy;
	class AbstractInlet;
	class Inlet;
	class MultiInlet;
	class Block;

	enum class SubInletPolicy { ANY, ALL, DISABLED };

	class UpdatePolicyMetainfo
	{

	public:

		static std::shared_ptr< UpdatePolicyMetainfo > make( std::shared_ptr< const MetainfoId >, const std::vector< InletDeclaration > );

		UpdatePolicyMetainfo() = delete;
		UpdatePolicyMetainfo( UpdatePolicyMetainfo const& other ) = delete;
		UpdatePolicyMetainfo( UpdatePolicyMetainfo && other ) = delete;
		UpdatePolicyMetainfo& operator=( UpdatePolicyMetainfo const& other ) = delete;
		UpdatePolicyMetainfo& operator=( UpdatePolicyMetainfo && other ) = delete;

		~UpdatePolicyMetainfo() = default;

		class InletPolicyMetainfo
		{
		public:
			std::string			mName;
			bool				mIsMulti;
			SubInletPolicy		mPolicy;
		};

		std::shared_ptr< const MetainfoId >	getId() const;

		void set( const DefaultPolicy );

		std::vector< std::vector< InletPolicyMetainfo > > getInternalRep() const;

	private:

		UpdatePolicyMetainfo( std::shared_ptr< const MetainfoId > );

		std::shared_ptr< const MetainfoId >					mId;
		std::map< std::string, InletDeclaration >			mInlets;
		std::vector< std::vector< InletPolicyMetainfo > >	mPolicy;

	};

	class UpdatePolicy : public UpdateTrigger, public std::enable_shared_from_this< UpdatePolicy >
	{

	public:

		static std::shared_ptr< UpdatePolicy > createFromMetainfo( std::shared_ptr< Block >, std::shared_ptr< const UpdatePolicyMetainfo >, std::vector< std::shared_ptr< AbstractInlet > > const& );

		UpdatePolicy() = delete;
		UpdatePolicy( UpdatePolicy const& other ) = delete;
		UpdatePolicy( UpdatePolicy && other ) = delete;
		UpdatePolicy& operator=( UpdatePolicy const& other ) = delete;
		UpdatePolicy& operator=( UpdatePolicy && other ) = delete;

		~UpdatePolicy();

		void set( const DefaultPolicy );

		void inletUpdated( std::shared_ptr< Inlet > );
		void subinletAdded( std::shared_ptr< Inlet > );
		void subinletRemoved( std::shared_ptr< Inlet > );

		void disable();
		void enable();
		bool reset();

		std::shared_ptr< const InstanceId > getId() const;
		std::shared_ptr< Block > getParent();

	private:

		UpdatePolicy( std::shared_ptr< Block >, std::shared_ptr< const UpdatePolicyMetainfo >, std::shared_ptr< const InstanceId > );

		class InletPolicy
		{
		public:
			std::shared_ptr< AbstractInlet >		mInlet;
			bool									mWasUpdated;
			std::vector< InletPolicy >				mSubInlets;
			SubInletPolicy							mSubInletPolicy;
		};

		struct PtrCmp : public std::unary_function< std::string, bool >
		{
			explicit PtrCmp( std::shared_ptr< AbstractInlet > obj ) : mObj( obj ) { assert( mObj ); }

			bool operator()( UpdatePolicy::InletPolicy const& other )
			{
				return mObj.get() == other.mInlet.get();
			}

			std::shared_ptr< AbstractInlet > mObj;
		};

		std::weak_ptr< Block >																	mParent;
		std::weak_ptr< const UpdatePolicyMetainfo >												mMetainfo;
		std::shared_ptr< const InstanceId >														mId;
		std::vector< std::shared_ptr< AbstractInlet > >											mInlets;
		std::vector< std::vector< InletPolicy > >												mPolicy;
		mutable	std::mutex																		mMutex;

		bool																					mIsAlreadyFulfilled;
		bool																					mIsEnabled;

	};

}