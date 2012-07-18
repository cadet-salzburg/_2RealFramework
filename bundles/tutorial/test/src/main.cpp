#include "_2RealBundlesUnitTest.h"
#include <QtGui/QApplication>

#ifndef _UNIX
    #include <windows.h>
#endif
#include <iostream>

using namespace _2Real;
using namespace _2Real::app;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	BundleUnitTestWidget testBundle;
	testBundle.setup("TutorialBundle");

	int iRet = a.exec();
	return iRet;
}
