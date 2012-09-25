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

using Poco::ScopedLock;
using Poco::FastMutex;

using namespace _2Real;

bool run = true;

class Receiver
{
public:
	Receiver()
	{
	}

	~Receiver()
	{
	}

	void receiveOtherMarkers( AppData const &data )
	{
		m_Access.lock();
		
		//std::vector < Point > otherMarkers = data.getData<std::vector <_2Real::Point> >();

		//for(int i = 0; i<otherMarkers.size(); i++)
		//{
		//	std::cout
		//		<< "Other Marker " << otherMarkers[i].getId()
		//		<< ": x=" << otherMarkers[i].x()
		//		<< "\ty=" << otherMarkers[i].y()
		//		<< "\tz=" << otherMarkers[i].z()
		//		<< "\t(" << otherMarkers[i].getLabel() << ")" << std::endl;
		//}

		m_Access.unlock();
	}

private:

	Poco::FastMutex			m_Access;
};


int main( int argc, char *argv[] )
{
	try
	{
		Receiver receiver;

		// start engine
		Engine &engine = Engine::instance();
		engine.setBaseDirectory( "../../bundles/bin" );

		// load bundles
		BundleHandle mhBundle = engine.loadBundle( "MovingHeadBundle" );
		BundleHandle sBundle = engine.loadBundle( "SerialBundle" );
		BundleHandle koniBundle = engine.loadBundle( "KinectOpenNIBundle" );
		
		// create blocks
		BlockHandle &mhBlock = mhBundle.createBlockInstance( "MovingHeadBlock" );
		BlockHandle &mhTBlock = mhBundle.createBlockInstance( "MovingHeadTrackingBlock" );
		BlockHandle &sBlock = sBundle.createBlockInstance( "SerialBlock" );
		BlockHandle &koniBlock = koniBundle.createBlockInstance( "KinectOpenNIUserSkeletonBlock" );

		// set update rates
		koniBlock.setUpdateRate( 30.0f );
		sBlock.setUpdateRate( 30.0f );
		mhTBlock.setUpdateRate( 30.0f );
		mhBlock.setUpdateRate( 30.0f );
		
		// get inlet handles

		// register outlet handles

		// link inlet with outlet handles

/*
		InletHandle	serverIPIn = mhBlock.getInletHandle( "server_ip" );
		serverIPIn.setUpdatePolicy( InletPolicy::ALWAYS );
		mhBlock.getOutletHandle( "other_marker" ).registerToNewData( receiver, &Receiver::receiveOtherMarkers );
		mhBlock.getInletHandle( "server_ip" ).link(mhBlock.getOutletHandle( "other_marker" ));
*/

		// setup blocks
		mhBlock.setup();
		mhTBlock.setup();
		sBlock.setup();
		koniBlock.setup();

		// start blocks
		mhBlock.start();
		mhTBlock.start();
		sBlock.start();
		koniBlock.start();

		while( ::run )
		{
			if( kbhit() )
				if( getch() == 'q' )
					break;

			Sleep( 10 );
		}

		// stop blocks
		mhBlock.stop();
		mhTBlock.stop();
		sBlock.stop();
		koniBlock.stop();

		// unregister outlets
//		mhBlock.getOutletHandle( "other_marker" ).unregisterFromNewData( receiver, &Receiver::receiveOtherMarkers );
		
		// save config		
		engine.safeConfig( "movinghead_test.xml" );

		// unload bundles
		mhBundle.unload();
		sBundle.unload();
		koniBundle.unload();
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;

#ifdef _DEBUG
		getchar();
#endif
	}

	return 0;
}