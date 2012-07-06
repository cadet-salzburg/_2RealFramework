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
	void							shutdown() 
	{
		m_VideoInputContoller.stopDevice( m_CurrentDeviceIndex );
	};

	int								getNumberOfConnectedDevices() const;
	bool							deviceIsSetup( const unsigned int deviceIdx );
	void							switchToDevice( const unsigned int deviceIdx );
	_2Real::ImageT<unsigned char>	getPixels( const unsigned int deviceIdx );
	int								getVideoWidth();
	int								getVideoHeight();

private:
	int								m_NumDevices;
	videoInput						m_VideoInputContoller;
	int								m_CurrentDeviceIndex;
};