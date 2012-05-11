#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"

#include <iostream>

_2Real::IService *const createService1();

class Service1 : public _2Real::IService
{

public:

	Service1() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~Service1() {}

private:

	_2Real::InputHandle				m_InValue;
	_2Real::OutputHandle			m_OutValue;

};