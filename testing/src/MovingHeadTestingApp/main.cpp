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
	: motorID(0)
	, command(0)
	, value(0)
	{}

	~Receiver() {}

	void receiveMotorID( AppData const &data )
	{
		m_Access.lock();
		motorID = data.getData<unsigned int>();
		m_Access.unlock();
	}

	void receiveCommand( AppData const &data )
	{
		m_Access.lock();
		command = data.getData<unsigned char>();
		m_Access.unlock();
	}

	void receiveValue( AppData const &data )
	{
		m_Access.lock();
		value = data.getData<unsigned int>();
		m_Access.unlock();
	}

	void printValues()
	{
		m_Access.lock();
		cout << motorID << " " << command << " " << value << endl;
		m_Access.unlock();
	}

private:
	Poco::FastMutex			m_Access;

	unsigned int motorID;
	unsigned char command;
	unsigned int value;
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
		koniBlock.setUpdateRate( 27.0f );
		sBlock.setUpdateRate( 17.0f );
		mhTBlock.setUpdateRate( 19.0f );
		mhBlock.setUpdateRate( 23.0f );

		// link inlet with outlet handles
		mhTBlock.getInletHandle("CentorOfMass").link(koniBlock.getOutletHandle("UsersCenterOfMass"));
		mhBlock.getInletHandle("MotorID").link(mhTBlock.getOutletHandle("MotorID"));
		mhBlock.getInletHandle("Value").link(mhTBlock.getOutletHandle("Value"));
		mhBlock.getInletHandle("Command").link(mhTBlock.getOutletHandle("Command"));
		sBlock.getInletHandle("Write").link(mhBlock.getOutletHandle("SerialByteStream"));
		
		// set inital inlet handle values
		mhTBlock.getInletHandle("MotorIDX").setValue<unsigned int>(5); // set proper values here
		mhTBlock.getInletHandle("MotorIDY").setValue<unsigned int>(15); // set proper values here
		sBlock.getInletHandle("SerialPort").setValue<std::string>("COM4"); // set proper values here
		sBlock.getInletHandle("BaudRate").setValue<unsigned int>(57600);

		// register outlet handles
		mhTBlock.getOutletHandle("MotorID").registerToNewData(receiver, &Receiver::receiveMotorID);
		mhTBlock.getOutletHandle("Command").registerToNewData(receiver, &Receiver::receiveCommand);
		mhTBlock.getOutletHandle("Value").registerToNewData(receiver, &Receiver::receiveValue);

		// setup blocks
		koniBlock.setup();
		mhTBlock.setup();
		mhBlock.setup();
		sBlock.setup();

		// start blocks
		koniBlock.start();
		mhTBlock.start();
		mhBlock.start();
		sBlock.start();

		while( ::run )
		{
			if( kbhit() )
				if( getch() == 'q' )
					break;

			receiver.printValues();
			Sleep( 100 );
		}

		// stop blocks
		mhBlock.stop();
		mhTBlock.stop();
		sBlock.stop();
		koniBlock.stop();
		
		// unregister outlet handles
		mhTBlock.getOutletHandle("MotorID").unregisterFromNewData(receiver, &Receiver::receiveMotorID);
		mhTBlock.getOutletHandle("Command").unregisterFromNewData(receiver, &Receiver::receiveCommand);
		mhTBlock.getOutletHandle("Value").unregisterFromNewData(receiver, &Receiver::receiveValue);

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
