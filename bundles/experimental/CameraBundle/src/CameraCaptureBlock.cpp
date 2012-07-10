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

		m_iCurrentCamera = -1;	// no device set yet
			
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void VideoInputBlock::update()
{
	try
	{
		if(m_CameraDeviceManager->getNumberOfConnectedDevices()<=0)	// if there is no cameras connected there is nothing todo so return
			return;

		int cameraIndex = abs( m_DeviceIndexHandle.getReadableRef<int>()  )%( m_CameraDeviceManager->getNumberOfConnectedDevices() ) ;  // sanitize input
		if(cameraIndex != m_iCurrentCamera)
		{
			// unbind old cam
			if(m_iCurrentCamera>=0)
			{
				m_CameraDeviceManager->unbindDevice( m_iCurrentCamera );
			}

			if( m_CameraDeviceManager->bindDevice( cameraIndex ) )
			{
				m_iCurrentCamera = cameraIndex;
			}
		}

		if(m_iCurrentCamera>=0)
		{
			m_ImageOutletHandle.getWriteableRef<_2Real::ImageT<unsigned char> >() = m_CameraDeviceManager->getPixels( m_iCurrentCamera );
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
