/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
#pragma once
#include "_2RealBlock.h"
#include "videoInput.h"
#include "Poco/Mutex.h"
#include <vector>

using namespace _2Real::bundle;

class CameraDeviceManager : public ContextBlock
{
public:
	CameraDeviceManager();
	~CameraDeviceManager();
	void							setup( _2Real::bundle::BlockHandle &context );
	void							update();
	void							shutdown();

	bool							bindDevice(const unsigned int deviceIdx, int w, int h, int fps=30);
	void							unbindDevice(const unsigned int deviceIdx);
	bool							setCameraParams(const unsigned int deviceIdx, int w, int h, int fps=30);
	unsigned int					getNumberOfConnectedDevices();
	bool							isDeviceRunning(const unsigned int deviceIdx);
	bool							isNewFrameAvailable(const unsigned int deviceIdx);
	int								getFirstFreeDevices();	// returns -1 for no free devices at all, otherwise int is the index for the free device
	_2Real::Image&  				getPixels( const unsigned int deviceIdx );
	int								getVideoWidth(const unsigned int  deviceIdx);
	int								getVideoHeight(const unsigned int  deviceIdx);

private:
	void							initDeviceList();
	void							rescanDeviceList();
	bool							isDeviceAvailable(const unsigned int deviceIdx);
	bool							isDeviceFree(const unsigned int deviceIdx);

	struct DeviceItem {
		DeviceItem::DeviceItem(std::string strDescription, bool bIsUsed) : m_strDescription(strDescription), m_bIsUsed(bIsUsed)
		{
		}
		_2Real::Image				m_Image;
		std::string						m_strDescription;
		bool							m_bIsUsed;
	};

	std::vector< DeviceItem >					m_DevicesInUse; 
	unsigned int								m_iNumDevices;
	videoInput*									m_VideoInputContoller;
	Poco::Mutex									m_Mutex;
};