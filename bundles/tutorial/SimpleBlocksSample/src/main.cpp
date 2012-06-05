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

/*
* This tutorial application demonstrates the basic use of the Tutorial Plugin and its services.
* The sample uses a Counter service and prints its values out via another service.
* It also shows how the counter values are multiplied by 2 and then printed out again.
* 
*			TERMINATE THE PROGRAM WITH "q+ ENTER/RETURN"
*/

#include "_2RealEngine.h"
#include "_2RealSystem.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"
#include "_2RealData.h"
#include "_2RealUpdatePolicy.h"

#include "Poco/Mutex.h"

#include <windows.h>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using std::cin;
using _2Real::Engine;
using _2Real::System;
using _2Real::BundleIdentifier;
using _2Real::BlockIdentifier;
using _2Real::UpdatePolicy;
using _2Real::Data;
using _2Real::Exception;
using Poco::FastMutex;
using Poco::ScopedLock;

#ifndef _DEBUG
	#define shared_library_suffix "_32.dll"
#else
	#define shared_library_suffix "_32d.dll"
#endif

template< typename T >
class Receiver
{

public:

	void receiveData( Data &data )
	{
		try
		{
			ScopedLock< FastMutex > lock( m_Mutex );
			m_Data = data.getData< T >();
			cout << "received: " << m_Data << endl;
		}
		catch ( Exception &e )
		{
			cout << e.message() << endl;
		}
	}

private:

	FastMutex	m_Mutex;
	T			m_Data;

};

int main( int argc, char *argv[] )
{
	string directory = "..\\bin\\win\\";

	Receiver< unsigned int > *obj = new Receiver< unsigned int >();

	Engine &testEngine = Engine::instance();
	System testSystem( "test system" );

	try 
	{
		testEngine.setBaseDirectory( directory );
		BundleIdentifier testBundle = testEngine.load( string( "SimpleBlocksBundle" ).append( shared_library_suffix ) );

		UpdatePolicy fpsTrigger;
		fpsTrigger.triggerByUpdateRate( 0.5f );

		UpdatePolicy newTrigger;
		newTrigger.triggerWhenAllDataNew();

		BlockIdentifier counter = testSystem.createBlock( testBundle, "counter", fpsTrigger );
		testSystem.setup(counter);

		BlockIdentifier doubler = testSystem.createBlock( testBundle, "doubler", newTrigger );
		testSystem.setup(doubler);

		testSystem.link( counter, "counter outlet", doubler, "doubler inlet" );
		testSystem.registerToNewData( doubler, "doubler outlet", *obj, &Receiver< unsigned int >::receiveData );
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

	delete obj;

	return 0;
}