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
using _2Real::Engine;
using _2Real::System;
using _2Real::BundleIdentifier;
using _2Real::BlockIdentifier;
using _2Real::UpdatePolicyHandle;
using _2Real::Exception;

#include "vld.h"

#ifndef _DEBUG
	#define shared_library_suffix ".dll"
#else
	#define shared_library_suffix "_d.dll"
#endif

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	System testSystem( "test system" );

	try 
	{
		BundleIdentifier testBundle = testEngine.load( string( "ThreadpoolTesting" ).append( shared_library_suffix ) );

		for ( unsigned int i=0; i<50; ++i )
		{
			std::ostringstream msg;
			msg << "in # " << i;

			BlockIdentifier out = testSystem.createBlock( testBundle, "out" );
			UpdatePolicyHandle outHandle = testSystem.getUpdatePolicy( out );
			outHandle.updateWithFixedRate( 60.0 );
			testSystem.setup( out );
			testSystem.start( out );

			BlockIdentifier inout = testSystem.createBlock( testBundle, "in - out" );
			UpdatePolicyHandle inoutHandle = testSystem.getUpdatePolicy( inout );
			inoutHandle.updateWhenAllInletDataNew();
			testSystem.setup( inout );
			testSystem.start( inout );

			BlockIdentifier in = testSystem.createBlock( testBundle, "in" );
			UpdatePolicyHandle inHandle = testSystem.getUpdatePolicy( in );
			inHandle.updateWhenAllInletDataNew();
			testSystem.setValue< string >( in, "in msg", msg.str() );
			testSystem.setup( in );
			testSystem.start( in );

			testSystem.link( out, "out outlet", inout, "inout inlet" );
			testSystem.link( inout, "inout outlet", in, "in inlet" );
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
			//else if ( line == "a" )
			//{
			//	unsigned int cnt = 0;
			//	testSystem.setValue< unsigned int >( in, "inlet", ++cnt );
			//}
		}

	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
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

	testSystem.clear();

	cout << "SYSTEM CLEARED" << endl;

	while(1)
	{
		string line;
		char lineEnd = '\n';
		getline(cin, line, lineEnd);
		if (line == "q")
		{
			break;
		}
	}

	return 0;
}