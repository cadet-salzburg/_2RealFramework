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

#include <Windows.h>

class TestObj
{

public:

	TestObj()
	{
		std::cout << "TEST OBJ CREATED" << std::endl;
	}

	~TestObj()
	{
		std::cout << "TEST OBJ DESTROIED" << std::endl;
	}

	void setup()
	{
		std::cout << "TEST OBJ SETUP" << std::endl;
		Sleep( 1000 );
	}

	void update()
	{
		std::cout << "TEST OBJ UPDATE" << std::endl;
		Sleep( 1000 );
	}

	void shutdown()
	{
		std::cout << "TEST OBJ SHUTDOWN" << std::endl;
		Sleep( 1000 );
	}

};

void getBundleMetainfo( _2Real::bundle::BundleMetainfo &info )
{
	info.setAuthor( "fhs33223" );
	info.setDescription( "test bundle" );
	info.setCategory( "testing" );
	info.setContact( "fhs33223@fh-salzburg.ac.at" );
	info.setVersion( 0, 0, 0 );

	_2Real::bundle::FunctionBlockMetainfo blockinfo = info.createFunctionBlockMetainfo( "blockA" );

	blockinfo.setBlockClass< TestObj >();
	blockinfo.setDescription( "the first test block" );
	blockinfo.setDependencies( { "name1", "name2", "name3" } );

	std::vector< std::string > test1 = { "a", "b", "c" };
	std::vector< std::vector< std::string > > test2 = { { "a0", "b0", "c0" }, { "a1", "b1", "c1" } };

	std::vector< std::vector< std::string > > params = { { "paramA", "paramB" } };

	blockinfo.setDefaultSetupPolicy( _2Real::bundle::VALUES_NEW( _2Real::bundle::VALUES_NEW( { { "paramA", "paramB" } } ) ) );
	blockinfo.setDefaultUpdatePolicy( _2Real::bundle::VALUES_NEW( _2Real::bundle::VALUES_NEW::ANY ) );

	_2Real::bundle::InletMetainfo inA = blockinfo.createInlet( "inA" );
	inA.setDescription( "the first inlet" );
	inA.setMultiInlet( false );
	inA.setDatatype( "int" );
	_2Real::bundle::InletMetainfo inB = blockinfo.createInlet( "inB" );
	inB.setDescription( "the second inlet" );
	inB.setMultiInlet( false );
	inB.setDatatype( "float" );
	_2Real::bundle::InletMetainfo inC = blockinfo.createInlet( "inC" );
	inC.setDescription( "the third inlet" );
	inC.setMultiInlet( true );
	inC.setDatatype( "string vector" );
	_2Real::bundle::OutletMetainfo outA = blockinfo.createOutlet( "outA" );
	outA.setDescription( "the first outlet" );
	outA.setDatatype( "bool" );
	_2Real::bundle::OutletMetainfo outB = blockinfo.createOutlet( "outB" );
	outB.setDescription( "the second outlet" );
	outB.setDatatype( "int" );
	_2Real::bundle::ParameterMetainfo paramA = blockinfo.createParameter( "paramA" );
	paramA.setDescription( "the first parameter" );
	paramA.setDatatype( "unsigned int" );
	_2Real::bundle::ParameterMetainfo paramB = blockinfo.createParameter( "paramB" );
	paramB.setDescription( "the second parameter" );
	paramB.setDatatype( "unsigned int" );

}