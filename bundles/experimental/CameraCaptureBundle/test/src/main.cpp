#include "_2RealApplication.h"
#include "_2RealDatatypes.h"
#include "helpers/_2RealPocoIncludes.h"
#include <iostream>
#include <list>
#include <vector>
//#ifdef _DEBUG
//	#include "vld.h"
//#endif

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
		std::cout << "-b\t" << block.getName() << " " << ( block.isContext() ? "context" : "regular" ) << std::endl;
		std::cout << "\t" << block.getDescription() << ", needs context: " << std::boolalpha << block.needsContext() << std::endl;
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
	sf::Window *window = new sf::Window( sf::VideoMode( 800, 600, 32 ), "display", sf::Style::Close, sf::ContextSettings( 0, 0, 0, 2, 1 ) );

	Engine &testEngine = Engine::instance();

	unsigned int numInstances = 2;
	std::vector< BlockInstance > camBlocks( numInstances );
	std::vector< BlockInstance > cvBlocks( numInstances );

	try
	{
		BundleHandle camBundle = testEngine.loadBundle( "CameraCaptureBundle" );
		printBundleInfo( camBundle );
		BundleHandle cvBundle = testEngine.loadBundle( "ComputerVisionBundle" );
		printBundleInfo( cvBundle );

		for ( unsigned int i=0; i<numInstances; ++i )
		{
			BlockInstance &instance = camBlocks[ i ];
			instance.block = camBundle.createFunctionBlockInstance( "CameraCaptureBlock" );

			instance.block.getAllInletHandles( instance.inlets );
			instance.block.getAllParameterHandles( instance.parameters );
			std::vector< OutletHandle > handles;
			instance.block.getAllOutletHandles( handles );
			for ( unsigned int o=0; o<handles.size(); ++o )
				instance.outlets.push_back( std::make_pair( handles[ o ], new OutletReceiver( handles[ o ] ) ) );
			instance.block.setup();
			instance.block.start();

			std::shared_ptr< CustomType > d = instance.parameters[ 0 ].makeData();
			unsigned int &id = *( d->get< unsigned int >( "default" ).get() );
			id = i;
			instance.parameters[ 0 ].setData( d );
		}

		for ( unsigned int i=0; i<numInstances; ++i )
		{
			BlockInstance &instance = cvBlocks[ i ];
			instance.block = cvBundle.createFunctionBlockInstance( "OcvEqualizeHistogramBlock" );

			instance.block.getAllInletHandles( instance.inlets );
			instance.block.getAllParameterHandles( instance.parameters );
			std::vector< OutletHandle > handles;
			instance.block.getAllOutletHandles( handles );
			for ( unsigned int o=0; o<handles.size(); ++o )
				instance.outlets.push_back( std::make_pair( handles[ o ], new OutletReceiver( handles[ o ] ) ) );

			instance.block.setup();
			instance.block.start();
		}

		for ( unsigned int i=0; i<numInstances; ++i )
		{
			BlockInstance &cam = camBlocks[ i ];
			BlockInstance &gauss = cvBlocks[ i ];

			LinkHandle link = cam.outlets[ 0 ].first.link( gauss.inlets[ 0 ] );
			if ( !link.isValid() )
			{
				std::cout << "argh!" << std::endl;
			}
		}
	}
	catch ( Exception &e )
	{
		std::cout << e.what() << " " << e.message() << std::endl;
	}

	try
	{
		while( 1 )
		{
			bool exit = false;
			sf::Event ev;
			while ( window->pollEvent( ev ) )
			{
				if ( sf::Event::Closed == ev.type )
				{
					exit = true;
					break;
				}
			}
			if ( exit ) break;

			window->setActive( true );

			std::shared_ptr< const CustomType > d0 = cvBlocks[ 0 ].outlets[ 0 ].second->getData();
			if ( d0.get() )
			{
				std::shared_ptr< const Image > img = Image::asImage( d0 );
				//std::cout << img->getWidth() << " " << img->getHeight() << std::endl;
				imageToTexture( cvBlocks[ 0 ].texture, img );
			}

			std::shared_ptr< const CustomType > d1 = cvBlocks[ 1 ].outlets[ 0 ].second->getData();
			if ( d1.get() )
			{
				std::shared_ptr< const Image > img = Image::asImage( d1 );
				//std::cout << img->getWidth() << " " << img->getHeight() << std::endl;
				imageToTexture( cvBlocks[ 1 ].texture, img );
			}

			std::shared_ptr< const CustomType > d2 = camBlocks[ 0 ].outlets[ 0 ].second->getData();
			if ( d2.get() )
			{
				std::shared_ptr< const Image > img = Image::asImage( d2 );
				//std::cout << img->getWidth() << " " << img->getHeight() << std::endl;
				imageToTexture( camBlocks[ 0 ].texture, img );
			}

			std::shared_ptr< const CustomType > d3 = camBlocks[ 1 ].outlets[ 0 ].second->getData();
			if ( d3.get() )
			{
				std::shared_ptr< const Image > img = Image::asImage( d3 );
				//std::cout << img->getWidth() << " " << img->getHeight() << std::endl;
				imageToTexture( camBlocks[ 1 ].texture, img );
			}

			glViewport( 0, 0, 800, 600 );
			glClearColor( 0.f, 0.f, 0.f, 1.f );
			glClear( GL_COLOR_BUFFER_BIT );
			glColor3f( 1.f, 1.f, 1.f );
			glEnable( GL_TEXTURE_2D );

			glBindTexture( GL_TEXTURE_2D, cvBlocks[ 0 ].texture.handle );
			glBegin( GL_QUADS );
				glTexCoord2f( 0.f, 0.f );
			glVertex2f( -1.f,  0.f );
				glTexCoord2f( 0.f, 1.f );
			glVertex2f( -1.f, -1.f );
				glTexCoord2f( 1.f, 1.f );
			glVertex2f(  0.f, -1.f );
				glTexCoord2f( 1.f, 0.f );
			glVertex2f(  0.f,  0.f );
			glEnd();
			glBindTexture( GL_TEXTURE_2D, cvBlocks[ 1 ].texture.handle );
			glBegin( GL_QUADS );
				glTexCoord2f( 0.f, 0.f );
			glVertex2f(  0.f,  0.f );
				glTexCoord2f( 0.f, 1.f );
			glVertex2f(  0.f, -1.f );
				glTexCoord2f( 1.f, 1.f );
			glVertex2f(  1.f, -1.f );
				glTexCoord2f( 1.f, 0.f );
			glVertex2f(  1.f,  0.f );
			glEnd();
			glBindTexture( GL_TEXTURE_2D, camBlocks[ 0 ].texture.handle );
			glBegin( GL_QUADS );
				glTexCoord2f( 0.f, 0.f );
			glVertex2f( -1.f,  1.f );
				glTexCoord2f( 0.f, 1.f );
			glVertex2f( -1.f,  0.f );
				glTexCoord2f( 1.f, 1.f );
			glVertex2f(  0.f,  0.f );
				glTexCoord2f( 1.f, 0.f );
			glVertex2f(  0.f,  1.f );
			glEnd();
			glBindTexture( GL_TEXTURE_2D, camBlocks[ 1 ].texture.handle );
			glBegin( GL_QUADS );
				glTexCoord2f( 0.f, 0.f );
			glVertex2f(  0.f,  1.f );
				glTexCoord2f( 0.f, 1.f );
			glVertex2f(  0.f,  0.f );
				glTexCoord2f( 1.f, 1.f );
			glVertex2f(  1.f,  0.f );
				glTexCoord2f( 1.f, 0.f );
			glVertex2f(  1.f,  1.f );
			glEnd();
			glBindTexture( GL_TEXTURE_2D, 0 );

			glDisable( GL_TEXTURE_2D );

			window->display();
		}
	}
	catch ( _2Real::Exception const& e )
	{
		std::cout << e.what() << std::endl;
	}
	catch ( std::exception const& e )
	{
		std::cout << "stdexcept" << std::endl;
	}
	catch ( ... )
	{
		std::cout << "unknownexcept" << std::endl;
	}

	delete window;
	cvBlocks.clear();
	camBlocks.clear();

	return 0;
}