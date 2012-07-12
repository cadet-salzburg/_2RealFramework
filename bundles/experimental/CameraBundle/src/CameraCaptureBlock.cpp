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

CameraCaptureBlock::CameraCaptureBlock( ContextBlock & context )
	:Block()
{
	m_CameraDeviceManager = static_cast<CameraDeviceManager*>( &context );
}

CameraCaptureBlock::~CameraCaptureBlock()
{
}

void CameraCaptureBlock::setup( BlockHandle &block )
{
	try
	{
		// inlet handles
		m_DeviceIndexHandle	= block.getInletHandle("DeviceIndex");
		m_WidthHandle = block.getInletHandle("Width");
		m_HeightHandle = block.getInletHandle("Height");
		m_FpsHandle = block.getInletHandle("Fps");

		m_iWidth = m_WidthHandle.getReadableRef<int>();
		m_iHeight = m_HeightHandle.getReadableRef<int>();
		m_iFps = m_FpsHandle.getReadableRef<int>();

		// outlet handles
		m_ImageOutletHandle = block.getOutletHandle("ImageData" );
		m_iCurrentCamera = -1;	// no device set yet
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void CameraCaptureBlock::update()
{
	try
	{
		if(m_CameraDeviceManager->getNumberOfConnectedDevices()<=0)	// if there is no cameras connected there is nothing todo so return
			return;

		int cameraIndex = m_DeviceIndexHandle.getReadableRef<int>();
		int w = m_WidthHandle.getReadableRef<int>();
		int h = m_HeightHandle.getReadableRef<int>();
		int fps = m_FpsHandle.getReadableRef<int>();

		// camera index changed
		if(cameraIndex != m_iCurrentCamera)
		{
			// unbind old cam
			if(m_iCurrentCamera>=0)
			{
				m_CameraDeviceManager->unbindDevice( m_iCurrentCamera );
				m_iCurrentCamera = -1;
			}

			if( m_CameraDeviceManager->bindDevice( cameraIndex, m_iWidth, m_iHeight, m_iFps ) )
			{
				m_iCurrentCamera = cameraIndex;
			}
		}

		// there is a camera so grab image or set new parameters on this device if there are
		if(m_iCurrentCamera>=0)  
		{
			// resolution changed
			if(w != m_iWidth || h != m_iHeight || fps != m_iFps)
			{
				m_iWidth  = w;
				m_iHeight = h;
				m_iFps = fps;
				if(!m_CameraDeviceManager->setCameraParams(m_iCurrentCamera, m_iWidth, m_iHeight, m_iFps))	// failed ?
					m_iCurrentCamera = -1;
			}

			if( m_CameraDeviceManager->isDeviceRunning(m_iCurrentCamera))
			{
				m_ImageOutletHandle.getWriteableRef<_2Real::ImageT<unsigned char> >() = m_CameraDeviceManager->getPixels( m_iCurrentCamera );
			}
			else
			{
				m_iCurrentCamera = -1;		// it was all running and ok bu through a rescan a change in the device list got noticed and so all devices have to quit
			}
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void CameraCaptureBlock::shutdown()
{
	if(m_iCurrentCamera>=0)
	{
		m_CameraDeviceManager->unbindDevice( m_iCurrentCamera );
		m_iCurrentCamera = -1;
	}
}