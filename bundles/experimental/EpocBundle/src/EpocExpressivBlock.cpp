#include <iostream>
#include <string>
#include <sstream>
#include "EpocExpressivBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real::bundle;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

EpocExpressivBlock::EpocExpressivBlock( ContextBlock & context )
	:Block()
{
	m_EpocDeviceManager = static_cast<EpocDeviceManager*>( &context );
}

EpocExpressivBlock::~EpocExpressivBlock()
{
}

void EpocExpressivBlock::setup( BlockHandle &block )
{
	try
	{
		// inlet handles
		m_userIdIn = block.getInletHandle("user_id");

		// outlet handles
		m_blinkOut = block.getOutletHandle("blink");
		m_rightWinkOut = block.getOutletHandle("right_wink");
		m_leftWinkOut = block.getOutletHandle("left_wink");
		m_lookRightOut = block.getOutletHandle("look_right");
		m_lookLeftOut = block.getOutletHandle("look_left");
		m_raiseBrowOut = block.getOutletHandle("raise_brow");
		m_furrowBrowOut = block.getOutletHandle("furrow_brow");
		m_smileOut = block.getOutletHandle("smile");
		m_clenchOut = block.getOutletHandle("clench");
		m_smirkRightOut = block.getOutletHandle("smirk_right");
		m_smirkLeftOut = block.getOutletHandle("smirk_left");
		m_laughOut = block.getOutletHandle("laugh");

		m_blinkOut.getWriteableRef<bool>() = 0;
		m_rightWinkOut.getWriteableRef<bool>() = 0;
		m_leftWinkOut.getWriteableRef<bool>() = 0;
		m_lookRightOut.getWriteableRef<bool>() = 0;
		m_lookLeftOut.getWriteableRef<bool>() = 0;
		m_raiseBrowOut.getWriteableRef<double>() = 0.0;
		m_furrowBrowOut.getWriteableRef<double>() = 0.0;
		m_smileOut.getWriteableRef<double>() = 0.0;
		m_clenchOut.getWriteableRef<double>() = 0.0;
		m_smirkRightOut.getWriteableRef<double>() = 0.0;
		m_smirkLeftOut.getWriteableRef<double>() = 0.0;
		m_laughOut.getWriteableRef<double>() = 0.0;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void EpocExpressivBlock::update()
{
	try
	{
		if(m_EpocDeviceManager)
		{
			if(m_EpocDeviceManager->getNumberOfConnectedDevices()<=0)	// if there is no device connected there is nothing todo so return
			{
				std::cout << "EpocExpressivBlock: EmoEngine is unable to acquire data for processing." << std::endl;
				
				m_blinkOut.discard();
				m_rightWinkOut.discard();
				m_leftWinkOut.discard();
				m_lookRightOut.discard();
				m_lookLeftOut.discard();
				m_raiseBrowOut.discard();
				m_furrowBrowOut.discard();
				m_smileOut.discard();
				m_clenchOut.discard();
				m_smirkRightOut.discard();
				m_smirkLeftOut.discard();
				m_laughOut.discard();

				Sleep(1000);
				return;
			}

			if(m_EpocDeviceManager->getDeviceID() != m_userIdIn.getReadableRef<unsigned int>() )
			{
				m_blinkOut.discard();
				m_rightWinkOut.discard();
				m_leftWinkOut.discard();
				m_lookRightOut.discard();
				m_lookLeftOut.discard();
				m_raiseBrowOut.discard();
				m_furrowBrowOut.discard();
				m_smileOut.discard();
				m_clenchOut.discard();
				m_smirkRightOut.discard();
				m_smirkLeftOut.discard();
				m_laughOut.discard();
				return;
			}

			if( m_EpocDeviceManager->isBlink() )
			{
				m_blinkOut.getWriteableRef<bool>() = true;
			}
			else
			{
				m_blinkOut.discard();
			}

			if( m_EpocDeviceManager->isRightWink() )
			{
				m_rightWinkOut.getWriteableRef<bool>() = true;
			}
			else
			{
				m_rightWinkOut.discard();
			}

			if( m_EpocDeviceManager->isLeftWink() )
			{
				m_leftWinkOut.getWriteableRef<bool>() = true;
			}
			else
			{
				m_leftWinkOut.discard();
			}

			if( m_EpocDeviceManager->isLookRight() )
			{
				m_lookRightOut.getWriteableRef<bool>() = true;
			}
			else
			{
				m_lookRightOut.discard();
			}

			if( m_EpocDeviceManager->isLookLeft() )
			{
				m_lookLeftOut.getWriteableRef<bool>() = true;
			}
			else
			{
				m_lookLeftOut.discard();
			}

			int upperType = m_EpocDeviceManager->getUpperFaceType();

			switch(upperType)
			{
			case 1:
				m_raiseBrowOut.getWriteableRef<double>() = m_EpocDeviceManager->getUpperFaceAmp();
				m_furrowBrowOut.discard();
				break;
			case 2:
				m_furrowBrowOut.getWriteableRef<double>() = m_EpocDeviceManager->getUpperFaceAmp();
				m_raiseBrowOut.discard();
				break;
			default:
				m_raiseBrowOut.discard();
				m_furrowBrowOut.discard();
			}
		
			int lowerType = m_EpocDeviceManager->getLowerFaceType();

			switch( lowerType ) {
				case 1:
					m_smileOut.getWriteableRef<double>() = m_EpocDeviceManager->getLowerFaceAmp();

					m_clenchOut.discard();
					m_smirkRightOut.discard();
					m_smirkLeftOut.discard();
					m_laughOut.discard();
					break;
				case 2:
					m_clenchOut.getWriteableRef<double>() = m_EpocDeviceManager->getLowerFaceAmp();

					m_smileOut.discard();
					m_smirkRightOut.discard();
					m_smirkLeftOut.discard();
					m_laughOut.discard();
					break;
				case 3:
					m_smirkRightOut.getWriteableRef<double>() = m_EpocDeviceManager->getLowerFaceAmp();

					m_smileOut.discard();
					m_clenchOut.discard();
					m_smirkLeftOut.discard();
					m_laughOut.discard();
					break;
				case 4:
					m_smirkLeftOut.getWriteableRef<double>() = m_EpocDeviceManager->getLowerFaceAmp();

					m_smileOut.discard();
					m_clenchOut.discard();
					m_smirkRightOut.discard();
					m_laughOut.discard();
					break;
				case 5:
					m_laughOut.getWriteableRef<double>() = m_EpocDeviceManager->getLowerFaceAmp();

					m_smileOut.discard();
					m_clenchOut.discard();
					m_smirkRightOut.discard();
					m_smirkLeftOut.discard();
					break;
				default:
					m_smileOut.discard();
					m_clenchOut.discard();
					m_smirkRightOut.discard();
					m_smirkLeftOut.discard();
					m_laughOut.discard();
			}
		}
		else
		{
			m_blinkOut.discard();
			m_rightWinkOut.discard();
			m_leftWinkOut.discard();
			m_lookRightOut.discard();
			m_lookLeftOut.discard();
			m_raiseBrowOut.discard();
			m_furrowBrowOut.discard();
			m_smileOut.discard();
			m_clenchOut.discard();
			m_smirkRightOut.discard();
			m_smirkLeftOut.discard();
			m_laughOut.discard();
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void EpocExpressivBlock::shutdown()
{
	std::cout << "shutdown block" << std::endl;
}