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

namespace mongo
{
	class BSONObjBuilder;
}

namespace _2Real
{
	namespace io
	{
		namespace bson
		{
			class Serializer : public boost::static_visitor< >
			{

			public:

				Serializer();

				void operator()( std::vector< uint8_t > const& value );
				void operator()( std::vector< int8_t > const& value );
				void operator()( std::vector< uint16_t > const& value );
				void operator()( std::vector< int16_t > const& value );
				void operator()( std::vector< uint32_t > const& value );
				void operator()( std::vector< int32_t > const& value );
				void operator()( std::vector< uint64_t > const& value );
				void operator()( std::vector< int64_t > const& value );
				void operator()( std::vector< double > const& value );
				void operator()( std::vector< float > const& value );
				void operator()( std::vector< bool > const& value );
				void operator()( std::vector< std::string > const& value );

				void operator()( const uint8_t value );
				void operator()( const int8_t value );
				void operator()( const uint16_t value );
				void operator()( const int16_t value );
				void operator()( const uint32_t value );
				void operator()( const int32_t value );
				void operator()( const uint64_t value );
				void operator()( const int64_t value );
				void operator()( const double value );
				void operator()( const float value );
				void operator()( const bool value );
				void operator()( std::string const& value );
				void operator()( _2Real::CustomDataItem const& value );

				// gives you current data; ( a std::vector< uint8_t > ) you can append more and call getData again later.
				std::shared_ptr< _2Real::DataItem > getDataItem( const uint32_t bytesToSkip );

			private:

				std::unique_ptr< mongo::BSONObjBuilder >	mObjBuilder;

			};
		}
	}
}