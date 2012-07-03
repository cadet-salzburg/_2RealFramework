#pragma once
#include "_2RealBlock.h"
#include "videoInput.h"
#include <vector>

class CameraDeviceManager : public _2Real::bundle::ContextBlock
{
public:
	CameraDeviceManager() : ContextBlock() {};
	~CameraDeviceManager() {};
	void							setup( _2Real::bundle::BlockHandle &context );
	void							update();
	void							shutdown() {};

	int								getNumberOfDevices() const;
	bool							isDeviceAvailable( const unsigned int deviceIdx );
	_2Real::ImageT<unsigned char>	getPixels( const unsigned int deviceIdx );
private:
	static int						m_NumDevices;
	videoInput						m_VideoInputContoller;
};