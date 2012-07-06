#include "_2RealBlock.h"

class CameraDeviceManager : public _2Real::bundle::ContextBlock
{

public:

	CameraDeviceManager() : ContextBlock() {}
	~CameraDeviceManager() {}
	void update();
	void shutdown() {}
	void setup( _2Real::bundle::BlockHandle &context );

private:

	_2Real::bundle::OutletHandle			m_ContextEnum;
	_2Real::bundle::OutletHandle			m_ContextNumber;

};