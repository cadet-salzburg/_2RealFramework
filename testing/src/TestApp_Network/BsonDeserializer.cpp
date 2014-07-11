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
			std::shared_ptr< Deserializer > Deserializer::create( std::shared_ptr< const _2Real::DataItem > dataItem, const uint32_t headerBytes )
			{
				try
				{
					std::vector< uint8_t > const& data = boost::get< std::vector< uint8_t > >( *dataItem.get() );

					//std::cout << data.size() << std::endl;
					//for ( unsigned int i=0; i<data.size(); ++i)
					//	std::cout << data[i];
					//std::cout << std::endl;

					std::shared_ptr< mongo::BSONObj > obj( new mongo::BSONObj( reinterpret_cast< char const* >( &data[ headerBytes ] ) ) );

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
				if ( mObj->hasField( "type" ) )
				{
					auto typeElem = mObj->getField( "type" );

					// weird. prolly sth to do with /0 termination
					std::string typeName;
					typeElem.Val( typeName );
					return typeName;
				}
				// crap
				else return "INVALID";
			}

			std::shared_ptr< _2Real::DataItem > Deserializer::getDataItem( std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) const
			{
				auto dataItem = extractDataItem( *mObj.get(), info );
				return std::shared_ptr< _2Real::DataItem >( new _2Real::DataItem( dataItem ) );
			}

			_2Real::DataItem Deserializer::extractDataItem( mongo::BSONObj &obj, std::shared_ptr< const _2Real::app::TypeMetainfo_I > info ) const
			{
				if ( !obj.hasField( protocol::TypeField ) )
					throw _2Real::Exception( "missing required field: type" );

				// get type
				auto typeElem = obj.getField( protocol::TypeField );
				std::string typeName;
				typeElem.Val( typeName );

				// another sanity check: type fo field must match type in metainfo
				if ( info->getName() != typeName )
					throw _2Real::Exception( std::string( "type mismatch: " ) + info->getName() + std::string( " vs. " ) + typeName );

				if ( info->isBasicType() )
				{
					auto basicInfo = std::dynamic_pointer_cast< const _2Real::app::TypeMetainfo >( info );

		// vectors have no more value :/

					// since this object represents a basic type, it must have a field named value
					if ( !obj.hasField( protocol::ValueField ) )
						throw _2Real::Exception( "missing required field: value" );

					// get value
					auto valueElem = obj.getField( protocol::ValueField );
					_2Real::DataItem value;

					_2Real::HumanReadableNameVisitor tmp;
					if ( typeName == tmp.operator()( (uint8_t)0 ) )
					{
						std::cout << "unsigned char" << std::endl;
						int num = valueElem.numberInt();
						uint8_t val = static_cast< uint8_t >( num );
						std::cout << val << std::endl;
						value = val;
					}
					else if ( typeName == tmp.operator()( (int8_t)0 ) )
					{
						std::cout << "signed char" << std::endl;
						int num = valueElem.numberInt();
						int8_t val = static_cast< int8_t >( num );
						std::cout << val << std::endl;
						value = val;
					}
					else if ( typeName == tmp.operator()( (uint32_t)0 ) )
					{
						std::cout << "unsigned int" << std::endl;
						int num = valueElem.numberInt();
						uint32_t val = static_cast< uint32_t >( num );
						std::cout << val << std::endl;
						value = val;
					}
					else if ( typeName == tmp.operator()( (int32_t)0 ) )
					{
						std::cout << "signed int" << std::endl;
						int num = valueElem.numberInt();
						int32_t val = static_cast< int32_t >( num );
						std::cout << val << std::endl;
						value = val;
					}
					else if ( typeName == tmp.operator()( (uint64_t)0 ) )
					{
						std::cout << "unsigned long" << std::endl;
						long long num = valueElem.numberLong();
						uint64_t val = static_cast< uint64_t >( num );
						std::cout << val << std::endl;
						value = val;
					}
					else if ( typeName == tmp.operator()( (int64_t)0 ) )
					{
						std::cout << "signed long" << std::endl;
						long long num = valueElem.numberLong();
						int64_t val = static_cast< int64_t >( num );
						std::cout << val << std::endl;
						value = val;
					}
					else if ( typeName == tmp.operator()( 0.f ) )
					{
						std::cout << "float" << std::endl;
						double num = valueElem.numberDouble();
						float val = static_cast< float >( num );
						std::cout << val << std::endl;
						value = val;
					}
					else if ( typeName == tmp.operator()( 0.0 ) )
					{
						std::cout << "double" << std::endl;
						double num = valueElem.numberDouble();
						std::cout << num << std::endl;
						value = num;
					}
					else if ( typeName == tmp.operator()( std::string( "" ) ) )
					{
						std::cout << "string" << std::endl;
						std::string val;
						valueElem.Val( val );
						std::cout << val << std::endl;
						value = val;
					}
					else if ( typeName == tmp.operator()( false ) )
					{
						std::cout << "bool" << std::endl;
						bool val = valueElem.Bool();
						std::cout << val << std::endl;
						value = val;
					}
					else if ( typeName == tmp.operator()( std::vector< uint8_t >() ) )
					{
					}
					else if ( typeName == tmp.operator()( std::vector< int8_t >() ) )
					{
					}
					else if ( typeName == tmp.operator()( std::vector< uint32_t >() ) )
					{
					}
					else if ( typeName == tmp.operator()( std::vector< int32_t >() ) )
					{
						std::cout << "vector of signed int" << std::endl;

						uint32_t numElements = static_cast< uint32_t >( valueElem.numberInt() );

						std::cout << "size " << numElements << std::endl;
					}
					else if ( typeName == tmp.operator()( std::vector< uint64_t >() ) )
					{
					}
					else if ( typeName == tmp.operator()( std::vector< int64_t >() ) )
					{
					}
					else if ( typeName == tmp.operator()( std::vector< float >() ) )
					{
					}
					else if ( typeName == tmp.operator()( std::vector< double >() ) )
					{
					}
					else if ( typeName == tmp.operator()( std::vector< std::string >() ) )
					{
					}
					else if ( typeName == tmp.operator()( std::vector< bool >() ) )
					{
					}
					else
						throw _2Real::Exception( "unknown type" );

					return value;
				}
				else
				{
					std::cout << "custom type " << typeName << std::endl;

					// no value field required for complex types
					//if ( !obj.hasField( protocol::ValueField ) )
					//	throw _2Real::Exception( "missing required field: value" );

					// get value
					// auto valueElem = obj.getField( protocol::ValueField );
					// and convert to array
					// auto fieldArray = valueElem.Array();
					
					auto complexInfo = std::dynamic_pointer_cast< const _2Real::app::CustomTypeMetainfo >( info );
					auto fields = complexInfo->getDataFields();

					_2Real::DataItem result = info->makeData();
					_2Real::CustomDataItem &data = boost::get< _2Real::CustomDataItem >( result );

					for ( auto it : fields )
					{
						auto fieldInfo = it.second;

					// sanity check: bson object must have a field with same name
						if ( !obj.hasField( it.first ) )
							throw _2Real::Exception( std::string( "missing required field" ) + it.first );

					// get field as bson obj
						auto fieldObj = mObj->getField( it.first ).Obj();

						auto dataItem = extractDataItem( fieldObj, it.second );
						data.set( it.first, dataItem );
					}

					return result;
				}
			}

		}
	}
}