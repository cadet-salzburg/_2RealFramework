#pragma once

#include <windows.h>
#include <ole2.h>
#include "MSR_NuiApi.h"

#include "Image.h"

class KinectDepthMap 
{

	public:
		
		KinectDepthMap();
		~KinectDepthMap();
		
		bool setup(unsigned int _width, unsigned int _height);
		void shutdown();
		bool update();
		
		Image< unsigned short, 2 > const& depthMap() const { return m_DepthMap; };

	private:

		HANDLE							m_DepthStreamHandle;
		::Image< unsigned short, 2 >	m_DepthMap;

		unsigned int					m_iCaptureWidth;
		unsigned int					m_iCaptureHeight;

		bool							m_bIsInitialized;
};