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
#include "common/_2RealDeclarations.h"

namespace _2Real
{
	class IoSlotMetainfoImpl;

	class UpdatePolicyMetainfoImpl;
	class BlockFactory_I;
	class TypeCollection;
	class MetainfoId;

	class BlockMetainfoImpl
	{

	public:

		static std::shared_ptr< BlockMetainfoImpl > make( std::shared_ptr< const MetainfoId >, std::shared_ptr< const TypeCollection >, BlockDeclaration const& );

		BlockMetainfoImpl() = delete;
		BlockMetainfoImpl( BlockMetainfoImpl const& other ) = delete;
		BlockMetainfoImpl( BlockMetainfoImpl && other ) = delete;
		BlockMetainfoImpl& operator=( BlockMetainfoImpl const& other ) = delete;
		BlockMetainfoImpl& operator=( BlockMetainfoImpl && other ) = delete;

		~BlockMetainfoImpl() = default;

		void setDescription( std::string const& );
		void setDependencies( std::vector< std::string > const& );

		std::string 				getName() const;
		std::string					getDescription() const;
		std::vector< std::string >	getDependencies() const;

		std::vector< std::shared_ptr< const IoSlotMetainfoImpl > > getInletMetainfos() const;
		std::vector< std::shared_ptr< const IoSlotMetainfoImpl > > getOutletMetainfos() const;
		std::vector< std::shared_ptr< const IoSlotMetainfoImpl > > getParameterMetainfos() const;

		std::shared_ptr< IoSlotMetainfoImpl > getInletMetainfo( std::string const& );
		std::shared_ptr< IoSlotMetainfoImpl > getOutletMetainfo( std::string const& );
		std::shared_ptr< IoSlotMetainfoImpl > getParameterMetainfo( std::string const& );
		std::shared_ptr< const IoSlotMetainfoImpl > getInletMetainfo( std::string const& ) const;
		std::shared_ptr< const IoSlotMetainfoImpl > getOutletMetainfo( std::string const& ) const;
		std::shared_ptr< const IoSlotMetainfoImpl > getParameterMetainfo( std::string const& ) const;

		std::shared_ptr< UpdatePolicyMetainfoImpl > getDefaultUpdatePolicy();
		std::shared_ptr< const UpdatePolicyMetainfoImpl > getDefaultUpdatePolicy() const;

		void setFactory( std::shared_ptr< BlockFactory_I > factory );
		std::shared_ptr< const BlockFactory_I > getFactory() const;

		std::shared_ptr< const MetainfoId > getId() const;

	private:

		BlockMetainfoImpl( std::shared_ptr< const MetainfoId >, std::shared_ptr< const TypeCollection > );

		typedef std::vector< std::shared_ptr< IoSlotMetainfoImpl > >		IoSlots;

		std::shared_ptr< const MetainfoId >					mId;
		std::weak_ptr< const TypeCollection >				mTypes;

		std::string											mDescription;
		std::vector< std::string >							mDependencies;

		std::shared_ptr< UpdatePolicyMetainfoImpl >				mDefaultUpdatePolicy;
		std::shared_ptr< BlockFactory_I >		mFactory;

		IoSlots												mInlets;
		IoSlots												mParameters;
		IoSlots												mOutlets;
		

	};

}