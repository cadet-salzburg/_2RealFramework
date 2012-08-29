
#include "TUIOBundleTestWindow.h"
#include "_2RealBundle.h"

using namespace _2Real::bundle;
using			_2Real::Exception;

void getBundleMetainfo(BundleMetainfo& info)
{
    try
    {
        info.setName("TUIOBundleTest");
		info.setDescription( "This bundle serves as a generator of tracks to verify functionality of the TUIOBundle." );
		info.setAuthor( "Otto Naderer" );
		info.setCategory( "Experimental" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		BlockMetainfo tuioBlockInfo = info.exportBlock<TUIOBundleTestWindow, WithoutContext>( "TUIOBundleTest" );
		tuioBlockInfo.addOutlet<std::vector<_2Real::Point>>("TracklistOut");
		tuioBlockInfo.addInlet<std::string>("Host", "127.0.0.1");
		tuioBlockInfo.addInlet<unsigned short>("Port", 3333);

    }
    catch ( Exception &e )
	{
		std::cout << e.message() << std::endl;
		e.rethrow();
	}
}
