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
#include "PrintInfoBlock.h"
#include "CounterBlock.h"

void getBundleMetainfo( _2Real::bundle::BundleMetainfo &info )
{
	info.setAuthor( "fhs33223" );
	info.setDescription( "a test bundle that exports simple blocks only - no context blocks and no types" );
	info.setCategory( "testing" );
	info.setContact( "fhs33223@fh-salzburg.ac.at" );
	info.setVersion( 0, 0, 0 );

	{
		_2Real::bundle::FunctionBlockMetainfo printInfoMeta = info.createFunctionBlockMetainfo( "print info" );

		printInfoMeta.setBlockClass< PrintInfo >();
		printInfoMeta.setDescription( "test block - prints the name/value of all io slots in setup, update & shutdown" );

		//printInfoMeta.setDependencies( { "name1", "name2", "name3" } );
		//std::vector< std::vector< std::string > > test2 = { { "a0", "b0", "c0" }, { "a1", "b1", "c1" } };

		printInfoMeta.setDefaultUpdatePolicy( _2Real::bundle::VALUES_NEW( _2Real::bundle::VALUES_NEW::ANY ) );

		_2Real::bundle::InletMetainfo inA = printInfoMeta.createInlet( "inA" );
		inA.setDescription( "the first inlet" );
		inA.setMultiInlet( false );
		inA.setDatatypeAndInitialValue( ( int32_t )0 );
		_2Real::bundle::InletMetainfo inB = printInfoMeta.createInlet( "inB" );
		inB.setDescription( "the second inlet" );
		inB.setMultiInlet( false );
		inB.setDatatypeAndInitialValue( 0.1f );
		_2Real::bundle::InletMetainfo inC = printInfoMeta.createInlet( "inC" );
		inC.setDescription( "the third inlet" );
		inC.setMultiInlet( true );
		inC.setDatatypeAndInitialValue( ( int64_t )pow( 2, 60 ) );
		_2Real::bundle::OutletMetainfo outA = printInfoMeta.createOutlet( "outA" );
		outA.setDescription( "the first outlet" );
		outA.setDatatypeAndInitialValue( ( bool )false );
		_2Real::bundle::OutletMetainfo outB = printInfoMeta.createOutlet( "outB" );
		outB.setDescription( "the second outlet" );
		outB.setDatatypeAndInitialValue( ( int32_t )-5 );
		_2Real::bundle::ParameterMetainfo paramA = printInfoMeta.createParameter( "paramA" );
		paramA.setDescription( "the first parameter" );
		paramA.setDatatypeAndInitialValue( ( uint32_t )10 );
		_2Real::bundle::ParameterMetainfo paramB = printInfoMeta.createParameter( "paramB" );
		paramB.setDescription( "the second parameter" );
		paramB.setDatatypeAndInitialValue( ( uint64_t )0 );
	}

	{
		_2Real::bundle::FunctionBlockMetainfo counterMeta = info.createFunctionBlockMetainfo( "counter" );

		counterMeta.setBlockClass< Counter >();
		counterMeta.setDescription( "test block - some simple counters" );
		counterMeta.setDefaultUpdatePolicy( _2Real::bundle::VALUES_NEW( _2Real::bundle::VALUES_NEW::ANY ) );

		_2Real::bundle::InletMetainfo inA = counterMeta.createInlet( "inA" );
		inA.setDescription( "int increment" );
		inA.setMultiInlet( false );
		inA.setDatatypeAndInitialValue( ( int32_t )10 );

		_2Real::bundle::InletMetainfo inB = counterMeta.createInlet( "inB" );
		inB.setDescription( "float increment" );
		inB.setMultiInlet( false );
		inB.setDatatypeAndInitialValue( 1.f );

		_2Real::bundle::InletMetainfo inC = counterMeta.createInlet( "inC" );
		inC.setDescription( "ulong increment" );
		inC.setMultiInlet( false );
		inC.setDatatypeAndInitialValue( ( uint64_t )1000 );

		_2Real::bundle::ParameterMetainfo paramA = counterMeta.createParameter( "paramA" );
		paramA.setDescription( "int init value" );
		paramA.setDatatypeAndInitialValue( ( int32_t )0 );

		_2Real::bundle::ParameterMetainfo paramB = counterMeta.createParameter( "paramB" );
		paramB.setDescription( "float init value" );
		paramB.setDatatypeAndInitialValue( 0.f );

		_2Real::bundle::ParameterMetainfo paramC = counterMeta.createParameter( "paramC" );
		paramC.setDescription( "ulong init value" );
		paramC.setDatatypeAndInitialValue( ( uint64_t )0 );

		_2Real::bundle::OutletMetainfo outA = counterMeta.createOutlet( "outA" );
		outA.setDescription( "int counter" );
		outA.setDatatypeAndInitialValue( ( int32_t )0 );

		_2Real::bundle::OutletMetainfo outB = counterMeta.createOutlet( "outB" );
		outB.setDescription( "float counter" );
		outB.setDatatypeAndInitialValue( 0.f );

		_2Real::bundle::OutletMetainfo outC = counterMeta.createOutlet( "outC" );
		outC.setDescription( "ulong counter" );
		outC.setDatatypeAndInitialValue( ( uint64_t )0 );
	}

}