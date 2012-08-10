
#include "WindowBlock.h"
#include "_2RealDatatypes.h"

#include <iostream>

//#include "unsupported/Eigen/OpenGLSupport"

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

#define glewGetContext() this->getGlewContext()

WindowBlock::WindowBlock() : Block(), m_Win( nullptr ) {}
WindowBlock::~WindowBlock() {}

GLEWContext * WindowBlock::getGlewContext() const
{
	return m_GlewContext;
}

void WindowBlock::setup( BlockHandle &block )
{
	try
	{
		m_Block = block;

		if ( m_Win == nullptr )
		{
			sf::ContextSettings windowSettings( 16, 8, 32, 2, 0 );
			m_Win = new sf::Window( sf::VideoMode( 400, 300, 32 ), "uberhorn", sf::Style::Resize, windowSettings );

			if ( m_Win == nullptr )
			{
				throw _2Real::Exception( "could not create SFML window" );
			}

			m_GlewContext = new GLEWContext();
			GLenum success = glewContextInit( m_GlewContext );
			if ( success != GLEW_OK )
			{
				throw _2Real::Exception( "could not create GLEW context" );
			}
			else
			{
				success = glewInit();
				if ( success != GLEW_OK )
				{
					throw _2Real::Exception( "could not initialize GLEW" );
				}
			}

			m_Win->setTitle( m_Block.getInletHandle( "window_title" ).getReadableRef< std::string >() );

			//windowSettings = m_Win->getSettings();

			//std::cout << "OPENGL SETTINGS: " << std::endl;
			//std::cout << "version: " << windowSettings.majorVersion << "." << windowSettings.minorVersion << std::endl;
			//std::cout << "antialiasing samples: " << windowSettings.antialiasingLevel << std::endl;

			glGenTextures( 1, &m_Texture );
			glBindTexture( GL_TEXTURE_2D, m_Texture );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 4, 3, 0, GL_RGBA, GL_FLOAT, nullptr );
			glGenerateMipmap( GL_TEXTURE_2D );

			m_Win->setActive( false );		// immediately disable window in this thread
											// otherwise nothing can be drawn into it :)
		}
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void WindowBlock::update()
{
	try
	{
		m_Win->setActive( true );									// make window ( and thus, the opengl context ) active in this thread

		sf::Event ev;
		while ( m_Win->pollEvent( ev ) )
		{
			if ( ev.type == sf::Event::Resized )
			{
				glViewport( 0, 0, ev.size.width, ev.size.height );
			}
		}

		// draw stuff

		ImageT< float > img( 4, 3, ImageChannelOrder::RGBA );
		ImageT< float >::iterator it = img.iter();
		while( it.nextLine() )
		{
			while ( it.nextPixel() )
			{
				it.r() = float( rand()%255 + 1 ) / 255.0f;
				it.g() = float( rand()%255 + 1 ) / 255.0f;
				it.b() = float( rand()%255 + 1 ) / 255.0f;
				it.a() = float( rand()%255 + 1 ) / 100.0f;
			}
		}

		glBindTexture( GL_TEXTURE_2D, m_Texture );
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 4, 3, GL_RGBA, GL_FLOAT, img.getData() );
		glGenerateMipmap( GL_TEXTURE_2D );

		m_Win->setTitle( m_Block.getInletHandle( "window_title" ).getReadableRef< std::string >() );
		_2Real::Vec3 const& vec = m_Block.getInletHandle( "clear_color" ).getReadableRef< _2Real::Vec3 >();
		_2Real::Mat4 const& mat = m_Block.getInletHandle( "scale_mat" ).getReadableRef< _2Real::Mat4 >();

		glClearColor( (GLfloat)vec.x(), (GLfloat)vec.y(), (GLfloat)vec.z(), 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glMatrixMode( GL_MODELVIEW );
		glLoadMatrixd( mat.data() );

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		glEnable( GL_TEXTURE_2D );
		glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( -1.0f, -1.0f );
		glTexCoord2f( -0.5f, 1.0f ); glVertex2f( -1.0f, 1.0f );
		glTexCoord2f( 1.5f, 1.0f ); glVertex2f( 1.0f, 1.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( 1.0f, -1.0f );
		glEnd();

		m_Win->display();
		m_Win->setActive( false );
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void WindowBlock::shutdown()
{
	try
	{
		glDeleteTextures( 1, &m_Texture );
		delete m_Win;
		delete m_GlewContext;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}
