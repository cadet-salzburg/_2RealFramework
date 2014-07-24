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

#include "BsonDeserializer.h"
#include "_2RealApplication.h"
#include "engine/_2RealHumanReadableNameVisitor.h"

namespace _2Real
{
	namespace io
	{
		namespace bson
		{
			template< typename TType >
			TType get( mongo::BSONElement &value );

			template< >
			uint8_t get( mongo::BSONElement &value )
			{
				return static_cast< uint8_t >( value.numberInt() );
			}

			template< >
			int8_t get( mongo::BSONElement &value )
			{
				return static_cast< int8_t >( value.numberInt() );
			}

			template< >
			uint32_t get( mongo::BSONElement &value )
			{
				return static_cast< uint32_t >( value.numberInt() );
			}

			template< >
			int32_t get( mongo::BSONElement &value )
			{
				return static_cast< int32_t >( value.numberInt() );
			}

			template< >
			uint64_t get( mongo::BSONElement &value )
			{
				return static_cast< uint64_t >( value.numberInt() );
			}

			template< >
			int64_t get( mongo::BSONElement &value )
			{
				return static_cast< int64_t >( value.numberInt() );
			}

			template< >
			float get( mongo::BSONElement &value )
			{
				return static_cast< float >( value.numberDouble() );
			}

			template< >
			double get( mongo::BSONElement &value )
			{
				return value.numberDouble();
			}

			template< >
			bool get( mongo::BSONElement &value )
			{
				return value.Bool();
			}

			template< >
			std::string get( mongo::BSONElement &value )
			{
				std::string result;
				value.Val( result );
				return result;
			}

			template< typename TType >
			std::vector< TType > vec( mongo::BSONObj obj, mongo::BSONElement &value )
			{
				uint32_t numElements = get< uint32_t >( value );
				std::cout << "vector size " << numElements << std::endl;

				std::vector< TType > result( numElements );
				for ( unsigned int i=0; i<numElements; ++i )
				{
					std::string label = std::to_string( i );
					auto field = obj.getField( label );
					result[ i ] = get< TType >( field );
				}
				return result;
			}

			std::shared_ptr< Deserializer > Deserializer::create( std::shared_ptr< const _2Real::DataItem > dataItem, const uint32_t headerBytes )
			{
				try
				{
					( void )( headerBytes );
					std::vector< uint8_t > const& data = boost::get< std::vector< uint8_t > >( *dataItem.get() );
					std::shared_ptr< mongo::BSONObj > obj( new mongo::BSONObj( reinterpret_cast< char const* >( &data[ 0/*headerBytes*/ ] ) ) );

					return std::shared_ptr< Deserializer >( new Deserializer( obj ) );
				}
				catch ( std::exception &e )
				{
					std::cout << "error in BSONDeserializer::create " << e.what() << std::endl;
					return nullptr;
				}
			}

			Deserializer::Deserializer( std::shared_ptr< mongo::BSONObj > obj ) : mObj( obj )
			{
			}

			std::string Deserializer::getTypename() const
			{
				return getTypename( *mObj.get() );
			}

			std::string Deserializer::getTypename( mongo::BSONObj &obj )
			{
				if ( obj.hasField( protocol::TypeField ) )
				{
					auto type = obj.getField( protocol::TypeField );
					return get< std::string >( type );
				}
				else
					throw _2Real::Exception( std::string( "missing required field: " ) + protocol::TypeField );
			}

			std::shared_ptr< _2Real::DataItem > Deserializer::getDataItem( std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) const
			{
				std::shared_ptr< _2Real::DataItem > result( new DataItem( info->makeData() ) );
				extract( *mObj.get(), *result.get(), info );
				return result;
			}

			void Deserializer::extract( mongo::BSONObj &obj, _2Real::DataItem &data, std::shared_ptr< const _2Real::app::TypeMetainfo_I > info )
			{
				std::string typeName = getTypename( obj );
				if ( info->getName() != typeName )
					throw _2Real::Exception( std::string( "type mismatch: " ) + info->getName() + std::string( " vs. " ) + typeName );

				if ( info->isBasicType() )
				{
					auto basicInfo = std::static_pointer_cast< const _2Real::app::TypeMetainfo >( info );

					if ( !obj.hasField( protocol::ValueField ) )
						throw _2Real::Exception( std::string( "missing required field: " ) + protocol::ValueField );
					auto value = obj.getField( protocol::ValueField );

					_2Real::HumanReadableNameVisitor name;
					if ( typeName == name( (uint8_t)0 ) )
						data = get< uint8_t >( value );
					else if ( typeName == name( (int8_t)0 ) )
						data = get< int8_t >( value );
					else if ( typeName == name( (uint32_t)0 ) )
						data = get< uint32_t >( value );
					else if ( typeName == name( (int32_t)0 ) )
						data = get< int32_t >( value );
					else if ( typeName == name( (uint64_t)0 ) )
						data = get< uint64_t >( value );
					else if ( typeName == name( (int64_t)0 ) )
						data = get< int64_t >( value );
					else if ( typeName == name( 0.f ) )
						data = get< float >( value );
					else if ( typeName == name( 0.0 ) )
						data = get< double >( value );
					else if ( typeName == name( std::string( "" ) ) )
						data = get< std::string >( value );
					else if ( typeName == name( false ) )
						data = get< bool >( value );
					else if ( typeName == name( std::vector< uint8_t >() ) )
						data = vec< uint8_t >( obj, value );
					else if ( typeName == name( std::vector< int8_t >() ) )
						data = vec< int8_t >( obj, value );
					else if ( typeName == name( std::vector< uint32_t >() ) )
						data = vec< uint32_t >( obj, value );
					else if ( typeName == name( std::vector< int32_t >() ) )
						data = vec< int32_t >( obj, value );
					else if ( typeName == name( std::vector< uint64_t >() ) )
						data = vec< uint64_t >( obj, value );
					else if ( typeName == name( std::vector< int64_t >() ) )
						data = vec< int64_t >( obj, value );
					else if ( typeName == name( std::vector< float >() ) )
						data = vec< float >( obj, value );
					else if ( typeName == name( std::vector< double >() ) )
						data = vec< double >( obj, value );
					else if ( typeName == name( std::vector< std::string >() ) )
						data = vec< std::string >( obj, value );
					else if ( typeName == name( std::vector< bool >() ) )
						data = vec< bool >( obj, value );
					else
						throw _2Real::Exception( std::string( "unknown type: " ) + typeName );
				}
				else
				{
					auto complexInfo = std::static_pointer_cast< const _2Real::app::CustomTypeMetainfo >( info );
					auto fields = complexInfo->getDataFields();

					_2Real::CustomDataItem &customData = boost::get< _2Real::CustomDataItem >( data );

					for ( auto it : fields )
					{
						auto fieldInfo = it.second;

						if ( !obj.hasField( it.first ) )
							throw _2Real::Exception( std::string( "missing required field " ) + it.first );

						auto field = obj.getField( it.first );

						_2Real::DataItem &item = customData.get( it.first );
						extract( field.Obj(), item, it.second );
					}
				}
			}
		}
	}
}