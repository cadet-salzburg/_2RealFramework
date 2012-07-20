#pragma once
#include "_2RealBlock.h"
#include "_2RealKinect.h"
#include "Poco/Mutex.h"
#include <vector>

using namespace _2Real::bundle;

class OpenNIDeviceManager : public ContextBlock
{
public:
	OpenNIDeviceManager();
	~OpenNIDeviceManager();
	void							setup( _2Real::bundle::BlockHandle &context );
	void							update();
	void							shutdown();

	bool							bindDevice(const unsigned int deviceIdx, int w, int h, int fps=30);
	void							unbindDevice(const unsigned int deviceIdx);
	bool							setCameraParams(const unsigned int deviceIdx, int w, int h, int fps=30);
	unsigned int					getNumberOfConnectedDevices();
	bool							isDeviceRunning(const unsigned int deviceIdx);
	int								getFirstFreeDevices();	// returns -1 for no free devices at all, otherwise int is the index for the free device

	_2Real::ImageT<unsigned char>	getRgbImage( const unsigned int deviceIdx );

private:
	void							initDeviceList();
	void							rescanDeviceList();
	bool							isDeviceAvailable(const unsigned int deviceIdx);
	bool							isDeviceFree(const unsigned int deviceIdx);

	struct DeviceItem {
		DeviceItem::DeviceItem(bool bIsUsed) : m_bIsUsed(bIsUsed)
		{
		}
		_2Real::ImageT<unsigned char>	m_Image;
		bool							m_bIsUsed;
	};

	std::vector< DeviceItem >					m_DevicesInUse; 
	unsigned int								m_iNumDevices;
	_2RealKinectWrapper::_2RealKinect*			m_2RealKinect;
	Poco::Mutex									m_Mutex;
};