#include "_2RealApplication.h"
#include "_2RealDatatypes.h"
#include "helpers/_2RealPocoIncludes.h"
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

void printout( std::ostream &out, DataFields const& fields, const unsigned int offset )
{
	for ( DataFields::const_iterator it = fields.begin(); it != fields.end(); ++it )
	{
		DataFieldRef f = *it;
		for ( unsigned int i=0; i<offset; ++ i ) out << "\t";
		std::string name = f->getName();
		if ( name.length() >= 16 )
			out << f->getName() << "\t";
		else if ( name.length() >= 8 )
			out << f->getName() << "\t\t";
		else
			out << f->getName() << "\t\t\t";
		std::cout << f->getTypename().first << "::" << f->getTypename().second << std::endl;
		if ( !f->getSubFields().empty() )
		{
			printout( out, f->getSubFields(), offset+1 );
		}
	}
}

void printBundleInfo( app::BundleHandle const& h )
{
	BundleMetainfo info = h.getBundleMetainfo();

	std::vector< BlockMetainfo > blocks;
	info.getExportedBlocks( blocks );

	for ( std::vector< BlockMetainfo >::const_iterator it = blocks.begin(); it != blocks.end(); ++it )
	{
		BlockMetainfo block = *it;

		std::vector< InputMetainfo > inlets;		block.getInletMetainfo( inlets );
		std::vector< OutputMetainfo > outlets;		block.getOutletMetainfo( outlets );
		std::vector< InputMetainfo > parameters;	block.getParameterMetainfo( parameters );

		std::cout << "---------------------" << std::endl;
		std::cout << "-b\t" << block.getName() << " " << ( block.isContext() ? "regular" : "context" ) << std::endl;
		std::cout << "\t" << block.getDescription() << ", needs context: " << " unknown" << std::endl;
		std::cout << "---------------------" << std::endl;
		std::cout << "inlets:" << std::endl;
		for ( std::vector< InputMetainfo >::const_iterator iIt = inlets.begin(); iIt != inlets.end(); ++iIt )
		{
			std::cout << "-i\t" << iIt->getName() << " " << iIt->getTypeName().first << "::" << iIt->getTypeName().second << std::endl;
			std::cout << "\t" << std::boolalpha << " -b " << iIt->isBuffered() << " -l " << iIt->canLink() << " -m " << iIt->canExpand() << std::endl;
		}
		std::cout << "parameters:" << std::endl;
		for ( std::vector< InputMetainfo >::const_iterator pIt = parameters.begin(); pIt != parameters.end(); ++pIt )
		{
			std::cout << "-i\t" << pIt->getName() << " " << pIt->getTypeName().first << "::" << pIt->getTypeName().second << std::endl;
			std::cout << "\t" << std::boolalpha << " -b " << pIt->isBuffered() << " -l " << pIt->canLink() << " -m " << pIt->canExpand() << std::endl;
		}
		std::cout << "outlets:" << std::endl;
		for ( std::vector< OutputMetainfo >::const_iterator oIt = outlets.begin(); oIt != outlets.end(); ++oIt )
		{
			std::cout << "-o\t" << oIt->getName() << " " << oIt->getTypeName().first << "::" << oIt->getTypeName().second << std::endl;
			std::cout << "\t" << std::boolalpha << " -l " << oIt->canLink() << " -m " << oIt->canExpand() << std::endl;
		}
	}
}

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	//testEngine.setBaseDirectory( "." );

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

		BundleMetainfo bundleMetainfo = bundle.getBundleMetainfo();
		std::vector< BlockMetainfo > exportedBlocks;
		bundleMetainfo.getExportedBlocks( exportedBlocks );
		numBlocks = exportedBlocks.size();
		blocks = std::vector< std::vector< BlockInstance > >( numBlocks, std::vector< BlockInstance >( numInstances ) );

		for ( unsigned int b=0; b<numBlocks; ++b )
		{
			for ( unsigned int i=0; i<numInstances; ++i )
			{
				BlockInstance &instance = blocks[ b ][ i ];
				std::string instanceName = exportedBlocks[ b ].getName();

				instance.window = new sf::Window( sf::VideoMode( 512, 512, 32 ), instanceName, sf::Style::Close, sf::ContextSettings( 0, 0, 0, 2, 1 ) );

				instance.block = bundle.createFunctionBlockInstance( instanceName );
				instance.block.getAllInletHandles( instance.inlets );
				instance.block.getAllParameterHandles( instance.parameters );
				std::vector< OutletHandle > handles;
				instance.block.getAllOutletHandles( handles );
				for ( unsigned int o=0; o<handles.size(); ++o )
					instance.outlets.push_back( std::make_pair( handles[ o ], new OutletReceiver( handles[ o ] ) ) );

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