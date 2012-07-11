#include "BlockUnitTestWidget.h"
#include <QtGui/QApplication>
#include "_2RealApplication.h"
#include "_2RealDatatypes.h"

#include <windows.h>
#include <iostream>


using namespace _2Real;
using namespace _2Real::app;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BlockUnitTestWidget testWidget, testWidget1;

	std::string directory = "../experimental/bin/win/";
	Engine &engine = Engine::instance();
	_2Real::app::BundleHandle bundleHandle;

	try 
	{
		// load bundles for use in runtime engine
		engine.setBaseDirectory( directory );
		bundleHandle = engine.loadBundle( "CameraCaptureBundle" );
	}
	catch ( Exception &e )
	{
		std::cout << e.message() << " " << e.what() << endl;
	}

	testWidget.setup(bundleHandle,"CameraCaptureBlock");
	
	testWidget1.setup(bundleHandle,"CameraCaptureBlock");
	int iRet = a.exec();

	engine.clearAll();
		
	return iRet;
}
