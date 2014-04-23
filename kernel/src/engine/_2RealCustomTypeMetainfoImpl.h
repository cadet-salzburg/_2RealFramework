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
#include "engine/_2RealTypeMetainfoImpl_I.h"
#include "common/_2RealData.h"
#include "common/_2RealSignals.h"

namespace _2Real
{

	class TypeCollection;

	class CustomTypeMetainfoImpl : public TypeMetainfoImpl_I, public std::enable_shared_from_this< CustomTypeMetainfoImpl >
	{

	public:

		static std::shared_ptr< CustomTypeMetainfoImpl > make( std::shared_ptr< const MetainfoId > id, std::shared_ptr< TypeCollection > types, const std::string );

		bool isBasicType() const;
		std::string getDescription() const;

		DataItem makeData() const;
		CustomDataItem makeCustomData() const;

		void setDescription( const std::string );
		void addField( std::string fieldName, DataItem value );

		std::vector< std::pair< std::string, std::shared_ptr< const TypeMetainfoImpl_I > > > getDataFields() const;

		std::string getName() const;
		std::shared_ptr< const MetainfoId > getId() const;

		void destroy();

		Connection registerToDestroyed( boost::signals2::signal< void( std::shared_ptr< const CustomTypeMetainfoImpl > ) >::slot_type ) const;

	private:

		CustomTypeMetainfoImpl( std::shared_ptr< const MetainfoId >, std::shared_ptr< TypeCollection > );

		std::shared_ptr< const MetainfoId >			mId;
		std::string									mDescription;
		CustomDataItem								mTemplate;
		std::weak_ptr< TypeCollection >				mTypes;

		mutable boost::signals2::signal< void( std::shared_ptr< const CustomTypeMetainfoImpl > ) > mDestroyed;

	};

}