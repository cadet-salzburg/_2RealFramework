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
#include "engine/_2RealCloneableMetainfo.h"
#include "engine/_2RealData.h"
#include "engine/_2RealCustomData.h"

namespace _2Real
{

	class MetainfoId;
	class TypeCollection;

	/*
	*	base class for basic type metainfo & user defined type metainfo
	*/
	class TMetainfo
	{

	public:

		TMetainfo() = default;
		virtual ~TMetainfo() = default;

		TMetainfo( TMetainfo const& ) = delete;
		TMetainfo& operator=( TMetainfo const& ) = delete;
		TMetainfo( TMetainfo && ) = delete;
		TMetainfo& operator=( TMetainfo && ) = delete;

		virtual DataItem makeData() const = 0;
		virtual bool isBasicType() const = 0;
		virtual std::shared_ptr< const MetainfoId > getId() const = 0;
		virtual std::string getName() const = 0;

	};

	/*
	*	basic type metainfo: represents every type within the set of data types except CustomDataItem
	*/
	class BasicTypeMetainfo : public TMetainfo
	{

	public:

		static std::shared_ptr< BasicTypeMetainfo > make( std::shared_ptr< TypeCollection > types, const DataItem );

		DataItem makeData() const;
		bool isBasicType() const;

		std::string getName() const;
		std::shared_ptr< const MetainfoId > getId() const;

	private:

		explicit BasicTypeMetainfo( std::shared_ptr< const MetainfoId >, std::shared_ptr< TypeCollection >, DataItem );

		std::shared_ptr< const MetainfoId >			mId;
		std::weak_ptr< const TypeCollection >		mTypes;
		DataItem									mTemplate;

	};

	class SharedTypeMetainfo : public TMetainfo
	{

	public:

		static std::shared_ptr< SharedTypeMetainfo > make( std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types, std::string const& );

		bool isBasicType() const;
		std::string getDescription() const;

		DataItem makeData() const;
		CustomDataItem makeCustomData() const;

		void setDescription( const std::string );
		void addField( std::string fieldName, DataItem value );

		std::vector< std::pair< std::string, std::shared_ptr< const TMetainfo > > > getDataFields() const;

		std::string getName() const;
		std::shared_ptr< const MetainfoId > getId() const;

		void unregisterFromTypeCollection();

	private:

		SharedTypeMetainfo() = delete;
		SharedTypeMetainfo( std::shared_ptr< const MetainfoId >, std::shared_ptr< TypeCollection > );

		std::shared_ptr< const MetainfoId >			mId;
		std::string									mDescription;
		CustomDataItem								mTemplate;
		std::weak_ptr< TypeCollection >				mTypes;

	};

}