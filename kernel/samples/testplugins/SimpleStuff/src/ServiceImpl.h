#pragma once

#include "_2RealIService.h"
#include "_2RealServiceContext.h"
#include "_2RealInputHandle.h"
#include "_2RealOutputHandle.h"

#include <iostream>
#include <vector>

_2Real::IService *const createVecInit();
_2Real::IService *const createVecAdd();
_2Real::IService *const createVecSub();

class VecInit : public _2Real::IService
{

public:

	VecInit() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~VecInit() {}

private:

	_2Real::OutputHandle			m_OutputVector;
	_2Real::InputHandle				m_ElementValue;
	_2Real::InputHandle				m_VectorSize;

};

class VecAdd : public _2Real::IService
{

public:

	VecAdd() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~VecAdd() {}

private:

	_2Real::OutputHandle			m_OutputVector;
	_2Real::InputHandle				m_InputVectorA;
	_2Real::InputHandle				m_InputVectorB;

};

class VecSub : public _2Real::IService
{

public:

	VecSub() {}
	void shutdown() {}
	void update();
	void setup(_2Real::ServiceContext &context);
	~VecSub() {}

private:

	_2Real::OutputHandle			m_OutputVector;
	_2Real::InputHandle				m_InputVectorA;
	_2Real::InputHandle				m_InputVectorB;

};