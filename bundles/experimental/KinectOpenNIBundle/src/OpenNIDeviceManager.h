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

	bool							bindGenerator(const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType, int w, int h, int fps=30);
	void							unbindGenerator(const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType);
	bool							setGeneratorParams(const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType, int w, int h, int fps);
	void							setMirrored(const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType, bool bIsMirrored);
	void							setAlignToColor(const unsigned int deviceIdx, bool bIsAligned);
	unsigned int					getNumberOfConnectedDevices();
	int								getNumberOfUsers( const unsigned int deviceIdx );
	int								getNumberOfSkeletons( const unsigned int deviceIdx);
	_2Real::Skeleton				getSkeletonScreen(const unsigned int deviceIdx, unsigned int userId );
	bool							isDeviceRunning(const unsigned int deviceIdx);

	_2Real::Image&					getImage( const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType, bool bIs16Bit = false);
	int								getWidth( const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType );
	int								getHeight( const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType );

private:
	void							initDeviceList();
	void							rescanDeviceList();
	bool							isDeviceAvailable(const unsigned int deviceIdx);
	bool							isGeneratorFree(const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType);

	struct DeviceItem {
		DeviceItem::DeviceItem()
		{
		}
		_2Real::Image										m_Image;
		std::map<_2RealKinectWrapper::_2RealGenerator,bool>		m_bGeneratorIsUsed;
	};

	std::vector< DeviceItem >					m_DevicesInUse; 
	unsigned int								m_iNumDevices;
	_2RealKinectWrapper::_2RealKinect*			m_2RealKinect;
	Poco::Mutex									m_Mutex;
};