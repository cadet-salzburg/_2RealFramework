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
		m_DeviceIndexInletHandle = block.getInletHandle("DeviceIndex");
		m_WidthInletHandle = block.getInletHandle("Width");
		m_HeightInletHandle = block.getInletHandle("Height");
		m_FpsInletHandle = block.getInletHandle("Fps");

		m_iWidth = m_WidthInletHandle.getReadableRef<int>();
		m_iHeight = m_HeightInletHandle.getReadableRef<int>();
		m_iFps = m_FpsInletHandle.getReadableRef<int>();

		// outlet handles
		m_ImageOutletHandle = block.getOutletHandle("ImageData" );
		m_WidthOutletHandle = block.getOutletHandle("Width" );
		m_HeightOutletHandle = block.getOutletHandle("Height" );

		m_WidthOutletHandle.getWriteableRef<int>() = 0;
		m_HeightOutletHandle.getWriteableRef<int>() = 0;

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
		if(m_CameraDeviceManager->getNumberOfConnectedDevices()>0)	// if there is no cameras connected there is nothing todo 
		{
			int cameraIndex = m_DeviceIndexInletHandle.getReadableRef<int>();
			int w = m_WidthInletHandle.getReadableRef<int>();
			int h = m_HeightInletHandle.getReadableRef<int>();
			int fps = m_FpsInletHandle.getReadableRef<int>();

			// camera index changed
			if(cameraIndex != m_iCurrentCamera)
			{
				// unbind old cam
				if(m_iCurrentCamera>=0)
				{
					m_CameraDeviceManager->unbindDevice( m_iCurrentCamera );
					m_iCurrentCamera = -1;
				}

				if( m_CameraDeviceManager->bindDevice( cameraIndex, w, h, fps ) )
				{
					m_iCurrentCamera = cameraIndex;
					m_iWidth  = w;
					m_iHeight = h;
					m_iFps = fps;
				}
			}

			// there is a camera so grab image or set new parameters on this device if there are
			if(m_iCurrentCamera>=0)  
			{
				// resolution changed
				if(w != m_iWidth || h != m_iHeight || fps != m_iFps)
				{
					if(!m_CameraDeviceManager->setCameraParams(m_iCurrentCamera, w, h, fps))	// failed ?
					{
						m_CameraDeviceManager->unbindDevice( m_iCurrentCamera );
						m_iCurrentCamera = -1;
						discardOutlets();
						return;
					}
					else
					{
						m_iWidth  = w;
						m_iHeight = h;
						m_iFps = fps;
					}
				}

				if( m_CameraDeviceManager->isDeviceRunning(m_iCurrentCamera))
				{
					if(m_CameraDeviceManager->isNewFrameAvailable(m_iCurrentCamera))
					{
						m_ImageOutletHandle.getWriteableRef<_2Real::Image>() = m_CameraDeviceManager->getPixels( m_iCurrentCamera );
						m_WidthOutletHandle.getWriteableRef<int>() = m_CameraDeviceManager->getVideoWidth(m_iCurrentCamera);
						m_HeightOutletHandle.getWriteableRef<int>() = m_CameraDeviceManager->getVideoHeight(m_iCurrentCamera);
					}
					else
					{
						discardOutlets();
					}
				}
				else
				{
					discardOutlets();
					m_iCurrentCamera = -1;		// it was all running and ok bu through a rescan a change in the device list got noticed and so all devices have to quit
				}
			}
			else
			{
				discardOutlets();
			}
		}
		else
		{
			discardOutlets();
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
	std::cout << "shutdown block" << std::endl;
	if(m_iCurrentCamera>=0)
	{
		m_CameraDeviceManager->unbindDevice( m_iCurrentCamera );
		m_iCurrentCamera = -1;
	}
}

void CameraCaptureBlock::discardOutlets()
{
	m_WidthOutletHandle.discard();
	m_HeightOutletHandle.discard();
	m_ImageOutletHandle.discard();
}
