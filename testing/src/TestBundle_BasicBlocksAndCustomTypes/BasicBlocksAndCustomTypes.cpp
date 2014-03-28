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
#include "ComplexCounterBlock.h"

void getBundleMetainfo( _2Real::bundle::BundleMetainfo &info )
{
	info.setAuthor( "fhs33223" );
	info.setDescription( "a test bundle that exports simple blocks and custom types - no context blocks" );
	info.setCategory( "testing" );
	info.setContact( "fhs33223@fh-salzburg.ac.at" );
	info.setVersion( 0, 0, 0 );

	_2Real::bundle::TypeMetainfo simpleType = info.createTypeMetainfo( "testtype_simple" );
	simpleType.setDescription( "a custom type w. simple fields" );
	simpleType.addField( "uchar_field", ( uint8_t )0 );
	simpleType.addField( "uint_field", ( uint32_t )0 );
	simpleType.addField( "ulong_field", ( uint64_t )0 );
	simpleType.addField( "double_field", 0.0 );
	simpleType.addField( "float_field", 0.f );

	_2Real::bundle::TypeMetainfo complexType = info.createTypeMetainfo( "testtype_complex" );
	complexType.setDescription( "testing complex types, that is, types where at least one field is a custom type" );
	complexType.addField( "string_field", std::string( "denn nudeln sind wir, und piraten wollen wir werden" ) );
	complexType.addField( "simple_field", simpleType.makeData() );

	_2Real::bundle::FunctionBlockMetainfo counterMeta = info.createFunctionBlockMetainfo( "complex counter" );
	counterMeta.setBlockClass< ComplexCounter >();
	counterMeta.setDescription( "test block, a counter w. a custom type" );
	counterMeta.setDefaultUpdatePolicy( _2Real::bundle::VALUES_NEW( _2Real::bundle::VALUES_NEW::ANY ) );

	_2Real::bundle::InletMetainfo inA = counterMeta.createInlet( "inc" );
	inA.setDescription( "increment" );
	inA.setMultiInlet( false );
	inA.setDatatypeAndInitialValue( simpleType.makeData() );

	_2Real::bundle::ParameterMetainfo paramA = counterMeta.createParameter( "init" );
	paramA.setDescription( "initial value" );
	// param will be used to init the counter ( simple type ) & the string field
	paramA.setDatatypeAndInitialValue( complexType.makeData() );

	_2Real::bundle::OutletMetainfo outA = counterMeta.createOutlet( "counter" );
	outA.setDescription( "counter value" );
	outA.setDatatypeAndInitialValue( complexType.makeData() );

}