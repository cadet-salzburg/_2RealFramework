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

#include "_2RealBundle.h"

void getBundleMetainfo( _2Real::bundle::BundleMetainfo &info )
{
	// --- basic information

	info.setAuthor( "fhs33223" );
	info.setDescription( "test bundle for datatypes" );
	info.setCategory( "testing" );
	info.setContact( "fhs33223@fh-salzburg.ac.at" );
	info.setVersion( 0, 0, 0 );

	// -- types ( by name )

	info.exportsType( "simpleType", { _2Real::declareField( "int_field", "int" ), _2Real::declareField( "string_field", "string" ) } );
	info.exportsType( "complexType", { _2Real::declareField( "simple_field1", "simpleType" ), _2Real::declareField( "simple_field2", "simpleType" ), _2Real::declareField( "simple_field3", "simpleType" ), _2Real::declareField( "simple_field4", "simpleType" ), _2Real::declareField( "float_field", "float" ) } );
}

struct FindType : public std::unary_function< bool, const _2Real::bundle::CustomTypeMetainfo >
{
	explicit FindType( const std::string baseline ) : mBaseline( baseline ) {}
	bool operator()( const _2Real::bundle::CustomTypeMetainfo obj ) const { return mBaseline == obj.getName(); }
	std::string mBaseline;
};

void getTypeMetainfo( _2Real::bundle::CustomTypeMetainfo & info, _2Real::bundle::TypeMetainfoCollection const& existingTypes )
{
	if ( info.getName() == "simpleType" )
	{
		info.setDescription( "testing simple types, that is, types where all fields are not custom types" );
		info.setInitialFieldValue( "int_field", ( int32_t )15 );
		info.setInitialFieldValue( "string_field", std::string( "poit" ) );
	}
	else if ( info.getName() == "complexType" )
	{
		// ordering is preserved
		auto simpleInfo = std::static_pointer_cast< const _2Real::bundle::CustomTypeMetainfo >( existingTypes.getTypeMetainfo( "simpleType" ) );
		_2Real::CustomDataItem aSimple = simpleInfo->makeCustomData();
		aSimple.set( "int_field", ( int32_t )64 );
		aSimple.set( "string_field", std::string( "narf" ) );

		info.setDescription( "testing complex types, that is, types where at least one field is a custom type" );
		info.setInitialFieldValue( "simple_field1", aSimple );	
		info.setInitialFieldValue( "simple_field2", std::move( aSimple ) );
		info.setInitialFieldValue( "simple_field3", std::move( simpleInfo->makeData() ) );
		info.setInitialFieldValue( "simple_field4", simpleInfo->makeData() );
		info.setInitialFieldValue( "float_field", 10.5f );
	}
}