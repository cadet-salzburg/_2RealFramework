#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"

#include <iostream>

_2Real::IService *const createCounter();
_2Real::IService *const createDoubler();
_2Real::IService *const createPrintOut();

class Counter : public _2Real::IService
{

public:

	Counter() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~Counter() {}

private:

	int								m_CurrentCount;
	_2Real::OutputHandle			m_CounterValue;

};

class Doubler : public _2Real::IService
{

public:

	Doubler() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~Doubler() {}

private:

	_2Real::InputHandle				m_InputValue;
	_2Real::OutputHandle			m_OutputValue;

};

class PrintOut : public _2Real::IService
{

public:

	PrintOut() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~PrintOut() {}

private:

	_2Real::InputHandle				m_InputValue;

};