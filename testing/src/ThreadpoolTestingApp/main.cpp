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
using _2Real::BundleData;
using _2Real::BlockData;
using _2Real::app::BlockHandle;
using _2Real::app::BundleHandle;
using _2Real::app::InletHandle;
using _2Real::app::OutletHandle;
using _2Real::app::ParameterHandle;
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
		BundleHandle testBundle = testEngine.loadBundle( "ThreadpoolTesting" );

		for ( unsigned int i=0; i<1; ++i )
		{
			std::ostringstream msg;
			msg << "in # " << i;

			BlockHandle out = testBundle.createBlockInstance( "out" );
			OutletHandle outOut = out.getOutletHandle( "out outlet" );
			out.setUpdateRate( 1.0 );
			out.setUp();
			out.start();

			BlockHandle inout = testBundle.createBlockInstance( "in - out" );
			InletHandle inoutIn = inout.getInletHandle( "inout inlet" );
			OutletHandle inoutOut = inout.getOutletHandle( "inout outlet" );
			inout.setInletUpdatePolicy( BlockHandle::ALL_DATA_NEW );
			inout.setUpdateRate( 0.5 );
			inout.setUp();
			inout.start();

			BlockHandle in = testBundle.createBlockInstance( "in" );
			InletHandle inIn = in.getInletHandle( "in inlet" );
			ParameterHandle inParam = in.getParameterHandle( "in msg" );
			inParam.setValue< string >( msg.str() );
			in.setUpdateRate( 0.25 );
			in.setInletUpdatePolicy( BlockHandle::ALL_DATA_NEW );
			in.setUp();
			in.start();

			inoutIn.linkTo( outOut );
			inIn.linkTo( inoutOut );
		}
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

	return 0;
}