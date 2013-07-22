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

#include "engine/_2RealInletPolicy.h"
#include "datatypes/_2RealCustomData.h"
#include "engine/_2RealAbstractIOManager.h"

#include "engine/_2RealTypeMetadata.h"

#include <string>

namespace _2Real
{
	// TODO: this object is now after the rewrite more or less identical to inlet metadata....

	class TypeMetadata;
	struct InletInfo;
	struct OutletInfo;

	namespace app
	{
		class TypeMetainfo
		{
		public:

			struct FieldMetainfo
			{
				std::string			name;
				std::string			type;
			};

			TypeMetainfo( _2Real::TypeMetadata const& impl ) : mImpl( &impl ) {}

			void getFieldInfo( Fields &f ) const
			{
				mImpl->getFields( f );
			}

			std::pair< std::string, std::string > getTypename() const
			{
				return mImpl->getTypeId();
			}


		private:
			friend class CustomType;
			_2Real::TypeMetadata					const* mImpl;
		};

		//class InletInfo
		//{
		//public:
		//	InletInfo( _2Real::InletInfo const& impl ) : mImpl( &impl ) {}
		//	//std::string getName() const { return mImpl->baseName; }
		//	//bool isMultiinlet() const { return mImpl->isMuliinlet; }
		//private:
		//	_2Real::InletInfo						const* mImpl;
		//};

		//class OutletInfo
		//{
		//public:
		//	OutletInfo( _2Real::OutletInfo const& impl ) : mImpl( &impl ) {}
		//private:
		//	_2Real::OutletInfo						const* mImpl;
		//};

		//class ParameterInfo
		//{
		//};

		struct InletInfo
		{
			InletInfo() : name( "undefined" ), customName( "undefined" ),
				isMultiInlet( false ), defaultPolicy( InletPolicy::INVALID ), initValue() {}

			std::string								name;
			std::string								customName;
			InletPolicy								defaultPolicy;
			bool									isMultiInlet;
			std::shared_ptr< const CustomType >		initValue;
		};

		struct OutletInfo
		{
			OutletInfo() : name( "undefined" ), customName( "undefined" ) {}

			std::string								name;
			std::string								customName;
		};
	}
}