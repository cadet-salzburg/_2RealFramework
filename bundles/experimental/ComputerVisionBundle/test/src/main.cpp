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

template< typename TData >
TData * makeCirclePattern( const unsigned int width, const unsigned int height, const unsigned char channels, const unsigned char sz, TData *black, TData *white )
{
	float radius = 0.5f*sz;

	TData *data = new TData[ width * height * channels ];
	TData *p = data;
	for ( unsigned int i=0; i<height; ++i )
	{
		unsigned int divI = i/sz;
		unsigned int modI = i%sz;
		for ( unsigned int j=0; j<width; ++j )
		{
			unsigned int divJ = j/sz;
			unsigned int modJ = j%sz;
			if ( ( divJ%2 == 0 ) ^ ( divI%2 == 0 ) )
			{
				float x = modI - radius;
				float y = modJ - radius;
				float dist = sqrt( x*x + y*y );

				if ( dist < radius )
				{
					for ( unsigned int k=0; k<channels; ++k )
					{
						*p = black[ k ];
						++p;
					}
				}
				else
				{
					for ( unsigned int k=0; k<channels; ++k )
					{
						*p = white[ k ];
						++p;
					}
				}
			}
			else
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = white[ k ];
					++p;
				}
			}
		}
	}
	return data;
}

template< typename TData >
TData * makeCheckerPattern( const unsigned int width, const unsigned int height, const unsigned char channels, const unsigned char sz, TData *black, TData *white )
{
	TData *data = new TData[ width * height * channels ];
	TData *p = data;
	for ( unsigned int i=0; i<height; ++i )
	{
		unsigned int divI = i/sz;
		for ( unsigned int j=0; j<width; ++j )
		{
			unsigned int divJ = j/sz;
			if ( ( divJ%2 == 0 ) ^ ( divI%2 == 0 ) )
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = black[ k ];
					++p;
				}
			}
			else
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = white[ k ];
					++p;
				}
			}
		}
	}
	return data;
}

class OutletReceiver
{
public:
	OutletReceiver( OutletHandle out ) : mOutlet( out )
	{
		mOutlet.registerToNewData( *this, &OutletReceiver::receiveData );
	}
	~OutletReceiver()
	{
		mOutlet.unregisterFromNewData( *this, &OutletReceiver::receiveData );
	}
	void receiveData( std::shared_ptr< const CustomType > data )
	{
		// must never be null
		assert( data.get() );
		Poco::ScopedLock< Poco::FastMutex > lock( mMutex );
		mData = data;
	}
	std::shared_ptr< const CustomType > getData() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mMutex );
		return mData;
	}
private:
	mutable Poco::FastMutex				mMutex;
	std::shared_ptr< const CustomType >	mData;
	_2Real::app::OutletHandle			mOutlet;
};

struct Texture
{
	Texture() : handle( 0 ), width( 0 ), height( 0 ) {}

	GLuint handle;
	unsigned int width, height;
};

struct BlockInstance
{
	typedef std::pair< OutletHandle, OutletReceiver * > OutletHandler;

	BlockInstance() : wasClosed( false ), window( nullptr ) {}

	~BlockInstance()
	{
		for ( std::vector< BlockInstance::OutletHandler >::iterator it = outlets.begin(); it != outlets.end(); ++it )
			delete it->second;

		delete window;
	}

	BlockHandle block;
	std::vector< InletHandle >		inlets;
	std::vector< OutletHandler >	outlets;
	std::vector< ParameterHandle >	parameters;
	sf::Window						*window;
	Texture							texture;
	bool							wasClosed;
};

void imageToTexture( Texture &tex, std::shared_ptr< const Image > img )
{
	if ( 0 == tex.handle )
	{
		// uninitialized: create texture handle & set filters
		glGenTextures( 1, &tex.handle );
		glBindTexture( GL_TEXTURE_2D, tex.handle );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}
	else
		glBindTexture( GL_TEXTURE_2D, tex.handle );

	GLenum uploadFormat = GL_RGBA;
	GLenum uploadType = GL_UNSIGNED_BYTE;

	const unsigned int w = img->getWidth();
	const unsigned int h = img->getHeight();
	const Image::ChannelOrder channels = img->getChannelOrder();
	const Image::Datatype type = img->getDatatype();

	// note that i ignore the bgr vs rgb thing for testing
	switch ( channels.getNumberOfChannels() )
	{
	case 4:
		uploadFormat = GL_RGBA;
		break;
	case 3:
		uploadFormat = GL_RGB;
		break;
	default:
		// for some reason GL_RG is not recognized ( probably need glew )
		uploadFormat = GL_RED;
		break;
	}

	switch ( type.getCode() )
	{
	case Image::Datatype::UINT8:
		uploadType = GL_UNSIGNED_BYTE;
		break;
	case Image::Datatype::UINT16:
		uploadType = GL_UNSIGNED_SHORT;
		break;
	case Image::Datatype::UINT32:
		uploadType = GL_UNSIGNED_INT;
		break;
	case Image::Datatype::INT8:
		uploadType = GL_BYTE;
		break;
	case Image::Datatype::INT16:
		uploadType = GL_SHORT;
		break;
	case Image::Datatype::INT32:
		uploadType = GL_INT;
		break;
	case Image::Datatype::FLOAT32:
		uploadType = GL_FLOAT;
		break;
	case Image::Datatype::FLOAT64:
		// in this case,assume float - it won't work properly, but then, nothing will
		uploadType = GL_UNSIGNED_BYTE;
		break;
	default:
		uploadType = GL_UNSIGNED_BYTE;
		break;
	}

	if ( w != tex.width || h != tex.height )
	{
		tex.width = w;
		tex.height = h;
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, uploadFormat, uploadType, ( const GLvoid * )img->getPixels() );
	}
	else
	{
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, w, h, uploadFormat, uploadType, ( const GLvoid * )img->getPixels() );
	}

	glBindTexture( GL_TEXTURE_2D, 0 );
}

