#include "ServiceImpl.h"

#include "_2RealBundle.h"

using namespace _2Real::bundle;
using namespace _2Real;

using std::string;

void getBundleMetainfo( BundleMetainfo &info )
{
	try
	{
		info.setCategory( "state testing" );
		info.setVersion( 0, 0, 0 );

		FunctionBlockMetainfo testBlock = info.exportFunctionBlock< Test, WithoutContext >( "TestBlock" );
		testBlock.setDescription( "state testing" );
		testBlock.addInlet< unsigned int >( "i0", 0 );
		testBlock.addParameter< unsigned int >( "p0", 0 );
		testBlock.addOutlet< unsigned int >( "o0" );
	}
	catch ( Exception &e )
	{
		e.rethrow();
	}
}