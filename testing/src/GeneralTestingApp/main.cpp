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

#include <iostream>
#include <map>
#include <list>

#include "Poco/Mutex.h"

#ifndef _UNIX
	#include "vld.h"
#endif

using std::string;
using std::cout;
using std::endl;
using std::cin;

using _2Real::Exception;

using _2Real::app::Engine;
using _2Real::app::BundleInfo;
using _2Real::app::BlockInfo;
using _2Real::app::ContextBlockHandle;
using _2Real::app::BlockHandle;
using _2Real::app::BundleHandle;
using _2Real::app::InletHandle;
using _2Real::app::OutletHandle;
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
		cout << "RECEIVED (BLOCK): " << m_Received.size() << " DATA ITEMS" << endl;
		for ( std::list< AppData >::iterator it = m_Received.begin(); it != m_Received.end(); ++it )
		{
			cout << "RECEIVED (BLOCK): name: " << ( *it ).getName() << "\t\t val: ";
			cout << ( *it ).getDataAsString() << endl;
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
		cout << "RECEIVED (OUTLET): name: " << m_Received.getName() << "\t\t val: ";
		cout << m_Received.getDataAsString() << endl;
	}

private:

	AppData					m_Received;
	mutable FastMutex		m_Mutex;

};

int main( int argc, char *argv[] )
{
	try
	{
		Receiver receiver;
		Receiver contextOutlet;
		BlockReceiver blockReceiver;

		Engine &engine = Engine::instance();

		BlockHandle outHandle;
		BlockHandle inHandle;

		engine.setBaseDirectory( "." );
		BundleHandle bundleHandle = engine.loadBundle( "ContextTesting" );

		BundleInfo const& bundleData = bundleHandle.getBundleInfo();
		//cout << "BUNDLE INFO: EXPORTED BLOCKS" << endl;
		//for ( BundleInfo::BlockInfoConstIterator it = bundleData.getExportedBlocks().begin(); it != bundleData.getExportedBlocks().end(); ++it )
		//{
		//	cout << it->getName() << endl << it->getDescription() << endl << endl;
		//	for ( BlockInfo::ParameterInfoConstIterator pIt = it->getOutlets().begin(); pIt != it->getOutlets().end(); ++pIt )
		//	{
		//		cout << "OUTLET:" << endl;
		//		cout << pIt->getName() << endl << pIt->getLongTypename() << endl;
		//	}
		//	for ( BlockInfo::ParameterInfoConstIterator pIt = it->getInlets().begin(); pIt != it->getInlets().end(); ++pIt )
		//	{
		//		cout << "INLET:" << endl;
		//		cout << pIt->getName() << endl << pIt->getLongTypename() << endl;
		//	}
		//}

		//ContextBlockHandle const& contextHandle = bundleHandle.getContextBlock();
		//BlockInfo const& contextData = contextHandle.getBlockInfo();
		//for ( BlockInfo::ParameterInfoConstIterator it = contextData.getOutlets().begin(); it != contextData.getOutlets().end(); ++it )
		//{
		//	cout << "CONTEXT OUTLET:" << endl;
		//	cout << it->getName() << endl << it->getLongTypename() << endl;
		//}

		//OutletHandle contextOut = contextHandle.getOutletHandle( "devices" );
		//contextOut.registerToNewData( contextOutlet, &Receiver::receiveData );

		outHandle = bundleHandle.createBlockInstance( "out" );
		outHandle.setUpdateRate( 0.0 );

		//BlockHandle::OutletHandles const& outHandles = outHandle.getAllOutletHandles();
		//cout << outHandles.size() << endl;
		//for ( BlockHandle::OutletHandleConstIterator it = outHandles.begin(); it != outHandles.end(); ++it )
		//{
		//	cout << it->getName() << " " << it->getTypename() << std::endl;
		//}

		//BlockInfo const& outData = outHandle.getBlockInfo();
		//for ( BlockInfo::ParamConstIterator it = outData.getOutlets().begin(); it != outData.getOutlets().end(); ++it )
		//{
		//	cout << "OUT OUTLET:" << endl;
		//	cout << it->getName() << endl << it->getLongTypename() << endl;
		//}

		OutletHandle outletHandle = outHandle.getOutletHandle( "outlet" );

		inHandle = bundleHandle.createBlockInstance( "in" );
		inHandle.setUpdateRate( 0.0 );

		InletHandle inletHandle = inHandle.getInletHandle( "inlet" );
		inletHandle.setUpdatePolicy( _2Real::InletPolicy::OR_NEWER_DATA );

		//BlockHandle::InletHandles const& inHandles = inHandle.getAllInletHandles();
		//cout << inHandles.size() << endl;

		//InletHandle in2 = inHandle.getInletHandle( "device index" );

		unsigned int cnt = 0;
		printf("startup ok!\n");
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
				inletHandle.link( outletHandle );
			}
			else if ( line == "ulink" )
			{
				inletHandle.unlinkFrom( outletHandle );
			}
			else if ( line == "setup in" )
			{
				inHandle.setup();
			}
			else if ( line == "setup out" )
			{
				outHandle.setup();
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
				inletHandle.setValue< unsigned int >( ++cnt );
			}
			else if ( line == "buffer 10" )
			{
				inletHandle.setBufferSize( 10 );
			}
			else if ( line == "buffer 1" )
			{
				inletHandle.setBufferSize( 1 );
			}
			else if ( line == "links" )
			{
				Engine::Links links( engine.getCurrentLinks() );
				std::cout << "# OF LINKS: " << links.size() << std::endl;
			}
			else if ( line == "step in" )
			{
				inHandle.stop();
				inHandle.singleStep();
			}
			else if ( line == "step out" )
			{
				outHandle.stop();
				outHandle.singleStep();
			}
		}

		//outletHandle.unregisterFromNewData< Receiver >( receiver, &Receiver::receiveData );
		//contextOut.unregisterFromNewData( contextOutlet, &Receiver::receiveData );

		engine.clearAll();
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
		if ( line == "quit" )
		{
			break;
		}
	}

	return 0;
}
