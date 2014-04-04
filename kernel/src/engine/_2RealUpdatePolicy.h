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

namespace _2Real
{

	class MetainfoId;
	class InstanceId;
	class InletPolicy;
	class AbstractInlet;

	class UpdatePolicyMetainfo
	{

	public:

		static std::shared_ptr< UpdatePolicyMetainfo > make( std::shared_ptr< const MetainfoId >, const std::vector< std::pair< std::string, bool > > );

		struct InletPolicyMetainfo
		{
			std::string	mName;
			bool		mRequireAll;
		};

		std::shared_ptr< const MetainfoId >	getId() const;
		std::vector< std::vector< InletPolicyMetainfo > > getInternalRep() const;

		void set( const DefaultPolicy );
		void set( std::vector< std::vector< InletPolicy > > );

	private:

		UpdatePolicyMetainfo( std::shared_ptr< const MetainfoId > );

		std::shared_ptr< const MetainfoId >					mId;
		std::map< std::string, bool >						mInlets;
		std::vector< std::vector< InletPolicyMetainfo > >	mPolicy;

	};

	class UpdatePolicy : public UpdateTrigger
	{

	public:

		static std::shared_ptr< UpdatePolicy > makeFromMetainfo( std::shared_ptr< const UpdatePolicyMetainfo >, std::map< std::shared_ptr< const InstanceId >, std::shared_ptr< AbstractInlet > > );

		void set( const DefaultPolicy );
		//void set( ALL( ), inlets

		void inletUpdated( std::shared_ptr< const InstanceId > );
		void subinletAdded( std::shared_ptr< const InstanceId >, std::shared_ptr< const InstanceId > );

	private:

		UpdatePolicy( std::shared_ptr< const InstanceId > );

		struct InletPolicy
		{
			std::shared_ptr< const InstanceId >		mId;
			std::vector< InletPolicy >				mSubInlets;	// empty if this is not a multiinlet
			bool									mWasUpdated;
			bool									mRequireAll;
		};

		std::shared_ptr< const InstanceId >														mId;
		std::map< std::shared_ptr< const InstanceId >, std::shared_ptr< AbstractInlet > >		mInlets;
		std::vector< std::vector< InletPolicy > >												mPolicy;
		mutable	std::mutex																		mMutex;

	};

}