#pragma once
#include "_2RealBlock.h"
#include "videoInput.h"
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

	bool							bindDevice(const unsigned int deviceIdx);
	void							unbindDevice(const unsigned int deviceIdx);
	unsigned int					getNumberOfConnectedDevices() const;
	int								getFirstFreeDevices();	// returns -1 for no free devices at all, otherwise int is the index for the free device
	_2Real::ImageT<unsigned char>	getPixels( const unsigned int deviceIdx );
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
		_2Real::ImageT<unsigned char>	m_Image;
		std::string						m_strDescription;
		bool							m_bIsUsed;
	};

	std::vector< DeviceItem >					m_DevicesInUse; 
	unsigned int								m_iNumDevices;
	videoInput*									m_VideoInputContoller;
};