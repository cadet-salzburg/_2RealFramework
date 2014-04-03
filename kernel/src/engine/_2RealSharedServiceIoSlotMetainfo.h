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

	class SharedServiceIoSlotMetainfo
	{

	public:

		static std::shared_ptr< SharedServiceIoSlotMetainfo > make( std::shared_ptr< const MetainfoId >, std::shared_ptr< const TypeCollection >, std::string const& );

		void setDescription( std::string const& );
		void setDatatypeAndInitialValue( DataItem const& value );
		void setMulti( const bool );

		std::string getName() const;
		std::string getDescription() const;
		std::string getDatatype() const;
		DataItem const& getInitialValue() const;
		std::shared_ptr< const TMetainfo >	getTypeMetainfo() const;
		bool isMulti() const;

	protected:

		std::shared_ptr< const MetainfoId >				mId;
		std::string										mDescription;
		DataItem										mInitialValue;
		bool											mIsMulti;
		std::weak_ptr< const TypeCollection >			mTypes;

	private:

		SharedServiceIoSlotMetainfo( std::shared_ptr< const MetainfoId >, std::shared_ptr< const TypeCollection > );
		SharedServiceIoSlotMetainfo( SharedServiceIoSlotMetainfo const& ) = delete;
		SharedServiceIoSlotMetainfo& operator=( SharedServiceIoSlotMetainfo const& ) = delete;

	};

}