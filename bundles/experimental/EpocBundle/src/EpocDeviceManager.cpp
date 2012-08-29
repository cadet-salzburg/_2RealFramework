#include "EpocDeviceManager.h"

#include "edk.h"
#include "edkErrorCode.h"
#include "EmoStateDLL.h"

#include <iostream>
#include <string>

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

using namespace _2Real;

class EpocDeviceManagerImpl
{
public:
	EpocDeviceManagerImpl() : 
		m_isShutDown( false )
	{}

	~EpocDeviceManagerImpl()
	{
		if( !m_isShutDown )
			this->shutdown();
	}

	void setup()
	{
		m_Event = EE_EmoEngineEventCreate();
		m_State = EE_EmoStateCreate();
		deviceID = -1;
		countUsers = 0;

		if (EE_EngineConnect() != EDK_OK) throw _2Real::Exception("Emotiv Engine start up failed.");
		printf("Start afer 8 seconds\n");
		Sleep(8000);
	}

	void update()
	{
/*
		int x, y;
		if(EE_HeadsetGetGyroDelta( 0, &x, &y))
			cout << "error1: " << EE_HeadsetGetGyroDelta( 0, &x, &y) << endl;
		std::cout << "update1: " << x << ", " << y << std::endl;
*/

		int state = EE_EngineGetNextEvent(m_Event);
		if( state == EDK_OK )
		{
			EE_Event_t eventType = EE_EmoEngineEventGetType(m_Event);	
			EE_EmoEngineEventGetUserId(m_Event, &deviceID);
			if(deviceID==-1)
				return;

			//if(EE_HeadsetGetGyroDelta( 0, &x, &y))
			//	cout << "error2: " << EE_HeadsetGetGyroDelta( 0, &x, &y) << endl;
			//std::cout << "update2: " << x << ", " << y << std::endl;

			if(eventType == EE_EmoStateUpdated  )
			{								
				// Copies an EmoState returned with a EE_EmoStateUpdate event to memory referenced by an EmoStateHandle. 
				EE_EmoEngineEventGetEmoState(m_Event, m_State);
			}
			else if( eventType == EE_UserAdded )
			{
				countUsers++;

				std::cout << "Device added. Current number of devices: " << countUsers << std::endl;
			}
			else if( eventType == EE_UserRemoved )
			{
				countUsers--;
				std::cout << "Device removed. Current number of devices: " << countUsers << std::endl;
			}
			deviceID = -1;
		}
		if(countUsers <= 0)
			std::cout << "No devices connected yet." << std::endl;
	}

	void shutdown()
	{
		EE_EngineDisconnect();

		EE_EmoStateFree(m_State);
		EE_EmoEngineEventFree(m_Event);	

		m_isShutDown = true;
	}

	Vec3 getGyro(unsigned int userID)
	{
		int x = 0; int y = 0;

		int err = EE_HeadsetGetGyroDelta( userID, &x, &y );
		Vec3 gyro;
		gyro[0] =x;
		gyro[1] =y;
		gyro[2] =err;
		
		return gyro;		
	}

	Vec4 getAffectiv()
	{
		float eng = ES_AffectivGetEngagementBoredomScore(m_State);
		float fru = ES_AffectivGetFrustrationScore(m_State);
		float med = ES_AffectivGetMeditationScore(m_State);
		float exc = ES_AffectivGetExcitementShortTermScore(m_State);

		return Vec4(eng, fru, med, exc);
	}


	bool isBlink()
	{
		return ES_ExpressivIsBlink(m_State);
	}

	bool isRightWink()
	{
		return ES_ExpressivIsRightWink(m_State);
	}

	bool isLeftWink()
	{
		return ES_ExpressivIsLeftWink(m_State);
	}

	bool isLookRight()
	{
		return ES_ExpressivIsLookingRight(m_State);
	}

	bool isLookLeft()
	{
		return ES_ExpressivIsLookingLeft(m_State);
	}

	int getUpperFaceType()
	{
		EE_ExpressivAlgo_t upperFaceType = ES_ExpressivGetUpperFaceAction(m_State);

		switch (upperFaceType) {
			case EXP_EYEBROW:	return 1;
			case EXP_FURROW:    return 2;
			default:			return 0;
		}
	}

	double getUpperFaceAmp()
	{
		return ES_ExpressivGetUpperFaceActionPower(m_State);
	}

	int getLowerFaceType()
	{
		EE_ExpressivAlgo_t lowerFaceType = ES_ExpressivGetLowerFaceAction(m_State);

		switch (lowerFaceType) {
			case EXP_CLENCH:		return 1;
			case EXP_SMILE:			return 2;
			case EXP_LAUGH:			return 3;
			case EXP_SMIRK_LEFT:	return 4;
			case EXP_SMIRK_RIGHT:	return 5;
			default:				return 0;
		}
	}

