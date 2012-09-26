#include "MovingHeadTrackingBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

MovingHeadTrackingBlock::MovingHeadTrackingBlock( ContextBlock & context ) 
: Block(), m_CurrentPosX( 0 ), m_CurrentPosY( 0 )
{

}

MovingHeadTrackingBlock::~MovingHeadTrackingBlock()
{
}

void MovingHeadTrackingBlock::setup( BlockHandle &context )
{
	try
	{
		// Get the handles to all needed In and Outlets
		m_CenterOfMassInlet = context.getInletHandle( "CentorOfMass" );
		m_UserIDInlet = context.getInletHandle( "UserID" );
		m_MotorIDXInlet = context.getInletHandle( "MotorIDX");
		m_MotorIDYInlet = context.getInletHandle( "MotorIDY");
		m_MotorIDOutlet = context.getOutletHandle( "MotorID" );
		m_CommandOutlet = context.getOutletHandle( "Command" );
		m_ValueOutlet = context.getOutletHandle( "Value" );
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void MovingHeadTrackingBlock::shutdown()
{
}

void MovingHeadTrackingBlock::discardAllOutlets()
{
	m_MotorIDOutlet.discard();
	m_CommandOutlet.discard();
	m_ValueOutlet.discard();
}

void MovingHeadTrackingBlock::update()
{
	try
	{
		// there is only something happening when a new value is provided!
		if (m_CenterOfMassInlet.hasChanged())
		{
			int newXPos = m_CurrentPosX;
			int newYPos = m_CurrentPosY;

			int id = m_UserIDInlet.getReadableRef<int>();
			_2Real::Point com;
			std::vector<_2Real::Point> coms = m_CenterOfMassInlet.getReadableRef<std::vector<_2Real::Point>>();
			std::vector<_2Real::Point>::const_iterator it = coms.begin();
			while (it != coms.end())
			{
				com = *it;
				if (id == _2Real::Point::INVALID_ID || id == com.getId())
				{
					break;
				}
				it++;
			}
			if (it == coms.end())
			{
				discardAllOutlets();
				return;
			}
			
			int x = com.x();
			int y = com.y();
			int z = com.z();

			Vec3 com2Dy = Vec3(x-320, 0, z);
			Vec3 com2Dx = Vec3(0, y-240, z);
			
			com2Dy.normalize();
			com2Dy.normalize();

			float alpha = acos(com2Dy.dot(Vec3(0,0,1)));
			float beta = acos(com2Dx.dot(Vec3(0,0,1)));
		
			if(x >= 320.f)	alpha = -alpha;
			if(y < 240.f) beta = -beta;

			if(alpha > 0.005)
			{
				newXPos = newXPos+alpha*40;
			}
			else if(alpha < -0.005)
			{
				newXPos = newXPos+alpha*40;
			}
			if(beta > 0.005)
			{
				newYPos = newYPos-beta*20;
			}
				else if (beta < -0.005)
			{
				newYPos = newYPos-beta*20;
			}

			int deltaX = abs(newXPos - m_CurrentPosX);
			int deltaY = abs(newYPos - m_CurrentPosY);

			// as we can update only one motor per update cyclus we choose the more significant here
			if (deltaX > 0 || deltaX > deltaY)
			{
				m_CurrentPosX = newXPos;
				m_ValueOutlet.getWriteableRef<unsigned int>() = m_CurrentPosX;
				m_MotorIDOutlet.getWriteableRef<unsigned int>() = m_MotorIDXInlet.getReadableRef<unsigned int>();
				m_CommandOutlet.getWriteableRef<unsigned char>() = 'P';
			}
			else if (deltaY > 0)
			{
				m_CurrentPosY = newYPos;
				m_ValueOutlet.getWriteableRef<unsigned int>() = m_CurrentPosY;
				m_MotorIDOutlet.getWriteableRef<unsigned int>() = m_MotorIDYInlet.getReadableRef<unsigned int>();
				m_CommandOutlet.getWriteableRef<unsigned char>() = 'P';
			}
		}
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
