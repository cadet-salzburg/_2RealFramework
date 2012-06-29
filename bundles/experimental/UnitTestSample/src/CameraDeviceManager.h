#include "_2RealBlock.h"
#include "_2RealOutletHandle.h"

class CameraDeviceManager : public _2Real::ContextBlock
{

public:

	CameraDeviceManager() : ContextBlock() {}
	~CameraDeviceManager() {}
	void update();
	void shutdown() {}
	void setup( _2Real::FrameworkContext &context );

private:

	_2Real::OutletHandle			m_ContextEnum;
	_2Real::OutletHandle			m_ContextNumber;

};