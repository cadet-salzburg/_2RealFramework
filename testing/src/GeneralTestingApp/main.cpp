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
#include "_2RealDatatypes.h"

#include <iostream>
#include <map>
#include <list>

#include "Poco/Mutex.h"

#include "vld.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;

using _2Real::Exception;

using _2Real::app::Engine;
using _2Real::app::BundleData;
using _2Real::app::BlockData;
using _2Real::app::BlockHandle;
using _2Real::app::BundleHandle;
using _2Real::app::InletHandle;
using _2Real::app::OutletHandle;
using _2Real::app::ParameterHandle;
using _2Real::app::AppData;

using Poco::ScopedLock;
using Poco::FastMutex;

class BlockReceiver
{

public:

	void receiveData( std::list< AppData > const& data )
	{
		ScopedLock< FastMutex > lock( m_Mutex );
		m_Received = data;
		cout << "RECEIVED: " << m_Received.size() << endl;
		for ( std::list< AppData >::iterator it = m_Received.begin(); it != m_Received.end(); ++it )
		{
			cout << "RECEIVED: " << ( *it ).getName() << endl;
			cout << "RECEIVED: " << ( *it ).getDataAsString() << endl;
		}
	}

private:

	std::list< AppData >	m_Received;
	mutable FastMutex		m_Mutex;

};

class Receiver
{

public:

	void receiveData( AppData const& data )
	{
		ScopedLock< FastMutex > lock( m_Mutex );
		m_Received = data;
		cout << "RECEIVED: " << m_Received.getName() << endl;
		cout << "RECEIVED: " << m_Received.getDataAsString() << endl;
	}

private:

	AppData					m_Received;
	mutable FastMutex		m_Mutex;

};

int main( int argc, char *argv[] )
{
	Engine &engine = Engine::instance();

	Receiver receiver;
	BlockReceiver blockReceiver;

	try
	{
		BundleHandle bundleHandle = engine.loadBundle( "ContextTesting" );
		BundleData const& bundleData = bundleHandle.getBundleData();

		BlockHandle outHandle = bundleHandle.createBlockInstance( "out" );
		BlockData const& outData = outHandle.getBlockData();
		outHandle.setUpdateRate( 0.2 );

		OutletHandle outletHandle = outHandle.getOutletHandle( "outlet" );

		BlockHandle inHandle = bundleHandle.createBlockInstance( "in" );
		BlockData const& inData = inHandle.getBlockData();
		inHandle.setUpdateRate( 1.0 );
		inHandle.setInletUpdatePolicy( BlockHandle::ALL_DATA_NEW );

		InletHandle inletHandle = inHandle.getInletHandle( "inlet" );
		inletHandle.setUpdatePolicy( InletHandle::DATA_NEW );

		ParameterHandle paramHandle = inHandle.getParameterHandle( "param" );

		unsigned int cnt = 0;

		while( 1 )
		{
			string line;
			char lineEnd = '\n';
			getline( cin, line, lineEnd );
			if ( line == "quit" )
			{
				break;
			}
			else if ( line == "link" )
			{
				inletHandle.linkTo( outletHandle );
			}
			else if ( line == "ulink" )
			{
				inletHandle.unlinkFrom( outletHandle );
			}
			else if ( line == "setup in" )
			{
				inHandle.setUp();
			}
			else if ( line == "setup out" )
			{
				outHandle.setUp();
			}
			else if ( line == "stop in" )
			{
				inHandle.stop();
			}
			else if ( line == "stop out" )
			{
				outHandle.stop();
			}
			else if ( line == "start in" )
			{
				inHandle.start();
			}
			else if ( line == "start out" )
			{
				outHandle.start();
			}
			else if ( line == "data in" )
			{
				AppData d = inletHandle.getCurrentInput();
				cout << d.getDataAsString() << endl;
			}
			else if ( line == "data out" )
			{
				AppData d = outletHandle.getLastOutput();
				cout << d.getDataAsString() << endl;
			}
			else if ( line == "reg outlet" )
			{
				outletHandle.registerToNewData< Receiver >( receiver, &Receiver::receiveData );
			}
			else if ( line == "reg out" )
			{
				outHandle.registerToNewData< BlockReceiver >( blockReceiver, &BlockReceiver::receiveData );
			}
			else if ( line == "unreg outlet" )
			{
				outletHandle.unregisterFromNewData< Receiver >( receiver, &Receiver::receiveData );
			}
			else if ( line == "unreg out" )
			{
				outHandle.unregisterFromNewData< BlockReceiver >( blockReceiver, &BlockReceiver::receiveData );
			}
			else if ( line == "set" )
			{
				paramHandle.setValue< unsigned int >( ++cnt );
			}
		}
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;
	}

	return 0;
}