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
		
		std::vector < Point > otherMarkers = data.getData<std::vector <_2Real::Point> >();

		for(int i = 0; i<otherMarkers.size(); i++)
		{
			std::cout
				<< "Other Marker " << otherMarkers[i].getId()
				<< ": x=" << otherMarkers[i].x()
				<< "\ty=" << otherMarkers[i].y()
				<< "\tz=" << otherMarkers[i].z()
				<< "\t(" << otherMarkers[i].getLabel() << ")" << std::endl;
		}

		m_Access.unlock();
	}

	void receiveMarkerSets( AppData const &data )
	{
		m_Access.lock();

		std::vector < Point > markerSets = data.getData<std::vector <_2Real::Point> >();

		for(int i = 0; i<markerSets.size(); i++)
		{
			std::cout 
				<< "Marker Set " << markerSets[i].getId()
				<< ": x=" << markerSets[i].x()
				<< "\ty=" << markerSets[i].y()
				<< "\tz=" << markerSets[i].z()
				<< "\t(" << markerSets[i].getLabel() << ")" << std::endl;
		}

		m_Access.unlock();
	}

	void receiveRigidBodies( AppData const &data )
	{
		m_Access.lock();

		std::vector < RigidBody > rigidBodies = data.getData<std::vector <_2Real::RigidBody> >();
		for(int i = 0; i<rigidBodies.size(); i++)
		{
			std::cout
				<< "Rigid Body" << rigidBodies[i].getId()
				<< ": x=" << rigidBodies[i].getPosition().x()
				<< "\ty=" << rigidBodies[i].getPosition().y()
				<< "\tz=" << rigidBodies[i].getPosition().z()
				<< "\tqx=" << rigidBodies[i].getOrientation().x()
				<< "\tqy=" << rigidBodies[i].getOrientation().y()
				<< "\tqz=" << rigidBodies[i].getOrientation().z()
				<< "\tqw=" << rigidBodies[i].getOrientation().w()
				<< "\t(" << rigidBodies[i].getLabel() << ")" << std::endl;
		}

		m_Access.unlock();
	}

	void receiveSkeletons( AppData const &data )
	{
		m_Access.lock();

		std::vector < Skeleton > skeletons = data.getData<std::vector <_2Real::Skeleton> >();

		for(int i = 0; i<skeletons.size(); i++)
		{
			std::cout
				<< "Skeleton" << skeletons[i].getId()
				<< "\t(" << skeletons[i].getLabel() << ")" << std::endl;
		}
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

		Engine &engine = Engine::instance();
		engine.setBaseDirectory( "C:\\Users\\veronikapa\\Desktop\\Projekte\\CADET\\cadet_git\\trunk\\bundles\\bin" );

		BundleHandle nnBundle = engine.loadBundle( "NatNetBundle" );

		BlockHandle &natNetData = nnBundle.createBlockInstance( "NatNetBlock" );
		natNetData.setUpdateRate( 30.0f ); // ???

		InletHandle	serverIPIn = natNetData.getInletHandle( "server_ip" );;
		InletHandle	clientIPIn = natNetData.getInletHandle( "client_ip" );;
		InletHandle	isUnicastIn =natNetData.getInletHandle( "isUnicast" );;

		// update-policy ??
		serverIPIn.setUpdatePolicy( InletHandle::ALWAYS );
		clientIPIn.setUpdatePolicy( InletHandle::ALWAYS );
		isUnicastIn.setUpdatePolicy( InletHandle::ALWAYS );

		natNetData.getOutletHandle( "other_marker" ).registerToNewData( receiver, &Receiver::receiveOtherMarkers );
		natNetData.getOutletHandle( "marker_set" ).registerToNewData( receiver, &Receiver::receiveMarkerSets );
		natNetData.getOutletHandle( "rigid_body" ).registerToNewData( receiver, &Receiver::receiveRigidBodies );
		natNetData.getOutletHandle( "skeleton" ).registerToNewData( receiver, &Receiver::receiveSkeletons );

		natNetData.setup();
		natNetData.start();

		while( ::run )
		{
			if( kbhit() )
				if( getch() == 'q' )
					break;

			Sleep( 10 );
		}

		natNetData.stop();

		natNetData.getOutletHandle( "other_marker" ).unregisterFromNewData( receiver, &Receiver::receiveOtherMarkers );
		natNetData.getOutletHandle( "marker_set" ).unregisterFromNewData( receiver, &Receiver::receiveMarkerSets );
		natNetData.getOutletHandle( "rigid_body" ).unregisterFromNewData( receiver, &Receiver::receiveRigidBodies );
		natNetData.getOutletHandle( "skeleton" ).unregisterFromNewData( receiver, &Receiver::receiveSkeletons );

		engine.safeConfig( "img_test.xml" );

		nnBundle.unload();
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