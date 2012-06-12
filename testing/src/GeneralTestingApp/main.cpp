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

#include "_2RealEngine.h"
#include "_2RealSystem.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"
#include "_2RealBlockError.h"
#include "_2RealOutputData.h"
#include "_2RealBundleData.h"
#include "_2RealBlockData.h"
#include "_2RealParameterData.h"
#include "_2RealUpdatePolicy.h"

#include "Poco/Mutex.h"

#include <windows.h>
#include <iostream>
#include <map>
#include <list>

//#include "vld.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::map;
using std::list;
using _2Real::Engine;
using _2Real::System;
using _2Real::BundleIdentifier;
using _2Real::BlockIdentifier;
using _2Real::UpdatePolicy;
using _2Real::OutputData;
using _2Real::Exception;
using _2Real::BlockError;
using _2Real::BundleData;
using _2Real::BlockData;
using _2Real::ParameterData;
using Poco::FastMutex;
using Poco::ScopedLock;

#ifndef _DEBUG
	#define shared_library_suffix ".dll"
#else
	#define shared_library_suffix "_d.dll"
#endif

template< typename T >
class Receiver
{

public:

	void receiveData( OutputData &data )
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

	void receiveError( BlockError &error )
	{
		cout << "exception in: " << error.getIdentifier().getName() << endl;
		cout << error.getException().message() << endl;
	}

private:

	FastMutex	m_Mutex;
	T			m_Data;

};

class BlockReceiver
{

public:

	void receiveData( list< OutputData > data )
	{
		try
		{
			ScopedLock< FastMutex > lock( m_Mutex );
			cout << "received: " << data.size() << " data items" << endl;

			for ( std::list< OutputData >::iterator it = data.begin(); it != data.end(); ++it )
			{
				cout << ( *it ).getName() << endl;
				cout << ( *it ).getTypename() << endl;
				cout << ( *it ).getDataAsString() << endl;
			}
		}
		catch ( Exception &e )
		{
			cout << e.message() << endl;
		}
	}

private:

	FastMutex	m_Mutex;

};

int main( int argc, char *argv[] )
{
	Receiver< unsigned int > *obj = new Receiver< unsigned int >();
	BlockReceiver *b = new BlockReceiver();

	Engine &testEngine = Engine::instance();
	System testSystem( "test system" );

	try
	{
		BundleIdentifier testBundle = testEngine.load( string( "GeneralTesting" ).append( shared_library_suffix ) );

		//cout << testBundle << endl;

		UpdatePolicy fpsTrigger;
		fpsTrigger.triggerByUpdateRate( 30.0f );

		UpdatePolicy newTrigger;
		newTrigger.triggerWhenAllDataNew();

		//BundleData const& bundleData = testEngine.getBundleData( testBundle );
		BlockData const& counterData = testEngine.getBlockData( testBundle, "counter" );
		//cout << bundleData << endl;
		//cout << counterData << endl;
		//map< string, BlockData > const& b = bundleData.getExportedBlocks();
		map< string, ParameterData > const& outlets = counterData.getOutlets();

		cout << "counter outlets: " << endl;
		for ( map< string, ParameterData >::const_iterator it = outlets.begin(); it != outlets.end(); ++it )
		{
			ParameterData p = it->second;
			cout << "name: " << p.getName() << endl;
			cout << "type: " << p.getType() << endl;
			cout << "key: " << p.getKeyword() << endl;
			cout << "category: " << p.getCategory() << endl;
			cout << "default value: " << p.getDefaultValue() << endl;
		}

		BlockIdentifier counter = testSystem.createBlock( testBundle, "counter", fpsTrigger );
		testSystem.setup( counter );

		BlockIdentifier doubler = testSystem.createBlock( testBundle, "doubler", newTrigger );
		testSystem.setup( doubler );

		BlockIdentifier print = testSystem.createBlock( testBundle, "print out", newTrigger );
		testSystem.setup( print );

		testSystem.link( counter, "counter outlet", doubler, "doubler inlet" );
		testSystem.link( doubler, "doubler outlet", print, "printout inlet" );
		//testSystem.registerToNewData( doubler, "doubler outlet", *obj, &Receiver< unsigned int >::receiveData );
		//testSystem.registerToException( *obj, &Receiver< unsigned int >::receiveError );
		testSystem.registerToNewData( doubler, *b, &BlockReceiver::receiveData );
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
	delete b;

	return 0;
}