#include "_2RealApplication.h"
#include "_2RealDatatypes.h"
#include <iostream>
#include <list>
#include <vector>
#ifdef _DEBUG
	#include "vld.h"
#endif

#include <SFML\OpenGL.hpp>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

using namespace _2Real;
using namespace _2Real::app;

Poco::FastMutex mutex;

std::shared_ptr< const Image > outty( nullptr );

void receivedData( void *, std::shared_ptr< const CustomType > data )
{
	// must never be null
	assert( data.get() );

	mutex.lock();
	outty = Image::asImage( data );
	mutex.unlock();

	//std::cout << "received: " << outty->getWidth() << " " << outty->getHeight() <<  std::endl;
}

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	testEngine.setBaseDirectory( "." );

	InletHandle i0A, i0B;
	OutletHandle o0;

	unsigned int w = 600;
	unsigned int h = 600;
	sf::Window window( sf::VideoMode( w, h, 32 ), "geometry shader stuff", sf::Style::Default, sf::ContextSettings( 0, 0, 0, 2, 1 ) );

	GLuint tex;
	glGenTextures( 1, &tex );
	glBindTexture( GL_TEXTURE_2D, tex );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, 480, 480, 0, GL_RGBA, GL_FLOAT, nullptr );
	glBindTexture( GL_TEXTURE_2D, 0 );

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

		//for ( unsigned int i=0; i<2; ++i )
		//{
		//	BlockHandle b = testBundle.createBlockInstance( "OcvGaussianBlurBlock" );
		//	b.setup();
		//	b.start();
		//}

		testBlock0.setup();
		testBlock0.start();

		o0.registerToNewData( &receivedData, nullptr );
	}
	catch ( Exception &e )
	{
		std::cout << e.what() << " " << e.message() << std::endl;
	}

	while( 1 )
	{
		bool exit = false;
		sf::Event ev;
		while ( window.pollEvent( ev ) )
		{
			if ( sf::Event::Closed == ev.type )
			{
				exit = true;
				break;
			}
			else if ( sf::Event::Resized == ev.type )
			{
				w = ev.size.width;
				h = ev.size.height;
			}
		}
		if ( exit ) break;

		std::shared_ptr< const CustomType > i0Adata = i0A.getCurrentData();
		std::shared_ptr< const CustomType > i0Bdata = i0B.getCurrentData();

		// getCurrentData will return nullptrs until the first update happens, so just keep looping
		if ( i0Adata.get() == nullptr || i0Bdata.get() == nullptr )
			continue;

		std::shared_ptr< const Image > img0A = Image::asImage( i0Adata );
		std::shared_ptr< const Image > img0B = Image::asImage( i0Bdata );
		std::shared_ptr< const Image > imgOut = Image::asImage( o0.getCurrentData() );
		//std::cout << "img A: " << img0A->getWidth() << " " << img0A->getHeight() << std::endl;
		//std::cout << "img B: " << img0B->getWidth() << " " << img0B->getHeight() << std::endl;
		//std::cout << "img O: " << imgOut->getWidth() << " " << imgOut->getHeight() << std::endl;

		//float num = 32 * 24 * 4;
		//float *data = new float[ 32 * 24 * 4 ];
		//for ( unsigned int i=0; i<num; ++i )
		//	data[ i ] = 0.5f;
		//std::shared_ptr< CustomType > t = i0A.makeData();
		//t->set( Image::FIELD_WIDTH, unsigned int( 32 ) );
		//t->set( Image::FIELD_HEIGHT, unsigned int( 24 ) );
		//t->set( Image::FIELD_DATA, std::vector< unsigned char >( ( unsigned char * )data, ( unsigned char * )data+32*24*4*4 ) );
		//t->set< int >( Image::FIELD_CHANNELS, Image::ChannelOrder::RGB );
		//t->set< int >( Image::FIELD_DATATYPE, Image::Datatype::FLOAT32 );
		//i0A.receiveData( t );
		//delete [] data;

		glViewport( 0, 0, w, h );

		glBindTexture( GL_TEXTURE_2D, tex );

		mutex.lock();
		if ( outty.get() )
			glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 480, 480, GL_RGBA, GL_FLOAT, outty->getPixels() );
		mutex.unlock();

		glEnable( GL_TEXTURE_2D );

		glColor3f( 1.f, 1.f, 1.f );
		glBegin( GL_QUADS );
		glTexCoord2f( 0.f, 1.f );
		glVertex2f( -1.f,  1.f );
		glTexCoord2f( 0.f, 0.f );
		glVertex2f( -1.f, -1.f );
		glTexCoord2f( 1.f, 0.f );
		glVertex2f(  1.f, -1.f );
		glTexCoord2f( 1.f, 1.f );
		glVertex2f(  1.f,  1.f );
		glEnd();

		glDisable( GL_TEXTURE_2D );

		glBindTexture( GL_TEXTURE_2D, 0 );

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

		window.display();
	}

	return 0;
}