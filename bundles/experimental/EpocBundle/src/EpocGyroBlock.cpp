#include <iostream>
#include <string>
#include <sstream>
#include "EpocGyroBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

EpocGyroBlock::EpocGyroBlock( _2Real::bundle::ContextBlock & context )
	:Block()
{
	m_EpocDeviceManager = static_cast<EpocDeviceManager*>( &context );
}

EpocGyroBlock::~EpocGyroBlock()
{
}

void EpocGyroBlock::setup( BlockHandle &block )
{
	try
	{
		// inlet handles
		m_userIdIn = block.getInletHandle("user_id");
		m_gyroXOut = block.getOutletHandle("gyro_x");
		m_gyroYOut = block.getOutletHandle("gyro_y");

		m_gyroXOut.getWriteableRef<int>() = 0;
		m_gyroYOut.getWriteableRef<int>() = 0;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void EpocGyroBlock::update()
{
	try
	{
		if(m_EpocDeviceManager)
		{
			Vec3 gyro( m_EpocDeviceManager->getGyro(m_userIdIn.getReadableRef<unsigned int>() ) );

			if(m_EpocDeviceManager->getNumberOfConnectedDevices()<=0 || gyro[2])	// if there is no device connected there is nothing todo so return
			{
				std::cout << "EpocGyroBlock: EmoEngine is unable to acquire data for processing." << std::endl;
				
				m_gyroXOut.discard();
				m_gyroYOut.discard();

				Sleep(1000);
				return;
			}
			
			m_gyroXOut.getWriteableRef<int>() = (int) gyro[0];
			m_gyroYOut.getWriteableRef<int>() = (int) gyro[1];
		}
		else
		{
			m_gyroXOut.discard();
			m_gyroYOut.discard();
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void EpocGyroBlock::shutdown()
{
	std::cout << "shutdown block" << std::endl;
}

