#include "BlockUnitTestWidget.h"
#include <QtGui/QApplication>

#include <windows.h>
#include <iostream>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BlockUnitTestWidget testWidget;
	testWidget.setup("CameraCaptureBundle","CameraCaptureBlock");
	
	int iRet = a.exec();

	testWidget.shutdown();

	return iRet;
}
