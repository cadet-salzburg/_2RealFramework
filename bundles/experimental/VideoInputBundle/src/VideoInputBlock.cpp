#include "VideoInputBlock.h"
#include <iostream>
#include <string>

using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

VideoInputBlock::VideoInputBlock(_2Real::bundle::ContextBlock & context )
	:Block()
{
	m_CameraDeviceManager = static_cast<CameraDeviceManager*>( &context );
}
void VideoInputBlock::setup( _2Real::bundle::BlockHandle &block )
{
	try
	{
		m_DeviceIndexHandle	= block.getParameterHandle("DeviceIndex");
		m_ImageOutletHandle = block.getOutletHandle( "imageOutlet" );
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
		int cameraIndex = abs( m_DeviceIndexHandle.getReadableRef<int>() )%m_CameraDeviceManager->getNumberOfDevices();  //Sanitize input
		m_ImageOutletHandle.getWriteableRef<_2Real::ImageT<unsigned char> >() = m_CameraDeviceManager->getPixels( cameraIndex );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};