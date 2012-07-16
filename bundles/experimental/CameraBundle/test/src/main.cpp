#include "_2RealBundlesUnitTest.h"
#include <QtGui/QApplication>

#include <windows.h>
#include <iostream>

using namespace _2Real;
using namespace _2Real::app;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	BundleUnitTestWidget testBundle;
	testBundle.setup("CameraCaptureBundle");
	
	int iRet = a.exec();
	return iRet;
}
