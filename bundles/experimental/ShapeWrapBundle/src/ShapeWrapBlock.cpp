#include "ShapeWrapBlock.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;

ShapeWrapBlock::ShapeWrapBlock( ContextBlock & context ) :
	Block()
{
}

ShapeWrapBlock::~ShapeWrapBlock()
{
}

void ShapeWrapBlock::setup( BlockHandle &context )
{
	try
	{
		// Get the handles to all needed In and Outlets
		m_UDPPort = context.getInletHandle( "UDPport" );
		m_Skeleton = context.getOutletHandle( "Write" );
	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void ShapeWrapBlock::shutdown()
{
}

void ShapeWrapBlock::discardAllOutlets()
{
	m_Skeleton.discard();
}

void ShapeWrapBlock::update()
{
	try
	{
		// TODO

	}
	catch ( Exception& e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}
