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

void receivedData( void *, std::shared_ptr< const CustomType > data )
{
	if ( !data.get() )
		std::cout << "received null" << std::endl;

	std::shared_ptr< const Image > img = Image::asImage( data );
	std::cout << "received: " << img->getWidth() << " " << img->getHeight() <<  std::endl;
}

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	testEngine.setBaseDirectory( "." );

	InletHandle i0A, i0B;
	OutletHandle o0;

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

		i0A = testBlock0.getInletHandle( "InImageA" );
		i0B = testBlock0.getInletHandle( "InImageB" );
		o0 = testBlock0.getOutletHandle( "OutImage" );

		testBlock0.setup();
		testBlock0.start();

		o0.registerToNewData( &receivedData, nullptr );
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;
	}

	int cnt = 0;

	Sleep( 1000 );

	while( 1 )
	{
		std::shared_ptr< const CustomType > i0Adata = i0A.getCurrentData();
		std::shared_ptr< const CustomType > i0Bdata = i0B.getCurrentData();

		// getCurrentData() will return nullptrs until the first update happens
		if ( i0Adata.get() == nullptr || i0Bdata.get() == nullptr )
			continue;

		std::shared_ptr< const Image > img0A = Image::asImage( i0Adata );
		std::shared_ptr< const Image > img0B = Image::asImage( i0Bdata );
		std::cout << "img A: " << img0A->getWidth() << " " << img0A->getHeight() << std::endl;
		std::cout << "img B: " << img0B->getWidth() << " " << img0B->getHeight() << std::endl;

		float num = 32 * 24 * 3;
		float *data = new float[ 32 * 24 * 3 ];
		for ( unsigned int i=0; i<num; ++i )
			data[ i ] = 0.5f;
		std::shared_ptr< CustomType > t = i0A.makeData();
		t->set( Image::FIELD_WIDTH, unsigned int( 32 ) );
		t->set( Image::FIELD_HEIGHT, unsigned int( 24 ) );
		t->set( Image::FIELD_DATA, std::vector< unsigned char >( ( unsigned char * )data, ( unsigned char * )data+32*24*3*4 ) );
		t->set< int >( Image::FIELD_CHANNELS, Image::ChannelOrder::RGB );
		t->set< int >( Image::FIELD_DATATYPE, Image::Datatype::FLOAT32 );
		i0A.receiveData( t );
		std::cout << "set: " << *( t->get< unsigned int >( Image::FIELD_WIDTH ).get() ) << " " << *( t->get< unsigned int >( Image::FIELD_HEIGHT ).get() ) << std::endl;
		delete [] data;

		//app::TypeMetainfo info = i0A.getType();
		//_2Real::Fields fields; info.getFieldInfo( fields );
		//for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
		//{
		//	std::cout << ( *it )->getName() << " " << ( *it )->getTypename() << std::endl;
		//}
		//for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
		//{
		//	delete *it;
		//}

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