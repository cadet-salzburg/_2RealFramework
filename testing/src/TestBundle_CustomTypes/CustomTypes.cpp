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
	info.setAuthor( "fhs33223" );
	info.setDescription( "test bundle for datatypes" );
	info.setCategory( "testing" );
	info.setContact( "fhs33223@fh-salzburg.ac.at" );
	info.setVersion( 0, 0, 0 );

	// this setup was used to figure out the most 'efficient' way of creating custom types ( msvc++13 )
	// in terms of comfortability, i prefer the boost::variant based approach very much to the poco::any approach
	// - the api sure has gotten a lot cleaner, plus the variant suits our intent better than the any

	std::cout << "--------define simple type------" << std::endl;

	_2Real::bundle::TypeMetainfo simpleType = info.createTypeMetainfo( "testtype_simple" );
	simpleType.setDescription( "testing simple types, that is, types where all fields are not custom types" );
	std::cout << "--------simple int field--------" << std::endl;
	simpleType.addField( "int_field", ( int32_t )15 );
	std::cout << "--------simple str field--------" << std::endl;
	simpleType.addField( "string_field", std::string( "poit" ) );

	std::cout << "--------init simple type--------" << std::endl;

	_2Real::CustomDataItem aSimple;

	try
	{
		aSimple = simpleType.makeData();											// one copy, one move assignemnt
		aSimple.set( "int_field", ( int32_t )64 );
		aSimple.set( "string_field", std::string( "narf" ) );
		std::cout << aSimple << std::endl;
	}
	catch ( _2Real::Exception const& e )
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "--------define complex type-----" << std::endl;

	_2Real::bundle::TypeMetainfo complexType = info.createTypeMetainfo( "testtype_complex" );
	complexType.setDescription( "testing complex types, that is, types where at least one field is a custom type" );

	std::cout << "--------simple field 1----------" << std::endl;

	complexType.addField( "simple_field1", aSimple );								// one copy, followed by moves

	std::cout << "--------simple field 2----------" << std::endl;

	complexType.addField( "simple_field2", std::move( aSimple ) );					// moves only

	std::cout << "--------simple field 3----------" << std::endl;

	complexType.addField( "simple_field3", std::move( simpleType.makeData() ) );	// one copy, then moves

	std::cout << "--------simple field 4----------" << std::endl;

	complexType.addField( "simple_field4", simpleType.makeData() );					// one copy, then moves

	std::cout << "--------float field-------------" << std::endl;

	complexType.addField( "float_field", 10.5f );

	std::cout << "--------init complex type-------" << std::endl;

	_2Real::CustomDataItem aComplex = complexType.makeData();						// exactly one copy

	std::cout << "--------cout complex type-------" << std::endl;

	std::cout << aComplex << std::endl;

	std::cout << "--------init simple type-------" << std::endl;

	_2Real::CustomDataItem anotherSimple = simpleType.makeData();
	anotherSimple.set( "int_field", ( int32_t )100 );

	std::cout << "--------set complex type-------" << std::endl;

	aComplex.set( "simple_field3", anotherSimple );

	std::cout << "--------cout complex type-------" << std::endl;

	std::cout << aComplex << std::endl;

	std::cout << "--------done--------------------" << std::endl;

	/*
	*	TODO: one problem remains: what if the programmer now goes ahead and adds another field to the simpleType?
	*	basically, once i start creating instances of a custom type, no more adding of fiels should be allowed.
	*/
}