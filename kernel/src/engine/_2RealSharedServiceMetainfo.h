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
#include "engine/_2RealDeclarations.h"

namespace _2Real
{
	class IoSlotMetainfo;

	class UpdatePolicyMetainfo;
	class AbstractSharedServiceFactory;
	class TypeCollection;
	class MetainfoId;

	class SharedServiceMetainfo
	{

	public:

		static std::shared_ptr< SharedServiceMetainfo > make( std::shared_ptr< const MetainfoId >, std::shared_ptr< const TypeCollection >, BlockDeclaration const& );

		SharedServiceMetainfo() = delete;
		SharedServiceMetainfo( SharedServiceMetainfo const& other ) = delete;
		SharedServiceMetainfo( SharedServiceMetainfo && other ) = delete;
		SharedServiceMetainfo& operator=( SharedServiceMetainfo const& other ) = delete;
		SharedServiceMetainfo& operator=( SharedServiceMetainfo && other ) = delete;

		~SharedServiceMetainfo() = default;

		void setDescription( std::string const& );
		void setDependencies( std::vector< std::string > const& );

		bool						isSingleton() const;
		std::string 				getName() const;
		std::string					getDescription() const;
		std::vector< std::string >	getDependencies() const;

		std::vector< std::shared_ptr< const IoSlotMetainfo > > getInletMetainfos() const;
		std::vector< std::shared_ptr< const IoSlotMetainfo > > getOutletMetainfos() const;
		std::vector< std::shared_ptr< const IoSlotMetainfo > > getParameterMetainfos() const;

		std::shared_ptr< IoSlotMetainfo > getInletMetainfo( std::string const& );
		std::shared_ptr< IoSlotMetainfo > getOutletMetainfo( std::string const& );
		std::shared_ptr< IoSlotMetainfo > getParameterMetainfo( std::string const& );
		std::shared_ptr< const IoSlotMetainfo > getInletMetainfo( std::string const& ) const;
		std::shared_ptr< const IoSlotMetainfo > getOutletMetainfo( std::string const& ) const;
		std::shared_ptr< const IoSlotMetainfo > getParameterMetainfo( std::string const& ) const;

		std::shared_ptr< UpdatePolicyMetainfo > getDefaultUpdatePolicy();
		std::shared_ptr< const UpdatePolicyMetainfo > getDefaultUpdatePolicy() const;

		void setFactory( std::shared_ptr< AbstractSharedServiceFactory > factory );
		std::shared_ptr< const AbstractSharedServiceFactory > getFactory() const;

		std::shared_ptr< const MetainfoId > getId() const;

	private:

		SharedServiceMetainfo( std::shared_ptr< const MetainfoId >, std::shared_ptr< const TypeCollection >, const bool );

		typedef std::vector< std::shared_ptr< IoSlotMetainfo > >		IoSlots;

		std::shared_ptr< const MetainfoId >					mId;
		std::weak_ptr< const TypeCollection >				mTypes;

		bool												mIsSingleton;
		std::string											mDescription;
		std::vector< std::string >							mDependencies;

		std::shared_ptr< UpdatePolicyMetainfo >				mDefaultUpdatePolicy;
		std::shared_ptr< AbstractSharedServiceFactory >		mFactory;

		IoSlots												mInlets;
		IoSlots												mParameters;
		IoSlots												mOutlets;
		

	};

}