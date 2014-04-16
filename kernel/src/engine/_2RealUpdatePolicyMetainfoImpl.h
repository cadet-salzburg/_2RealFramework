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

	class UpdatePolicyMetainfoImpl
	{

	public:

		static std::shared_ptr< UpdatePolicyMetainfoImpl > make( std::shared_ptr< const MetainfoId >, const std::vector< InletDeclaration > );

		UpdatePolicyMetainfoImpl() = delete;
		UpdatePolicyMetainfoImpl( UpdatePolicyMetainfoImpl const& other ) = delete;
		UpdatePolicyMetainfoImpl( UpdatePolicyMetainfoImpl && other ) = delete;
		UpdatePolicyMetainfoImpl& operator=( UpdatePolicyMetainfoImpl const& other ) = delete;
		UpdatePolicyMetainfoImpl& operator=( UpdatePolicyMetainfoImpl && other ) = delete;

		~UpdatePolicyMetainfoImpl() = default;

		class InletPolicyMetainfo
		{
		public:
			std::string			mName;
			bool				mIsMulti;
			SubinletPolicy		mPolicy;
		};

		std::shared_ptr< const MetainfoId >	getId() const;

		void set( const DefaultUpdatePolicy );

		std::vector< std::vector< InletPolicyMetainfo > > getInternalRep() const;

	private:

		UpdatePolicyMetainfoImpl( std::shared_ptr< const MetainfoId > );

		std::shared_ptr< const MetainfoId >					mId;
		std::map< std::string, InletDeclaration >			mInlets;
		std::vector< std::vector< InletPolicyMetainfo > >	mPolicy;

	};
}