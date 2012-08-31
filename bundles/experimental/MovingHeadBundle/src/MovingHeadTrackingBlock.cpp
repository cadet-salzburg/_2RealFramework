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

		m_ValueXOutlet = context.getOutletHandle( "ValueX" );
		m_ValueYOutlet = context.getOutletHandle( "ValueY" );
	
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
	m_ValueXOutlet.discard();
	m_ValueYOutlet.discard();
}

void MovingHeadTrackingBlock::update()
{
	try
	{
		// there is only something happening when a new value is provided!
		if (m_CenterOfMassInlet.hasChanged())
		{
			_2Real::Point com = m_CenterOfMassInlet.getReadableRef<_2Real::Point>();


			if(m_UserIDInlet.getReadableRef<unsigned int>() != com.getId())
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
				m_CurrentPosX = m_CurrentPosX+alpha*40;
			}
			else if(alpha < -0.005)
			{
				m_CurrentPosX = m_CurrentPosX+alpha*40;
			}
			if(beta > 0.005)
			{
				m_CurrentPosY = m_CurrentPosY-beta*20;
			}
				else if (beta < -0.005)
			{
				m_CurrentPosY = m_CurrentPosY-beta*20;
			}

			m_ValueXOutlet.getWriteableRef<unsigned int>() = m_CurrentPosX;
			m_ValueYOutlet.getWriteableRef<unsigned int>() = m_CurrentPosY;
		}
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
