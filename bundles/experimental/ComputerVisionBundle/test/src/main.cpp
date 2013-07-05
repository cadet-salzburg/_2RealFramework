#include "_2RealApplication.h"
#include "_2RealDatatypes.h"

#include <windows.h>
#include <iostream>
#include <list>
#include <deque>
#include <vector>
#ifdef _DEBUG
	#include "vld.h"
#endif

using namespace std;
using namespace _2Real;
using namespace _2Real::app;

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	testEngine.setBaseDirectory( "." );

	InletHandle i0, i1, i2;
	OutletHandle o0, o1, o2;

	try
	{
		BundleHandle testBundle = testEngine.loadBundle( "ComputerVisionBundle" );

		BundleInfo info = testBundle.getBundleInfo();
		BundleInfo::BlockInfos blocks = info.exportedBlocks;
		for ( BundleInfo::BlockInfoIterator it = blocks.begin(); it != blocks.end(); ++it )
		{
			std::cout << "-b\t" << it->name << std::endl;
			BlockInfo::InletInfos inlets = it->inlets;
			for ( BlockInfo::InletInfoIterator iIt = inlets.begin(); iIt != inlets.end(); ++iIt )
			{
				std::cout << "-i\t" << iIt->name << ( iIt->isMultiInlet ? "\t+m " : "\t-m " ) << std::endl;
			}
			BlockInfo::OutletInfos outlets = it->outlets;
			for ( BlockInfo::OutletInfoIterator oIt = outlets.begin(); oIt != outlets.end(); ++oIt )
			{
				std::cout << "-o\t" << oIt->name << std::endl;
			}
		}

		BlockHandle testBlock0 = testBundle.createBlockInstance( "OcvGaussianBlurBlock" );
		BlockHandle testBlock1 = testBundle.createBlockInstance( "OcvGaussianBlurBlock" );

		i0 = testBlock0.getInletHandle( "InImageA" );
		//o0 = testBlock0.getOutletHandle( "customOutlet0" );

		i1 = testBlock1.getInletHandle( "InImageA" );
		//o1 = testBlock1.getOutletHandle( "customOutlet0" );

		//o0.link( i10 );
		//o1.link( i20 );

		testBlock0.setup();
		testBlock0.start();

		testBlock1.setup();
		testBlock1.start();
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;
	}

	int cnt = 0;

	Sleep( 1000 );

	while( 1 )
	{
		//std::shared_ptr< const CustomType > o0data = o0.getCurrentData();
		//std::cout << "o0: " << o0data->get< int >( "test int" ) << std::endl;

		//Image &oImg = o0data->get< Image >( "test image" );
		//oImg.set( data, 2, 2 );

		// this receives o0s outputs. it's extremely unlikely that it would ever be higher than o0
		std::shared_ptr< const CustomType > i1data = i1.getCurrentData();
		///Image const& i1Img = i1data->get< Image >( "default" );

		std::cout << i1data->get< unsigned int >( "width" ) << std::endl;
		//std::cout << i1Img.getHeight() << std::endl;

		std::shared_ptr< CustomType > t = i0.makeData();
		//t->set< int >( "test int", ++cnt );
		//Image img;
		//std::vector< unsigned char > data;
		//data.push_back( 10 ); data.push_back( 20 ); data.push_back( 10 ); data.push_back( 20 );
		//img.set( data, cnt, cnt );
		//t->set< Image >( "test image", img );
		//i00.receiveData( t );

		app::TypeMetainfo info = i0.getType();
		_2Real::Fields fields; info.getFieldInfo( fields );
		for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
		{
			std::cout << ( *it )->getName() << " " << ( *it )->getTypename() << std::endl;
		}
		for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
		{
			delete *it;
		}

		string line;
		char lineEnd = '\n';
		getline( cin, line, lineEnd );
		if ( line == "q" )
		{
			break;
		}
	}

	return 0;
}