 /*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2012 Fachhochschule Salzburg GmbH

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

#include "BsonIo.h"
#include "common/_2RealData.h"

namespace _2Real
{
	namespace app
	{
		class Engine;
		class TypeMetainfo_I;
	}

	namespace io
	{
		namespace bson
		{
			class Deserializer
			{

			public:

				static std::shared_ptr< Deserializer > Deserializer::create( std::shared_ptr< const _2Real::DataItem > dataItem, const uint32_t headerBytes = 0 );

				Deserializer( Deserializer const& ) = delete;
				Deserializer( Deserializer && ) = delete;

				~Deserializer() = default;

				std::string getTypename() const;
				std::shared_ptr< _2Real::DataItem > getDataItem( std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) const;
				_2Real::DataItem extractDataItem( mongo::BSONObj &obj, std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) const;

			private:

				Deserializer( std::shared_ptr< mongo::BSONObj > obj );

				std::shared_ptr< mongo::BSONObj > mObj;

			};
		}
	}
}