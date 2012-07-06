#include <iostream>
#include <string>
#include <sstream>
#include "CameraCaptureBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

VideoInputBlock::VideoInputBlock( ContextBlock & context )
	:Block()
{
	m_CameraDeviceManager = static_cast<CameraDeviceManager*>( &context );
}

void VideoInputBlock::setup( BlockHandle &block )
{
	try
	{
		m_DeviceIndexHandle	= block.getInletHandle("DeviceIndexInlet");
		m_ImageOutletHandle = block.getOutletHandle("ImageDataOutlet" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void VideoInputBlock::update()
{
	try
	{
		int cameraIndex = abs( m_DeviceIndexHandle.getReadableRef<int>() )%m_CameraDeviceManager->getNumberOfConnectedDevices() ;  //Sanitize input
		m_CameraDeviceManager->switchToDevice( cameraIndex );
		m_ImageOutletHandle.getWriteableRef<_2Real::ImageT<unsigned char> >() = m_CameraDeviceManager->getPixels( cameraIndex );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};