	double getLowerFaceAmp()
	{
		return ES_ExpressivGetLowerFaceActionPower(m_State);
	}


	int getCognitivActionType()
	{
		EE_CognitivAction_t actionType	= ES_CognitivGetCurrentAction(m_State);

		switch (actionType) {
			case COG_NEUTRAL:					return 1;
			case COG_PUSH:						return 2;
			case COG_PULL:						return 3;
			case COG_LIFT:						return 4;
			case COG_DROP:						return 5;
			case COG_LEFT:						return 6;
			case COG_RIGHT:						return 7;
			case COG_ROTATE_LEFT:				return 8;
			case COG_ROTATE_RIGHT:				return 9;
			case COG_ROTATE_CLOCKWISE:			return 10;
			case COG_ROTATE_COUNTER_CLOCKWISE:	return 11;
			case COG_ROTATE_FORWARDS:			return 12;
			case COG_ROTATE_REVERSE:			return 13;
			case COG_DISAPPEAR:					return 14;
			default:							return 0;
		}
	}

	double getCognitivActionPower()
	{
		return ES_CognitivGetCurrentActionPower(m_State);
	}

	int getNumberOfConnectedDevices()
	{
		return countUsers;
	}

	int getDeviceID()
	{
		return deviceID;
	}

	bool isNotReceiving( unsigned int userID)
	{
		/*
		int x1, y1;
		return (bool) EE_HeadsetGetGyroDelta(userID, &x1, &y1);
		*/
		return false;
	}

private:
	bool	m_isShutDown;

	EmoEngineEventHandle	m_Event;
	EmoStateHandle			m_State;
	unsigned int			deviceID;
	unsigned int			countUsers;
};


EpocDeviceManager::EpocDeviceManager() : 
	ContextBlock(),
	m_managerImpl( new EpocDeviceManagerImpl() )
{
	std::cout << "epocdevicemanager .ctor" << std::endl;
}

EpocDeviceManager::~EpocDeviceManager()
{
	std::cout << "epocdevicemanager .dtor" << std::endl;

	if( m_managerImpl )
	{
		delete m_managerImpl;
		m_managerImpl = NULL;
	}
}

void EpocDeviceManager::setup( BlockHandle &context )
{
	try
	{
		std::cout << "epocdevicemanager setup" << std::endl;

		if( m_managerImpl )
		m_managerImpl->setup();

	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

Vec3 EpocDeviceManager::getGyro( unsigned int userID )
{
	return m_managerImpl->getGyro( userID );
}

Vec4 EpocDeviceManager::getAffectiv()
{
	return m_managerImpl->getAffectiv();
}

bool EpocDeviceManager::isBlink()
{
	return m_managerImpl->isBlink();
}

bool EpocDeviceManager::isRightWink()
{
	return m_managerImpl->isRightWink();
}

bool EpocDeviceManager::isLeftWink()
{
	return m_managerImpl->isLeftWink();
}

bool EpocDeviceManager::isLookRight()
{
	return m_managerImpl->isLookRight();
}

bool EpocDeviceManager::isLookLeft()
{
	return m_managerImpl->isLookLeft();
}

int EpocDeviceManager::getUpperFaceType()
{
	return m_managerImpl->getUpperFaceType();
}

double EpocDeviceManager::getUpperFaceAmp()
{
	return m_managerImpl->getUpperFaceAmp();
}

int EpocDeviceManager::getLowerFaceType()
{
	return m_managerImpl->getLowerFaceType();
}

double EpocDeviceManager::getLowerFaceAmp()
{
	return m_managerImpl->getLowerFaceAmp();
}

int EpocDeviceManager::getCognitivActionType()
{
	return m_managerImpl->getCognitivActionType();
}

double EpocDeviceManager::getCognitivActionPower()
{
	return m_managerImpl->getCognitivActionPower();
}

int EpocDeviceManager::getNumberOfConnectedDevices()
{
	return m_managerImpl->getNumberOfConnectedDevices();
}

int EpocDeviceManager::getDeviceID()
{
	return m_managerImpl->getDeviceID();
}

bool EpocDeviceManager::isNotReceiving( unsigned int userID )
{
	return m_managerImpl->isNotReceiving( userID );
}

void EpocDeviceManager::update()
{
	try
	{
	//	std::cout << "epocdevicemanager update" << std::endl;

		if( m_managerImpl )
			m_managerImpl->update();
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void EpocDeviceManager::shutdown()
{
	std::cout << "epocdevicemanager shutdown" << std::endl;

	if( m_managerImpl )
		m_managerImpl->shutdown();
}