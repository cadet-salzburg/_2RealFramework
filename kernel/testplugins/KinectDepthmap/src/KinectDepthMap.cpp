#include "KinectDepthMap.h"

#include <iostream>

KinectDepthMap::KinectDepthMap() : m_iCaptureWidth(0), m_iCaptureHeight(0), m_bIsInitialized(false)
{
}

KinectDepthMap::~KinectDepthMap()
{
	if (m_bIsInitialized)
	{
		shutdown();
	}
}

bool KinectDepthMap::setup(unsigned int _width, unsigned int _height)
{
	if (m_bIsInitialized)
	{
		return false;
	}

	enum ::_NUI_IMAGE_RESOLUTION imageResolution;
	if (_width == 80 && _height == 60)
	{
		imageResolution = ::NUI_IMAGE_RESOLUTION_80x60;
	}
	else if (_width == 640 && _height == 480)
	{
		imageResolution = ::NUI_IMAGE_RESOLUTION_640x480;
	}
	else if (_width == 320 && _height == 240)
	{
		imageResolution = ::NUI_IMAGE_RESOLUTION_320x240;
	}
	else if (_width == 1280 && _height == 1024)
	{
		imageResolution = ::NUI_IMAGE_RESOLUTION_1280x1024;
	}
	else
	{
		imageResolution = ::NUI_IMAGE_RESOLUTION_INVALID;
		return false;
	}

	try 
	{
		::NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH);
		::NuiImageStreamOpen(::NUI_IMAGE_TYPE_DEPTH, imageResolution, 0, 2, NULL, &m_DepthStreamHandle);
	}
	catch(...) 
	{
		return false;
	}

	Image<unsigned short, 2>::Resolution res;
	res.set(0, _width);
	res.set(1, _height);
	m_DepthMap.setResolution(res);
	m_iCaptureWidth = _width;
	m_iCaptureHeight = _height;
	m_bIsInitialized = true;
	return true;
}

void KinectDepthMap::shutdown()
{
	::NuiShutdown();
	m_bIsInitialized = false;
}

bool KinectDepthMap::update()
{
	const NUI_IMAGE_FRAME* frame = NULL;
	HRESULT hr = NuiImageStreamGetNextFrame(m_DepthStreamHandle, 0, &frame);
	
	if(!FAILED(hr))
	{
		NuiImageBuffer* tex = frame->pFrameTexture;
		KINECT_LOCKED_RECT rect;
		tex->LockRect(0, &rect, NULL, 0);
		if (rect.Pitch != 0)
		{
			unsigned int sz = m_iCaptureWidth*m_iCaptureHeight;
			
			unsigned short* bits = (unsigned short*)rect.pBits;
			unsigned short* tmp =  new unsigned short[sz];

			memcpy(tmp, bits, sz);

			m_DepthMap.setData(tmp);
			
			tmp = NULL;
			bits = NULL;
		}

		tex->UnlockRect(0);
		NuiImageStreamReleaseFrame(m_DepthStreamHandle, frame);
		return true;
	}
	else
	{
		return false;
	}
}