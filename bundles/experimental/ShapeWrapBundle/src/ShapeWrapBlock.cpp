#include "ShapeWrapBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

ShapeWrapBlock::ShapeWrapBlock( ContextBlock & context ) :
	Block()
{
	m_ShapeWrapDeviceManager = static_cast<ShapeWrapDeviceManager*>( &context );
	m_ShapeWrapDeviceManager->registerShapeWrapBlock();
}

ShapeWrapBlock::~ShapeWrapBlock()
{
}

void ShapeWrapBlock::setup( BlockHandle &context )
{
	try
	{
		// Get the handles to all needed In and Outlets
		m_ShapeWrapPortInlet = context.getInletHandle( "ShapeWrapPort" );
		m_iBaudRateInlet = context.getInletHandle( "BaudRate" );
		m_ReadIntel = context.getInletHandle( "Read" );
		m_WriteOutlet = context.getOutletHandle( "Write" );
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void ShapeWrapBlock::shutdown()
{
	m_ShapeWrapDeviceManager->unregisterShapeWrapBlock();
}

void ShapeWrapBlock::discardAllOutlets()
{
	m_WriteOutlet.discard();
}

void ShapeWrapBlock::update()
{
	try
	{
		// Get the current ShapeWrap port that is set in the Inlet
		std::string port = m_ShapeWrapPortInlet.getReadableRef<std::string>();
		unsigned int bauRrate = m_iBaudRateInlet.getReadableRef<unsigned int>();
/*
		// Retreive the ShapeWrap message and store it into a std::vector<unsigned char>
		vector<unsigned char> ShapeWrapMessage = m_ShapeWrapDeviceManager->getShapeWrapMessage( m_iShapeWrapCurrentPort );
		m_ShapeWrapMessage0Outlet.getWriteableRef<unsigned char>() = ShapeWrapMessage[0];
*/

	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
