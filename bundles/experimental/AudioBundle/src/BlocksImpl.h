#pragma once

#include "_2RealBlock.h"
#include "bundle/_2RealInletHandle.h"
#include "bundle/_2RealOutletHandle.h"

#include <iostream>
 

class AudioOutputService :public _2Real::bundle::Block
{

public:

	AudioOutputService() {}
	void shutdown();
	void update();
	void setup(_2Real::bundle::BlockHandle &context);
	~AudioOutputService() {}

private:

	  
	_2Real::bundle::OutletHandle			m_Test;
	 
};

 