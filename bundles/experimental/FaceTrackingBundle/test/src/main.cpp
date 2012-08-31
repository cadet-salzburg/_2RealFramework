#include <_2RealDatatypes.h>
#include "_2RealBundlesUnitTest.h"
#include <QtGui/QApplication>

using namespace _2Real;
using namespace _2Real::app;

#include <windows.h>
#include <iostream>

using namespace _2Real;

int main( int argc, char *argv[] )
{
	QApplication a(argc, argv);

	BundleUnitTestWidget testBundle;
	testBundle.setup("FaceTrackingBundle");
	
	int iRet = a.exec();
	return iRet;
}