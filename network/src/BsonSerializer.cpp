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

#include "common/_2RealBoostDefines.h"

#include "BsonSerializer.h"
#include "engine/_2RealHumanReadableNameVisitor.h"

namespace _2Real
{
	namespace io
	{
		namespace bson
		{
			template< typename TType >
			struct VectorSerializer_T
			{
				void operator()( std::vector< TType > const& data, mongo::BSONObjBuilder *builder ) const
				{
					_2Real::HumanReadableNameVisitor typeName;
					builder->append( protocol::TypeField, typeName( data ) );
					builder->append( protocol::ValueField, data.size() );

					int length = sizeof( TType )*data.size();
					builder->appendBinData( protocol::BinDataField, length, mongo::BinDataGeneral, reinterpret_cast< void const* >( &( data[0] ) ) );

					// array builder always crashes -> I'm doing here what they'd be doing anyway
					//uint32_t fieldCount = 0;
					//for ( auto element : data )
					//{
					//	std::string fieldName = std::to_string( fieldCount );
					//	builder->append( fieldName, element );
					//	fieldCount+=1;
					//}
					//builder->append( protocol::ValueField, fieldCount );
				}
			};

			template< >
			struct VectorSerializer_T< bool >
			{
				void operator()( std::vector< bool > const& data, mongo::BSONObjBuilder *builder ) const
				{
					_2Real::HumanReadableNameVisitor typeName;
					builder->append( protocol::TypeField, typeName( data ) );
					uint32_t fieldCount = 0;
					for ( auto element : data )
					{
						std::string fieldName = std::to_string( fieldCount );
						builder->append( fieldName, element );
						fieldCount+=1;
					}
					builder->append( protocol::ValueField, fieldCount );
				}
			};

			//template< >
			//struct VectorSerializer_T< uint64_t >
			//{
			//	void operator()( std::vector< uint64_t > const& data, mongo::BSONObjBuilder *builder ) const
			//	{
			//		_2Real::HumanReadableNameVisitor typeName;
			//		builder->append( protocol::TypeField, typeName( data ) );
	
			//		uint32_t fieldCount = 0;
			//		for ( uint64_t element : data )
			//		{
			//			std::string fieldName = std::to_string( fieldCount );
			//			builder->append( fieldName, std::to_string( element ) );
			//			fieldCount+=1;
			//		}
			//		builder->append( protocol::ValueField, fieldCount );
			//	}
			//};

			template< >
			struct VectorSerializer_T< std::string >
			{
				void operator()( std::vector< std::string > const& data, mongo::BSONObjBuilder *builder ) const
				{
					_2Real::HumanReadableNameVisitor typeName;
					builder->append( protocol::TypeField, typeName( data ) );
	
					uint32_t fieldCount = 0;
					for ( auto const& element : data )
					{
						std::string fieldName = std::to_string( fieldCount );
						builder->append( fieldName, element );
						fieldCount+=1;
					}
					builder->append( protocol::ValueField, fieldCount );
				}
			};

			template< typename TType >
			struct Serializer_T
			{
				void operator()( const TType data, mongo::BSONObjBuilder *builder ) const
				{
					_2Real::HumanReadableNameVisitor typeName;
					builder->append( protocol::TypeField, typeName( data ) );
					builder->append( protocol::ValueField, data );
				}
			};

			template< >
			struct Serializer_T< uint64_t >
			{
				void operator()( const uint64_t data, mongo::BSONObjBuilder *builder ) const
				{
					_2Real::HumanReadableNameVisitor typeName;
					builder->append( protocol::TypeField, typeName( data ) );
					builder->append( protocol::ValueField, std::to_string( data ) );
				}
			};

			Serializer::Serializer()
			{
				mObjBuilder.reset( new mongo::BSONObjBuilder() );
			}

			std::shared_ptr< _2Real::DataItem > Serializer::getDataItem( const uint32_t bytesToSkip )
			{
				auto obj = mObjBuilder->asTempObj();

				( void )( bytesToSkip );
				std::shared_ptr< _2Real::DataItem > result( new _2Real::DataItem );
				result->operator=( std::vector< uint8_t >() );
				auto &data = boost::get< std::vector< uint8_t > >( *result.get() );
				data.resize( obj.objsize() + bytesToSkip );
				memcpy( &data[ bytesToSkip ], obj.objdata(), obj.objsize() );

				// right, quick test
				//try
				//{
				//	mongo::BSONObj obj( reinterpret_cast< char * >( &data[ 20 ] ) );
				//}
				//catch ( std::exception &e  )
				//{
				//	std::cout << "TEST FAIL: " << e.what() << std::endl;
				//}

				return result;
			}

			void Serializer::operator()( std::vector< uint8_t > const& values )
			{
				VectorSerializer_T< uint8_t > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< int8_t > const& values )
			{
				VectorSerializer_T< int8_t > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< uint16_t > const& values )
			{
				VectorSerializer_T< uint16_t > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< int16_t > const& values )
			{
				VectorSerializer_T< int16_t > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< uint32_t > const& values )
			{
				VectorSerializer_T< uint32_t > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< int32_t > const& values )
			{
				VectorSerializer_T< int32_t > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< uint64_t > const& values )
			{
				VectorSerializer_T< uint64_t > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< int64_t > const& values )
			{
				VectorSerializer_T< int64_t > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< double > const& values )
			{
				VectorSerializer_T< double > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< float > const& values )
			{
				VectorSerializer_T< float > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< bool > const& values )
			{
				VectorSerializer_T< bool > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( std::vector< std::string > const& values )
			{
				VectorSerializer_T< std::string > serial;
				serial( values, mObjBuilder.get() );
			}

			void Serializer::operator()( const uint8_t value )
			{
				Serializer_T< uint8_t > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( const int8_t value )
			{
				Serializer_T< int8_t > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( const uint16_t value )
			{
				Serializer_T< uint16_t > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( const int16_t value )
			{
				Serializer_T< int16_t > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( const uint32_t value )
			{
				Serializer_T< uint32_t > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( const int32_t value )
			{
				Serializer_T< int32_t > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( const uint64_t value )
			{
				Serializer_T< uint64_t > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( const int64_t value )
			{
				Serializer_T< int64_t > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( const double value )
			{
				Serializer_T< double > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( const float value )
			{
				Serializer_T< float > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( const bool value )
			{
				Serializer_T< bool > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( std::string const& value )
			{
				Serializer_T< std::string > serial;
				serial( value, mObjBuilder.get() );
			}

			void Serializer::operator()( _2Real::CustomDataItem const& data )
			{
				mObjBuilder->append( protocol::TypeField, data.getName() );
				//mongo::BSONArrayBuilder arrayBuilder;
				//for ( auto const& iter : data.fields() )
				//{
				//	Serializer fieldSerializer;
				//	fieldSerializer.mObjBuilder->append( std::string( "fieldname" ), iter.getName() );
				//	boost::apply_visitor< Serializer >( fieldSerializer, iter.getValue() );
				//	arrayBuilder.append( fieldSerializer.mObjBuilder->obj() );
				//}
	
				//mObjBuilder->appendArray( protocol::ValueField, arrayBuilder.arr() );

				// for each field: build type/value BSONObj; append as 'fieldName'
				// doing it this way b/c the array builder crashes :/
				for ( auto const& iter : data.fields() )
				{
					Serializer fieldSerializer;
					boost::apply_visitor< Serializer >( fieldSerializer, iter.getValue() );
					mObjBuilder->append( iter.getName(), fieldSerializer.mObjBuilder->obj() );
				}
			}
		}
	}
}
