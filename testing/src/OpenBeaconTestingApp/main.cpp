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
#include <sstream>
#include <map>
#include <list>

#include <conio.h>

#include "Poco/Mutex.h"

/*
#ifndef _UNIX
	#include "vld.h"
#endif
*/
#include <sstream>

#include "windows.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;

using _2Real::Exception;

using _2Real::app::Engine;
using _2Real::app::BundleInfo;
using _2Real::app::BlockInfo;
using _2Real::app::BlockHandle;
using _2Real::app::BundleHandle;
using _2Real::app::InletHandle;
using _2Real::app::OutletHandle;
using _2Real::app::AppData;
using _2Real::ImageT;

using Poco::ScopedLock;
using Poco::FastMutex;

using namespace _2Real;



class Receiver
{
public:
	Receiver()
	{
	}

	virtual ~Receiver()
	{
	}
	
	void receiveData( AppData const &data )
	{
		m_Access.lock();

		std::cout << "received dummy value: " << data.getData<int>() << std::endl;

		m_Access.unlock();
	}

protected:
	Poco::FastMutex		m_Access;
};


Receiver		*receiver	=	NULL;

Engine			&engine		=	Engine::instance();
BundleHandle	obBundle;
BlockHandle		obBlock;

InletHandle		dummyIn;
OutletHandle	dummyOut;


void setup()
{
	engine.setBaseDirectory( "C:\\Users\\AEC\\Desktop\\CADET_repos\\github\\trunk\\bundles\\bin" );

	obBundle = engine.loadBundle( "OpenBeaconBundle" );

	obBlock = obBundle.createBlockInstance( "OpenBeaconBlock" );
	obBlock.setUpdateRate( 2.0 );	//update every 2 seconds

	dummyIn = obBlock.getInletHandle( "dummy_in" );
	dummyOut = obBlock.getOutletHandle( "dummy_out" );

	dummyIn.setValue( 1 );
	dummyIn.setUpdatePolicy( InletPolicy::ALWAYS );

	receiver = new Receiver();

	dummyOut.registerToNewData( *receiver, &Receiver::receiveData );

	obBlock.setup();
	obBlock.start();
}

void cleanup()
{
	if( obBlock.isValid() )
		obBlock.stop();

	if( receiver )
	{
		if( dummyOut.isValid() )
			dummyOut.unregisterFromNewData( *receiver, &Receiver::receiveData );

		delete receiver;
		receiver = NULL;
	}

	engine.safeConfig( "img_test.xml" );

	if( obBundle.isValid() )
		obBundle.unload();
}

void __cdecl onExit()
{
	std::cout << "on exit called" << std::endl;

	cleanup();
}

void __cdecl onTerminate()
{
	std::cerr << "process terminated" << std::endl;
	abort();
}

void __cdecl onUnexpected()
{
	std::cerr << "an unexpected error occured" << std::endl;
}

int main( int argc, char *argv[] )
{
	atexit( onExit );
	std::set_terminate( onTerminate );
	std::set_unexpected( onUnexpected );

	try
	{
		setup();

		bool run = true;

		std::cout << "hit 'q' to quit" << std::endl;
		while( run )
		{
			if( _kbhit() )
			{
				char c = _getch();
				if( c == 'q' )
					run = false;
				else if( c >= '0' && c <= '9' )
				{
					std::cout << c << std::endl;
					dummyIn.setValue( (int)( c - '0' ) );
				}
			}

			Sleep( 10 );
		}
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;

#ifdef _DEBUG
		_getch();
#endif
	}

	return 0;
}