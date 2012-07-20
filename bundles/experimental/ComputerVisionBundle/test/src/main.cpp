#include "_2RealBundlesUnitTest.h"
#include <QtGui/QApplication>

#include <windows.h>
#include <iostream>

#ifdef _WIN32
	#include <vld.h>
#endif

using namespace _2Real;
using namespace _2Real::app;

int main( int argc, char *argv[] )
{
	QApplication a( argc, argv );

	BundleUnitTestWidget testBundle;
	testBundle.setup( "ComputerVisionBundle" );
	
	int iRet = a.exec();
	return iRet;
}
