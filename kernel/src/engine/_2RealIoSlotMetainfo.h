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
#include "engine/_2RealData.h"
#include "engine/_2RealCustomData.h"

namespace _2Real
{

	class TMetainfo;
	class TypeCollection;
	class MetainfoId;

	class IoSlotMetainfo
	{

	public:

		static std::shared_ptr< IoSlotMetainfo > make( std::shared_ptr< const MetainfoId >, std::shared_ptr< const TypeCollection >, const std::string );

		IoSlotMetainfo() = delete;
		IoSlotMetainfo( IoSlotMetainfo const& other ) = delete;
		IoSlotMetainfo( IoSlotMetainfo && other ) = delete;
		IoSlotMetainfo& operator=( IoSlotMetainfo const& other ) = delete;
		IoSlotMetainfo& operator=( IoSlotMetainfo && other ) = delete;

		~IoSlotMetainfo() = default;

		std::string getName() const;
		void setDescription( std::string const& );
		void setDatatypeAndInitialValue( DataItem const& value );
		void setMulti( const bool );

		std::shared_ptr< const MetainfoId >	getId() const;
		std::string getDescription() const;
		std::string getDatatype() const;
		DataItem const& getInitialValue() const;
		std::shared_ptr< const TMetainfo >	getTypeMetainfo() const;
		bool isMulti() const;

	private:

		IoSlotMetainfo( std::shared_ptr< const MetainfoId >, std::shared_ptr< const TypeCollection > );

		std::shared_ptr< const MetainfoId >				mId;
		std::string										mDescription;
		DataItem										mInitialValue;
		bool											mIsMulti;
		std::weak_ptr< const TypeCollection >			mTypes;

	};

}