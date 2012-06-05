#pragma once

#include "_2RealBlock.h"
#include "_2RealInletHandle.h"
#include "_2RealOutletHandle.h"

#include <iostream>
 

class AudioOutputService :public _2Real::Block
{

public:

	AudioOutputService() {}
	void shutdown();
	void update();
	void setup(_2Real::FrameworkContext &context);
	~AudioOutputService() {}

private:

	  
	_2Real::OutletHandle			m_Test;
	 
};

 