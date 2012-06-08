#include "CameraCaptureTest.h"
#include <QtGui/QApplication>

#include "_2RealEngine.h"
#include "_2RealSystem.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"
#include "_2RealUpdatePolicy.h"
#include "_2RealData.h"

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
using _2Real::Exception;
using _2Real::Data;
using Poco::FastMutex;
using Poco::ScopedLock;

#ifndef _DEBUG
	#define shared_library_suffix "_32.dll"
#else
	#define shared_library_suffix "_32d.dll"
#endif



void newCameraData(void *userdata, Data &data)
{
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CameraCaptureTest w;

	string directory = "../experimental/bin/win/";

	Engine &testEngine = Engine::instance();
	System testSystem( "test system" );

	try 
	{
		// load bundles for use in runtime engine
		testEngine.setBaseDirectory( directory );
		BundleIdentifier testBundle = testEngine.load( string( "CameraCaptureBundle" ).append( shared_library_suffix ) );

		// create certain blocks to a runtime system
		UpdatePolicy fpsTrigger;
		fpsTrigger.triggerByUpdateRate( 40.0f );
		BlockIdentifier cameraCaptureBlock = testSystem.createBlock( testBundle, "CameraCaptureBlock", fpsTrigger );
		
		// set needed setup parameters for block otherwise set to default
		
		// setup callbacks
		testSystem.registerToNewData( cameraCaptureBlock, "inputmidi", newCameraData );

		//start 
		testSystem.setup(cameraCaptureBlock);
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
	}

	w.show();
	int iRet = a.exec();

	testSystem.clear();

	return iRet;
}
