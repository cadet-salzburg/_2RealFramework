
#include "TUIOBlock.h"
#include "_2RealBundle.h"

using namespace _2Real::bundle;
using			_2Real::Exception;

void getBundleMetainfo(BundleMetainfo& info)
{
	try
	{
		info.setName("TUIOBundle");
		info.setDescription( "This bundle takes a vector of Points representing some track ID plus the x and y value as coordinates ranging from 0.f to 1.f (according to the TUIO protocol specification." );
		info.setAuthor( "Otto Naderer" );
		info.setCategory( "Experimental" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		BlockMetainfo tuioBlockInfo = info.exportBlock<TUIOBlock, WithoutContext>( "TUIOBlock" );
		tuioBlockInfo.addInlet<std::vector<_2Real::Point>>("Tracklist", std::vector<_2Real::Point>());
		tuioBlockInfo.addInlet<std::string>("Host", "127.0.0.1");
		tuioBlockInfo.addInlet<unsigned short>("Port", 3333);

	}
	catch ( Exception &e )
	{
		std::cout << e.message() << std::endl;
		e.rethrow();
	}
}
