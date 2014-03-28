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

namespace _2Real
{

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

		//virtual std::shared_ptr< TypeDescriptor > getTypeDescriptor() = 0;

	};

	class BasicTypeMetainfo : public TMetainfo, public std::enable_shared_from_this< BasicTypeMetainfo >
	{

	public:

		BasicTypeMetainfo() = delete;
		explicit BasicTypeMetainfo( DataItem );

		DataItem makeData() const;
		bool isBasicType() const;

	private:

		DataItem									mTemplate;
		std::shared_ptr< BasicTypeDescriptor >		mTypeDescriptor;

	};

	class SharedTypeMetainfo : public TMetainfo, /*public CloneableMetainfo< SharedTypeMetainfo >,*/ public std::enable_shared_from_this< SharedTypeMetainfo >
	{

	public:

		SharedTypeMetainfo() = delete;
		explicit SharedTypeMetainfo( const std::string );

		DataItem makeData() const;
		bool isBasicType() const;

		CustomDataItem makeCustomData() const;

		std::string getName() const;
		std::string getDescription() const;

		void setDescription( const std::string );

		// TODO: missing default value as default argument for value
		void addField( std::string fieldName, DataItem value );

		void cloneFrom( SharedTypeMetainfo const& other );

	private:

		std::string									mName;
		std::string									mDescription;
		CustomDataItem								mTemplate;
		std::shared_ptr< CustomTypeDescriptor >		mTypeDescriptor;

	};

}