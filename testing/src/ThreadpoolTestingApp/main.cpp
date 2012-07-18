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

#include "_2RealApplication.h"

#include <windows.h>
#include <iostream>
#include <list>

using std::list;
using std::string;
using std::cout;
using std::endl;
using std::cin;
using _2Real::app::Engine;
using _2Real::Exception;
using _2Real::app::BlockHandle;
using _2Real::app::BundleHandle;
using _2Real::app::InletHandle;
using _2Real::app::OutletHandle;
using _2Real::app::AppData;

#include "vld.h"

#ifndef _DEBUG
	#define shared_library_suffix ".dll"
#else
	#define shared_library_suffix "_d.dll"
#endif

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();

	try 
	{
		testEngine.loadConfig( "test.xml" );
		//BundleHandle testBundle = testEngine.loadBundle( "ThreadpoolTesting" );

		//std::vector< BlockHandle > vec;
		//vec.resize( 30 );

		//for ( unsigned int i=0; i<10; ++i )
		//{
		//	std::ostringstream msg;
		//	msg << "in_instance_" << i;

		//	BlockHandle out = testBundle.createBlockInstance( "out" );
		//	OutletHandle outOut = out.getOutletHandle( "out_outlet" );
		//	out.setUpdateRate( 1.0 );
		//	//out.setup();

		//	BlockHandle inout = testBundle.createBlockInstance( "in_out" );
		//	InletHandle inoutIn = inout.getInletHandle( "inout_inlet" );
		//	OutletHandle inoutOut = inout.getOutletHandle( "inout_outlet" );
		//	inoutIn.setUpdatePolicy( InletHandle::OR_NEWER_DATA );
		//	inout.setUpdateRate( 0.5 );
		//	//inout.setup();

		//	BlockHandle in = testBundle.createBlockInstance( "in" );
		//	InletHandle inIn = in.getInletHandle( "in_inlet" );
		//	InletHandle inMsg = in.getInletHandle( "in_msg" );
		//	inMsg.setValue< string >( msg.str() );
		//	in.setUpdateRate( 0.25 );
		//	inIn.setUpdatePolicy( InletHandle::OR_NEWER_DATA );
		//	//in.setup();

		//	inoutIn.linkTo( outOut );
		//	inIn.linkTo( inoutOut );

		//	vec[ i ] = in;
		//	vec[ 10 + i ] = inout;
		//	vec[ 20 + i ] = out;
		//}

		//for ( std::vector< BlockHandle >::iterator it = vec.begin(); it != vec.end(); ++it )
		//{
		//	it->setup();
		//	it->start();
		//}
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;
	}

	while( 1 )
	{
		string line;
		char lineEnd = '\n';
		getline( cin, line, lineEnd );
		if ( line == "q" )
		{
			break;
		}
	}

	//testEngine.safeConfig( "test.xml" );

	return 0;
}