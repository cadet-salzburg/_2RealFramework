#include <iostream>
#include <string>
#include <sstream>
#include "EpocCognitivBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real::bundle;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

EpocCognitivBlock::EpocCognitivBlock( ContextBlock & context )
	:Block()
{
	m_EpocDeviceManager = static_cast<EpocDeviceManager*>( &context );
}

EpocCognitivBlock::~EpocCognitivBlock()
{
}

void EpocCognitivBlock::setup( BlockHandle &block )
{
	try
	{
		// inlet handles
		m_userIdIn = block.getInletHandle("user_id");

		// outlet handles
		m_neutralOut = block.getOutletHandle("neutral");
		m_pushOut = block.getOutletHandle("push");
		m_pullOut = block.getOutletHandle("pull");
		m_liftOut = block.getOutletHandle("lift");
		m_dropOut = block.getOutletHandle("drop");
		m_leftOut = block.getOutletHandle("left");
		m_rightOut = block.getOutletHandle("right");
		m_rotateLeftOut = block.getOutletHandle("rotate_left");
		m_rotateRightOut = block.getOutletHandle("rotate_right");
		m_rotateClockwiseOut = block.getOutletHandle("rotate_clockwise");
		m_rotateCounterClockwiseOut = block.getOutletHandle("rotate_counter_clockwise");
		m_rotateForwardsOut = block.getOutletHandle("rotate_forwards");
		m_rotateReverseOut = block.getOutletHandle("rotate_reverse");
		m_disappearOut = block.getOutletHandle("disappear");

		m_neutralOut.getWriteableRef<double>() = 0.0;
		m_pushOut.getWriteableRef<double>() = 0.0;
		m_pullOut.getWriteableRef<double>() = 0.0;
		m_liftOut.getWriteableRef<double>() = 0.0;
		m_dropOut.getWriteableRef<double>() = 0.0;
		m_leftOut.getWriteableRef<double>() = 0.0;
		m_rightOut.getWriteableRef<double>() = 0.0;
		m_rotateLeftOut.getWriteableRef<double>() = 0.0;
		m_rotateRightOut.getWriteableRef<double>() = 0.0;
		m_rotateClockwiseOut.getWriteableRef<double>() = 0.0;
		m_rotateCounterClockwiseOut.getWriteableRef<double>() = 0.0;
		m_rotateForwardsOut.getWriteableRef<double>() = 0.0;
		m_disappearOut.getWriteableRef<double>() = 0.0;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void EpocCognitivBlock::update()
{
	try
	{
		if(m_EpocDeviceManager)
		{
			if(m_EpocDeviceManager->getNumberOfConnectedDevices()<=0)	// if there is no device connected there is nothing todo so return
			{
				std::cout << "EpocCognitivBlock: EmoEngine is unable to acquire data for processing." << std::endl;

				m_neutralOut.discard();
				m_pushOut.discard();
				m_pullOut.discard();
				m_liftOut.discard();
				m_dropOut.discard();
				m_leftOut.discard();
				m_rightOut.discard();
				m_rotateLeftOut.discard();
				m_rotateRightOut.discard();
				m_rotateClockwiseOut.discard();
				m_rotateCounterClockwiseOut.discard();
				m_rotateForwardsOut.discard();
				m_rotateReverseOut.discard();
				m_disappearOut.discard();

				Sleep(1000);
				return;
			}
			if(m_EpocDeviceManager->getDeviceID() != m_userIdIn.getReadableRef<unsigned int>() )
			{
				m_neutralOut.discard();
				m_pushOut.discard();
				m_pullOut.discard();
				m_liftOut.discard();
				m_dropOut.discard();
				m_leftOut.discard();
				m_rightOut.discard();
				m_rotateLeftOut.discard();
				m_rotateRightOut.discard();
				m_rotateClockwiseOut.discard();
				m_rotateCounterClockwiseOut.discard();
				m_rotateForwardsOut.discard();
				m_rotateReverseOut.discard();
				m_disappearOut.discard();
				return;
			}

			int cognitivActionType = m_EpocDeviceManager->getCognitivActionType();

			switch(cognitivActionType)
			{
				case 1:
					m_neutralOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 2:
					m_neutralOut.discard();
					m_pushOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 3:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 4:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 5:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 6:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 7:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 8:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 9:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 10:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 11:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 12:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;

				case 13:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					m_disappearOut.discard();
					break;

				case 14:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.getWriteableRef<double>() = m_EpocDeviceManager->getCognitivActionPower();
					break;

				default:
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
					break;
			}
		}
		else
		{
					m_neutralOut.discard();
					m_pushOut.discard();
					m_pullOut.discard();
					m_liftOut.discard();
					m_dropOut.discard();
					m_leftOut.discard();
					m_rightOut.discard();
					m_rotateLeftOut.discard();
					m_rotateRightOut.discard();
					m_rotateClockwiseOut.discard();
					m_rotateCounterClockwiseOut.discard();
					m_rotateForwardsOut.discard();
					m_rotateReverseOut.discard();
					m_disappearOut.discard();
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void EpocCognitivBlock::shutdown()
{
	std::cout << "shutdown block" << std::endl;
}

