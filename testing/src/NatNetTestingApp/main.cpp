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

		printf("Other Markers [Count=%d]\n", otherMarkers.size());
		for(int i=0; i < otherMarkers.size(); i++)
		{
		printf("Other Marker %d : %3.2f\t%3.2f\t%3.2f\t(%s)\n",
			i,
			otherMarkers[i].x(),
			otherMarkers[i].y(),
			otherMarkers[i].z(),
			otherMarkers[i].getLabel());
	}

		m_Access.unlock();
	}

	void receiveRigidBodies( AppData const &data )
	{
		m_Access.lock();

		std::vector < RigidBody > rigidBodies = data.getData<std::vector <_2Real::RigidBody> >();

		printf("Rigid Bodies [Count=%d]\n", rigidBodies.size());
		for(int i=0; i < rigidBodies.size(); i++)
		{
			printf("Rigid Body [ID=%d, %s]\n", rigidBodies[i].getId(), rigidBodies[i].getLabel().c_str());
			printf("\tx\ty\tz\tqx\tqy\tqz\tqw\n");
			printf("\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\n",
				rigidBodies[i].getPosition().x(),
				rigidBodies[i].getPosition().y(),
				rigidBodies[i].getPosition().z(),
				rigidBodies[i].getOrientation().x(),
				rigidBodies[i].getOrientation().y(),
				rigidBodies[i].getOrientation().z(),
				rigidBodies[i].getOrientation().w());

			printf("\tRigid body markers [Count=%d]\n", rigidBodies[i].getPoints().size());
			for(int iMarker=0; iMarker < rigidBodies[i].getPoints().size(); iMarker++)
			{
				printf("\t\t");
                printf("MarkerID:%d", rigidBodies[i].getPoints()[iMarker].getId());
                printf("\tMarkerPos:%3.2f,%3.2f,%3.2f\n" ,
                    rigidBodies[i].getPoints()[iMarker].x(),
                    rigidBodies[i].getPoints()[iMarker].y(),
                    rigidBodies[i].getPoints()[iMarker].z());
			}
		}

		m_Access.unlock();
	}

	void receiveSkeletons( AppData const &data )
	{
		m_Access.lock();

		std::vector < Skeleton > skeletons = data.getData<std::vector <_2Real::Skeleton> >();

		 printf("Skeletons [Count=%d]\n", skeletons.size());
	    for(int i=0; i < skeletons.size(); i++)
		{
			printf("Skeleton [ID=%d, %s, Bone count=%d]\n", skeletons[i].getId(), skeletons[i].getLabel().c_str(), skeletons[i].getRigidBodies().size());
			for(int j=0; j< skeletons[i].getRigidBodies().size(); j++)
			{
				printf("Bone %d\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\n",
                    skeletons[i].getRigidBodies()[j].getId(), skeletons[i].getRigidBodies()[j].getPosition().x(), skeletons[i].getRigidBodies()[j].getPosition().y(), skeletons[i].getRigidBodies()[j].getPosition().z(), skeletons[i].getRigidBodies()[j].getOrientation().x(), skeletons[i].getRigidBodies()[j].getOrientation().y(), skeletons[i].getRigidBodies()[j].getOrientation().z(), skeletons[i].getRigidBodies()[j].getOrientation().w() );
    
				printf("\tRigid body markers [Count=%d]\n", skeletons[i].getRigidBodies()[j].getPoints().size());
				for(int iMarker=0; iMarker < skeletons[i].getRigidBodies()[j].getPoints().size(); iMarker++)
				{
					printf("\t\t");
                    printf("MarkerID:%d", skeletons[i].getRigidBodies()[j].getPoints()[iMarker].getId());
                    printf("\tMarkerPos:%3.2f,%3.2f,%3.2f\n" ,
                    skeletons[i].getRigidBodies()[j].getPoints()[iMarker].x(),
                    skeletons[i].getRigidBodies()[j].getPoints()[iMarker].y(),
                    skeletons[i].getRigidBodies()[j].getPoints()[iMarker].z());
				}
			}
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
		engine.setBaseDirectory( "../../bundles/bin" );

		BundleHandle nnBundle = engine.loadBundle( "NatNetBundle" );

		BlockHandle &natNetData = nnBundle.createBlockInstance( "NatNetBlock" );
		natNetData.setUpdateRate( 1.0f );

		InletHandle	serverIPIn = natNetData.getInletHandle( "server_ip" );;
		InletHandle	clientIPIn = natNetData.getInletHandle( "client_ip" );;
		InletHandle	isUnicastIn =natNetData.getInletHandle( "isUnicast" );;

		serverIPIn.setUpdatePolicy( InletPolicy::ALWAYS );
		clientIPIn.setUpdatePolicy( InletPolicy::ALWAYS );
		isUnicastIn.setUpdatePolicy( InletPolicy::ALWAYS );

		natNetData.getOutletHandle( "other_marker" ).registerToNewData( receiver, &Receiver::receiveOtherMarkers );
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
		natNetData.getOutletHandle( "rigid_body" ).unregisterFromNewData( receiver, &Receiver::receiveRigidBodies );
		natNetData.getOutletHandle( "skeleton" ).unregisterFromNewData( receiver, &Receiver::receiveSkeletons );

		engine.safeConfig( "natnet_test.xml" );

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