void printBundleInfo( app::BundleHandle const& h )
{
	BundleInfo info = h.getBundleInfo();
	BundleInfo::BlockInfos blocks = info.exportedBlocks;
	for ( BundleInfo::BlockInfoIterator it = blocks.begin(); it != blocks.end(); ++it )
	{
		std::cout << "-b\t" << it->name << std::endl;
		BlockInfo::InletInfos inlets = it->inlets;
		for ( BlockInfo::InletInfoIterator iIt = inlets.begin(); iIt != inlets.end(); ++iIt )
		{
			std::cout << "-i\t" << iIt->name;
			std::cout << "\tt: " << iIt->customName;
			std::cout << ( iIt->isMultiInlet ? "\t+m " : "\t-m " ) << std::endl;
		}
		BlockInfo::OutletInfos outlets = it->outlets;
		for ( BlockInfo::OutletInfoIterator oIt = outlets.begin(); oIt != outlets.end(); ++oIt )
		{
			std::cout << "-o\t" << oIt->name;
			std::cout << "\tt: " << oIt->customName << std::endl;
		}
		BlockInfo::ParameterInfos params = it->parameters;
		for ( BlockInfo::ParameterInfoIterator pIt = params.begin(); pIt != params.end(); ++pIt )
		{
			std::cout << "-o\t" << pIt->name;
			std::cout << "\tt: " << pIt->customName << std::endl;
		}
	}
}

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	testEngine.setBaseDirectory( "." );

	unsigned int numInstances = 1;
	unsigned int numBlocks = 0;
	std::vector< std::vector< BlockInstance > > blocks;

	sf::Image img;
	if ( !img.loadFromFile( "test.jpg" ) )
		std::cout << "could not read image!" << std::endl;
	else
		std::cout << img.getSize().x << " " << img.getSize().y << std::endl;

	try
	{
		BundleHandle bundle = testEngine.loadBundle( "ComputerVisionBundle" );
		printBundleInfo( bundle );
		numBlocks = bundle.getBundleInfo().exportedBlocks.size();
		blocks = std::vector< std::vector< BlockInstance > >( numBlocks, std::vector< BlockInstance >( numInstances ) );

		for ( unsigned int b=0; b<numBlocks; ++b )
		{
			for ( unsigned int i=0; i<numInstances; ++i )
			{
				BlockInstance &instance = blocks[ b ][ i ];
				std::string instanceName =  bundle.getBundleInfo().exportedBlocks[ b ].name;

				instance.window = new sf::Window( sf::VideoMode( 512, 512, 32 ), instanceName, sf::Style::Close, sf::ContextSettings( 0, 0, 0, 2, 1 ) );

				instance.block = bundle.createBlockInstance( instanceName );
				std::vector< InletHandle > const& inlets = instance.block.getAllInletHandles();
				for ( unsigned int i=0; i<inlets.size(); ++i )
					instance.inlets.push_back( inlets[ i ] );
				std::vector< OutletHandle > const& outlets = instance.block.getAllOutletHandles();
				for ( unsigned int o=0; o<outlets.size(); ++o )
					instance.outlets.push_back( std::make_pair( outlets[ o ], new OutletReceiver( outlets[ o ] ) ) );
				std::vector< ParameterHandle > const& parameters = instance.block.getAllParameterHandles();
				for ( unsigned int p=0; p<parameters.size(); ++p )
					instance.parameters.push_back( parameters[ p ] );

				instance.block.setup();
				instance.block.start();

				std::shared_ptr< CustomType > inData = instance.inlets[ 0 ].makeData();
				std::shared_ptr< Image > inImage = Image::asImage( inData );
				Image &in = *inImage.get();
				in.setImagedata( img.getPixelsPtr(), img.getSize().x, img.getSize().y, Image::ChannelOrder::RGBA, Image::Datatype::UINT8 );
				instance.inlets[ 0 ].receiveData( inData );
			}
		}
	}
	catch ( Exception &e )
	{
		std::cout << e.what() << " " << e.message() << std::endl;
	}

	while( 1 )
	{
		bool allWindowsClosed = true;

		for ( unsigned int b=0; b<numBlocks; ++b )
		{
			for ( unsigned int i=0; i<numInstances; ++i )
			{
				BlockInstance &instance = blocks[ b ][ i ];

				allWindowsClosed &= instance.wasClosed;
				if ( instance.wasClosed ) continue;

				sf::Window *win = instance.window;
				win->setActive( true );

				bool exit = false;
				sf::Event ev;
				while ( win->pollEvent( ev ) )
				{
					if ( sf::Event::Closed == ev.type )
					{
						exit = true;
						break;
					}
				}
				if ( exit )
				{
					instance.wasClosed = true;
					win->close();
				}

				std::shared_ptr< const CustomType > d = instance.outlets[ 0 ].second->getData();
				if ( d.get() )
				{
					std::shared_ptr< const Image > img = Image::asImage( d );
					imageToTexture( instance.texture, img );
				}

				glViewport( 0, 0, 600, 600 );
				glBindTexture( GL_TEXTURE_2D, instance.texture.handle );
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

				win->display();
				win->setActive( false );
			}
		}

		if ( allWindowsClosed )
			break;
	}

	blocks.clear();

	return 0;
}