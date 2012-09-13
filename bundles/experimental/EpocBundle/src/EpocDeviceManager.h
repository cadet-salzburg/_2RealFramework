#pragma once
#include "_2RealBlock.h"

#include "Poco/Mutex.h"
#include <vector>

class EpocDeviceManagerImpl;

class EpocDeviceManager : public _2Real::bundle::ContextBlock
{
public:
	EpocDeviceManager();
	virtual ~EpocDeviceManager();
	virtual void							setup( _2Real::bundle::BlockHandle &context );
	virtual void							update();
	virtual void							shutdown();

	int getNumberOfConnectedDevices();
	int getDeviceID();
	bool isNotReceiving( unsigned int userID);

	_2Real::Vec3 getGyro( unsigned int userID );
	_2Real::Vec4 getAffectiv();

	bool isBlink();
	bool isRightWink();
	bool isLeftWink();
	bool isLookRight();
	bool isLookLeft();
	int getUpperFaceType();
	double getUpperFaceAmp();
	int getLowerFaceType();
	double getLowerFaceAmp();

	int getCognitivActionType();
	double getCognitivActionPower();

private:
	EpocDeviceManagerImpl	*m_managerImpl;
};