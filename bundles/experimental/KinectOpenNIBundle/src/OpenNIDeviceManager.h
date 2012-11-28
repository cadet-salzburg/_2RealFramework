/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
#pragma once
#include "_2RealBlock.h"
#include "_2RealKinect.h"
#include "Poco/Mutex.h"
#include <vector>
#include <map>

using namespace _2Real::bundle;

class OpenNIDeviceManager : public ContextBlock
{
public:
	OpenNIDeviceManager();
	~OpenNIDeviceManager();
	void							setup( _2Real::bundle::BlockHandle &context );
	void							update();
	void							shutdown();

	bool							bindGenerator(const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType, int w, int h, int fps=30);
	void							unbindGenerator(const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType);
	bool							setGeneratorParams(const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType, int w, int h, int fps);
	void							setMirrored(const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType, bool bIsMirrored);
	void							setAlignToColor(const unsigned int deviceIdx, bool bIsAligned);
	unsigned int					getNumberOfConnectedDevices();
	int								getNumberOfUsers( const unsigned int deviceIdx );
	int								getNumberOfSkeletons( const unsigned int deviceIdx);
	std::vector<_2Real::Skeleton>	getSkeletons(const unsigned int deviceIdx, bool bIsWorldCoordinates = false );
	_2Real::Skeleton				getSkeleton(const unsigned int deviceIdx, int userId, bool bIsWorldCoordinates );
	bool							isDeviceRunning(const unsigned int deviceIdx);

	_2Real::Image&					getImage( const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType, bool bIs16Bit = false);
	int								getWidth( const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType );
	int								getHeight( const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType );
	_2Real::Point					getUsersCenterOfMass(int deviceIdx, int userId, bool bIsWorldCoordinates);
	void							setMotorAngle(int deviceIdx, int angle);
	int								getMotorAngle(int deviceIdx);
	double							getFovH(int deviceIdx);
	double							getFovV(int deviceIdx);

	std::string						getLabelForJoint(_2RealKinectWrapper::_2RealJointType joint);

	void							projectiveToReal( const unsigned int deviceIdx, const unsigned int count, _2RealKinectWrapper::_2RealVector3f const* in, _2RealKinectWrapper::_2RealVector3f *out );
	//{
	//	m_2RealKinect->convertProjectiveToWorld( deviceIdx, count, in, out );
	//}

private:
	void							initDeviceList();
	void							rescanDeviceList();
	bool							isDeviceAvailable(const unsigned int deviceIdx);
	bool							isGeneratorFree(const unsigned int deviceIdx, _2RealKinectWrapper::_2RealGenerator generatorType);

	struct DeviceItem {
		DeviceItem::DeviceItem()
		{
		}
		_2Real::Image									    	m_ImageRgb;
		_2Real::Image									    	m_ImageIr;
		_2Real::Image									    	m_ImageDepth8Bit;
		_2Real::Image									    	m_ImageDepth16Bit;
		_2Real::Image									    	m_ImageUser;
		std::map<_2RealKinectWrapper::_2RealGenerator,bool>		m_bGeneratorIsUsed;
	};

	std::vector< DeviceItem >					m_DevicesInUse; 
	unsigned int								m_iNumDevices;
	_2RealKinectWrapper::_2RealKinect*			m_2RealKinect;
	Poco::Mutex									m_Mutex;
};