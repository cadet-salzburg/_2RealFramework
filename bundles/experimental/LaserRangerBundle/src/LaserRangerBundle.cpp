
#include "LaserRangerHokuyo.h"
#include "_2RealBundle.h"

using namespace _2Real::bundle;
using			_2Real::Exception;

void getBundleMetainfo(BundleMetainfo& info)
{
	try
	{
		info.setName("LaserRangerBundle");
		info.setDescription( "This bundle outputs the range image of implemented laser ranger devices" );
		info.setAuthor( "Otto Naderer" );
		info.setCategory( "Experimental" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		BlockMetainfo tuioBlockInfo = info.exportBlock<LaserRangerHokuyo, WithoutContext>( "LaserRangerHokuyoBlock" );
		tuioBlockInfo.addInlet<std::string>("Devicepath", "");
		tuioBlockInfo.addInlet<int>("Baudrate", 9600);
		tuioBlockInfo.addOutlet<std::vector<long>>("Rangeimage");

	}
	catch ( Exception &e )
	{
		std::cout << e.message() << std::endl;
		e.rethrow();
	}
}
