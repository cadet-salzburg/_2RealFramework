#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"

#include <iostream>
 

class AudioOutputService : public _2Real::IService
{

public:

	AudioOutputService() {}
	void shutdown();
	void update();
	void setup(_2Real::ServiceContext &context);
	~AudioOutputService() {}

private:

	  
	_2Real::OutputHandle			m_Test;
	 
};

 