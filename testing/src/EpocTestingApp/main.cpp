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

	void receiveEngagement( AppData const &data )
	{
		m_Access.lock();

		double eng = data.getData<double>();

		std::cout << "received engagement: " << eng << std::endl;

		m_Access.unlock();
	}

	void receiveFrustration( AppData const &data )
	{
		m_Access.lock();

		double fru = data.getData<double>();

		std::cout << "received frustration: " << fru << std::endl;

		m_Access.unlock();
	}


	void receiveMeditation( AppData const &data )
	{
		m_Access.lock();

		double med = data.getData<double>();

		std::cout << "received meditation: " << med << std::endl;

		m_Access.unlock();
	}


	void receiveExcitement( AppData const &data )
	{
		m_Access.lock();

		double exc = data.getData<double>();

		std::cout << "received excitement: " << exc << std::endl;

		m_Access.unlock();
	}

	void receiveLaugh( AppData const &data )
	{
		m_Access.lock();

		double laugh = data.getData<double>();

		std::cout << "received laugh: " << laugh << std::endl;

		m_Access.unlock();
	}

	void receiveNeutral( AppData const &data )
	{
		m_Access.lock();

		double neutral = data.getData<double>();

		std::cout << "received neutral: " << neutral << std::endl;

		m_Access.unlock();
	}

	void receivePush( AppData const &data )
	{
		m_Access.lock();

		double push = data.getData<double>();

		std::cout << "received push: " << push << std::endl;

		m_Access.unlock();
	}

	void receivePull( AppData const &data )
	{
		m_Access.lock();

		double pull = data.getData<double>();

		std::cout << "received pull: " << pull << std::endl;

		m_Access.unlock();
	}

	void receiveLift( AppData const &data )
	{
		m_Access.lock();

		double lift = data.getData<double>();

		std::cout << "received lift: " << lift << std::endl;

		m_Access.unlock();
	}

	void receiveDrop( AppData const &data )
	{
		m_Access.lock();

		double drop = data.getData<double>();

		std::cout << "received drop: " << drop << std::endl;

		m_Access.unlock();
	}

	void receiveLeft( AppData const &data )
	{
		m_Access.lock();

		double left = data.getData<double>();

		std::cout << "received left: " << left << std::endl;

		m_Access.unlock();
	}

	void receiveRight( AppData const &data )
	{
		m_Access.lock();

		double right = data.getData<double>();

		std::cout << "received right: " << right << std::endl;

		m_Access.unlock();
	}

	void receiveRotateLeft( AppData const &data )
	{
		m_Access.lock();

		double rotLeft = data.getData<double>();

		std::cout << "received rotate left: " << rotLeft << std::endl;

		m_Access.unlock();
	}

	void receiveRotateRight( AppData const &data )
	{
		m_Access.lock();

		double rotRight = data.getData<double>();

		std::cout << "received rotate right: " << rotRight << std::endl;

		m_Access.unlock();
	}

	void receiveRotateClockwise( AppData const &data )
	{
		m_Access.lock();

		double rotCW = data.getData<double>();

		std::cout << "received rotate clockwise: " << rotCW << std::endl;

		m_Access.unlock();
	}

	void receiveRotateCounterClockwise( AppData const &data )
	{
		m_Access.lock();

		double rotCCW = data.getData<double>();

		std::cout << "received rotate counter clockwise: " << rotCCW << std::endl;

		m_Access.unlock();
	}

	void receiveRotateForwards( AppData const &data )
	{
		m_Access.lock();

		double rotForwards = data.getData<double>();

		std::cout << "received rotate forwards: " << rotForwards << std::endl;

		m_Access.unlock();
	}

	void receiveRotateReverse( AppData const &data )
	{
		m_Access.lock();

		double rotReverse = data.getData<double>();

		std::cout << "received rotate reverse: " << rotReverse << std::endl;

		m_Access.unlock();
	}

	void receiveDisappear( AppData const &data )
	{
		m_Access.lock();

		double disappear = data.getData<double>();

		std::cout << "received rotate reverse: " << disappear << std::endl;

		m_Access.unlock();
	}

	void receiveBlink( AppData const &data )
	{
		m_Access.lock();

		bool blink = data.getData<double>();

		if(blink)
			std::cout << "received blink!" << std::endl;

		m_Access.unlock();
	}

	void receiveRightWink( AppData const &data )
	{
		m_Access.lock();

		bool rWink = data.getData<double>();

		if(rWink)
			std::cout << "received right wink!" << std::endl;

		m_Access.unlock();
	}

	void receiveLeftWink( AppData const &data )
	{
		m_Access.lock();

		bool lWink = data.getData<double>();

		if(lWink)
			std::cout << "received left wink!" << std::endl;

		m_Access.unlock();
	}

	void receiveLookRight( AppData const &data )
	{
		m_Access.lock();

		bool lookR = data.getData<double>();

		if(lookR)
			std::cout << "received look right!" << std::endl;

		m_Access.unlock();
	}

	void receiveLookLeft( AppData const &data )
	{
		m_Access.lock();

		bool lookL = data.getData<double>();

		if(lookL)
			std::cout << "received look left!" << std::endl;

		m_Access.unlock();
	}

	void receiveRaiseBrow( AppData const &data )
	{
		m_Access.lock();

		double raiseBrow = data.getData<double>();

		std::cout << "received raise brow: " << raiseBrow << std::endl;

		m_Access.unlock();
	}

	void receiveFurrowBrow( AppData const &data )
	{
		m_Access.lock();

		double furrowBrow = data.getData<double>();

		std::cout << "received furrow brow: " << furrowBrow << std::endl;

		m_Access.unlock();
	}

	void receiveSmile( AppData const &data )
	{
		m_Access.lock();

		double smile = data.getData<double>();

		std::cout << "received smile: " << smile << std::endl;

		m_Access.unlock();
	}

	void receiveClench( AppData const &data )
	{
		m_Access.lock();

		double clench = data.getData<double>();

		std::cout << "received clench: " << clench << std::endl;

		m_Access.unlock();
	}

	void receiveSmirkRight( AppData const &data )
	{
		m_Access.lock();

		double smirkR = data.getData<double>();

		std::cout << "received smirk right: " << smirkR << std::endl;

		m_Access.unlock();
	}

	void receiveSmirkLeft( AppData const &data )
	{
		m_Access.lock();

		double smirkL = data.getData<double>();

		std::cout << "received smirk left: " << smirkL << std::endl;

		m_Access.unlock();
	}

	void receiveX( AppData const &data )
	{
		m_Access.lock();

		int x = data.getData<int>();
		if (x>0)
			std::cout << "received x: " << x << std::endl;

		m_Access.unlock();
	}

	void receiveY( AppData const &data )
	{
		m_Access.lock();

		int y = data.getData<int>();
		if (y>0)
			std::cout << "received y: " << y << std::endl;

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

		BundleHandle ecBundle = engine.loadBundle( "EpocBundle" );


		BlockHandle &affectivData = ecBundle.createBlockInstance( "EpocAffectivBlock" );
		affectivData.setUpdateRate( 30.0f );

		InletHandle affectivUserIdIn = affectivData.getInletHandle( "user_id" );

		affectivUserIdIn.setUpdatePolicy( InletPolicy::ALWAYS );

		affectivData.getOutletHandle( "engagement" ).registerToNewData( receiver, &Receiver::receiveEngagement );
		affectivData.getOutletHandle( "frustration" ).registerToNewData( receiver, &Receiver::receiveFrustration );
		affectivData.getOutletHandle( "meditation" ).registerToNewData( receiver, &Receiver::receiveMeditation );
		affectivData.getOutletHandle( "excitement" ).registerToNewData( receiver, &Receiver::receiveExcitement );

		affectivData.setup();
		affectivData.start();


		BlockHandle &cognitivData = ecBundle.createBlockInstance( "EpocCognitivBlock" );
		cognitivData.setUpdateRate( 30.0f );

		InletHandle cognitivUserIdIn = cognitivData.getInletHandle( "user_id" );

		cognitivUserIdIn.setUpdatePolicy( InletPolicy::ALWAYS );

		cognitivData.getOutletHandle( "neutral" ).registerToNewData( receiver, &Receiver::receiveNeutral );
		cognitivData.getOutletHandle( "push" ).registerToNewData( receiver, &Receiver::receivePush);
		cognitivData.getOutletHandle( "pull" ).registerToNewData( receiver, &Receiver::receivePull );
		cognitivData.getOutletHandle( "lift" ).registerToNewData( receiver, &Receiver::receiveLift );
		cognitivData.getOutletHandle( "drop" ).registerToNewData( receiver, &Receiver::receiveDrop );
		cognitivData.getOutletHandle( "left" ).registerToNewData( receiver, &Receiver::receiveLeft );
		cognitivData.getOutletHandle( "right" ).registerToNewData( receiver, &Receiver::receiveRight );
		cognitivData.getOutletHandle( "rotate_left" ).registerToNewData( receiver, &Receiver::receiveRotateLeft );
		cognitivData.getOutletHandle( "rotate_right" ).registerToNewData( receiver, &Receiver::receiveRotateRight );
		cognitivData.getOutletHandle( "rotate_clockwise" ).registerToNewData( receiver, &Receiver::receiveRotateClockwise );
		cognitivData.getOutletHandle( "rotate_counter_clockwise" ).registerToNewData( receiver, &Receiver::receiveRotateCounterClockwise );
		cognitivData.getOutletHandle( "rotate_forwards" ).registerToNewData( receiver, &Receiver::receiveRotateForwards );
		cognitivData.getOutletHandle( "rotate_reverse" ).registerToNewData( receiver, &Receiver::receiveRotateReverse );
		cognitivData.getOutletHandle( "disappear" ).registerToNewData( receiver, &Receiver::receiveDisappear );
		
		cognitivData.setup();
		cognitivData.start();


		BlockHandle &expressivData = ecBundle.createBlockInstance( "EpocExpressivBlock" );
		expressivData.setUpdateRate( 30.0f );

		InletHandle expressivUserIdIn = cognitivData.getInletHandle( "user_id" );

		expressivUserIdIn.setUpdatePolicy( InletPolicy::ALWAYS );

		expressivData.getOutletHandle( "blink" ).registerToNewData( receiver, &Receiver::receiveBlink );
		expressivData.getOutletHandle( "right_wink" ).registerToNewData( receiver, &Receiver::receiveRightWink );
		expressivData.getOutletHandle( "left_wink" ).registerToNewData( receiver, &Receiver::receiveLeftWink );
		expressivData.getOutletHandle( "look_right" ).registerToNewData( receiver, &Receiver::receiveLookRight );
		expressivData.getOutletHandle( "look_left" ).registerToNewData( receiver, &Receiver::receiveLookLeft );
		expressivData.getOutletHandle( "raise_brow" ).registerToNewData( receiver, &Receiver::receiveRaiseBrow );
		expressivData.getOutletHandle( "furrow_brow" ).registerToNewData( receiver, &Receiver::receiveFurrowBrow );
		expressivData.getOutletHandle( "smile" ).registerToNewData( receiver, &Receiver::receiveSmile );
		expressivData.getOutletHandle( "clench" ).registerToNewData( receiver, &Receiver::receiveClench );
		expressivData.getOutletHandle( "smirk_right" ).registerToNewData( receiver, &Receiver::receiveSmirkRight );
		expressivData.getOutletHandle( "smirk_left" ).registerToNewData( receiver, &Receiver::receiveSmirkLeft );
		expressivData.getOutletHandle( "laugh" ).registerToNewData( receiver, &Receiver::receiveLaugh );

		expressivData.setup();
		expressivData.start();


		BlockHandle &gyroData = ecBundle.createBlockInstance( "EpocGyroBlock" );
		gyroData.setUpdateRate( 30.0f );

		InletHandle userIdIn = gyroData.getInletHandle( "user_id" );

		userIdIn.setUpdatePolicy( InletPolicy::ALWAYS );
		
		gyroData.getOutletHandle( "gyro_x" ).registerToNewData( receiver, &Receiver::receiveX );
		gyroData.getOutletHandle( "gyro_y" ).registerToNewData( receiver, &Receiver::receiveY );
		
		gyroData.setup();
		gyroData.start();

		while( ::run )
		{
			if( kbhit() )
				if( getch() == 'q' )
					break;

			Sleep( 10 );
		}

		affectivData.stop();

		affectivData.getOutletHandle( "engagement" ).unregisterFromNewData( receiver, &Receiver::receiveEngagement );
		affectivData.getOutletHandle( "frustration" ).unregisterFromNewData( receiver, &Receiver::receiveFrustration );
		affectivData.getOutletHandle( "meditation" ).unregisterFromNewData( receiver, &Receiver::receiveMeditation );
		affectivData.getOutletHandle( "excitement" ).unregisterFromNewData( receiver, &Receiver::receiveExcitement );


		cognitivData.stop();

		cognitivData.getOutletHandle( "neutral" ).unregisterFromNewData( receiver, &Receiver::receiveNeutral );
		cognitivData.getOutletHandle( "push" ).unregisterFromNewData( receiver, &Receiver::receivePush);
		cognitivData.getOutletHandle( "pull" ).unregisterFromNewData( receiver, &Receiver::receivePull );
		cognitivData.getOutletHandle( "lift" ).unregisterFromNewData( receiver, &Receiver::receiveLift );
		cognitivData.getOutletHandle( "drop" ).unregisterFromNewData( receiver, &Receiver::receiveDrop );
		cognitivData.getOutletHandle( "left" ).unregisterFromNewData( receiver, &Receiver::receiveLeft );
		cognitivData.getOutletHandle( "right" ).unregisterFromNewData( receiver, &Receiver::receiveRight );
		cognitivData.getOutletHandle( "rotate_left" ).unregisterFromNewData( receiver, &Receiver::receiveRotateLeft );
		cognitivData.getOutletHandle( "rotate_right" ).unregisterFromNewData( receiver, &Receiver::receiveRotateRight );
		cognitivData.getOutletHandle( "rotate_clockwise" ).unregisterFromNewData( receiver, &Receiver::receiveRotateClockwise );
		cognitivData.getOutletHandle( "rotate_counter_clockwise" ).unregisterFromNewData( receiver, &Receiver::receiveRotateCounterClockwise );
		cognitivData.getOutletHandle( "rotate_forwards" ).unregisterFromNewData( receiver, &Receiver::receiveRotateForwards );
		cognitivData.getOutletHandle( "rotate_reverse" ).unregisterFromNewData( receiver, &Receiver::receiveRotateReverse );
		cognitivData.getOutletHandle( "disappear" ).unregisterFromNewData( receiver, &Receiver::receiveDisappear );


		expressivData.stop();

		expressivData.getOutletHandle( "blink" ).unregisterFromNewData( receiver, &Receiver::receiveBlink );
		expressivData.getOutletHandle( "right_wink" ).unregisterFromNewData( receiver, &Receiver::receiveRightWink );
		expressivData.getOutletHandle( "left_wink" ).unregisterFromNewData( receiver, &Receiver::receiveLeftWink );
		expressivData.getOutletHandle( "look_right" ).unregisterFromNewData( receiver, &Receiver::receiveLookRight );
		expressivData.getOutletHandle( "look_left" ).unregisterFromNewData( receiver, &Receiver::receiveLookLeft );
		expressivData.getOutletHandle( "raise_brow" ).unregisterFromNewData( receiver, &Receiver::receiveRaiseBrow );
		expressivData.getOutletHandle( "furrow_brow" ).unregisterFromNewData( receiver, &Receiver::receiveFurrowBrow );
		expressivData.getOutletHandle( "smile" ).unregisterFromNewData( receiver, &Receiver::receiveSmile );
		expressivData.getOutletHandle( "clench" ).unregisterFromNewData( receiver, &Receiver::receiveClench );
		expressivData.getOutletHandle( "smirk_right" ).unregisterFromNewData( receiver, &Receiver::receiveSmirkRight );
		expressivData.getOutletHandle( "smirk_left" ).unregisterFromNewData( receiver, &Receiver::receiveSmirkLeft );
		expressivData.getOutletHandle( "laugh" ).unregisterFromNewData( receiver, &Receiver::receiveLaugh );


		gyroData.stop();
		
		gyroData.getOutletHandle( "gyro_x" ).unregisterFromNewData( receiver, &Receiver::receiveX );
		gyroData.getOutletHandle( "gyro_y" ).unregisterFromNewData( receiver, &Receiver::receiveY );

		
		engine.safeConfig( "img_test.xml" );

		ecBundle.unload();
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