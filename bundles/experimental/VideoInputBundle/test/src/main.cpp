#include <windows.h>
#include <iostream>
#include <string>

#include "_2RealApplication.h"
#include "_2RealEngine.h"
#include "_2RealUpdatePolicy.h"
#include "_2RealParameterData.h"
#include "_2RealImageT.h"
#include <boost/thread/mutex.hpp>
#include "GL/glew.h"
#include "GL/wglew.h"
#include "SDL.h"

#ifndef _DEBUG
#define shared_library_suffix "_32.dll"
#else
#define shared_library_suffix "_32d.dll"
#endif

using _2Real::ImageT;
using _2Real::ImageChannelOrder;
using _2Real::Exception;
using namespace _2Real;
using namespace std;


class ImageContainer
{
public:
	ImageContainer( const unsigned int r, const unsigned int g, const unsigned int b ) 
		: m_Image( 640, 480, ImageChannelOrder::RGB )
	{
		std::cout << "receiver ctor" << std::endl;
		ImageT< unsigned char >::iterator it = m_Image.iter();
		while( it.nextLine() )
		{
			while( it.nextPixel() )
			{
				it.r() = r;
				it.g() = g;
				it.b() = b;
			}
		}
		glGenTextures( 1, &m_Texture );
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_Texture );
		updateTexture();
	}
	~ImageContainer()
	{
		glDisable( GL_TEXTURE_2D );
		glDeleteTextures(1, &m_Texture);
	}
	void updateTexture()
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, m_Image.getWidth(), m_Image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid *)m_Image.getData() );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );
	}
	void renderTexture()
	{
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );
		glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 0.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f, -1.0f, 0.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f,  1.0f, 0.0f );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, 0.0f );
		glEnd();
	}

	void setImage( ImageT<unsigned char> img )
	{
		m_Image = img;
	}
private:
	GLuint						m_Texture;
	ImageT< unsigned char >		m_Image;
};

class videoApp {
public:
	ImageT<unsigned char>	m_VideoImage;
	BundleIdentifier		m_BundleId;	
	BlockIdentifier			m_BlockId; 
	boost::mutex			m_Mutex;
	int m_DeviceId;

	void updateData(std::list<OutputData> data)
	{
		m_Mutex.lock();
		m_VideoImage = data.front().getData< ImageT<unsigned char> >();
		m_Mutex.unlock();
	}
	void startup()
	{
		m_DeviceId = 0;
		std::string directory	= "../experimental/bin/win/";
		Engine		&m_Engine	= Engine::instance();
		System*		m_System	= new _2Real::System( "VideoInput_System" );
		std::string bundleName	= "VideoInputBundle";
		std::string blockName	= "VideoInputBlock";
		try 
		{
			//Load bundle for use in runtime engine
			m_Engine.setBaseDirectory( directory );
			m_BundleId = m_Engine.load( bundleName.append( shared_library_suffix ) );
			m_BlockId  = m_System->createBlock( m_BundleId, blockName );

			//Create certain blocks to a runtime system
			UpdatePolicy fpsTrigger;
			fpsTrigger.triggerByUpdateRate( 100.f );
			m_System->setPolicy(m_BlockId, fpsTrigger);
			m_System->setup(m_BlockId);
			m_System->start(m_BlockId);
			// setup callbacks
			m_System->registerToAllOutletData( m_BlockId, *this, &videoApp::updateData );

			SDL_Init( SDL_INIT_VIDEO );
			SDL_Window		*window;
			SDL_GLContext	context;
			//
			window	= SDL_CreateWindow( "Webcam Output", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
			context = SDL_GL_CreateContext( window );
			string error = SDL_GetError();
			cout << error << endl;

			//needs to happen after 1st window
			GLenum err = glewInit();
			if ( err != GLEW_OK )
			{
				throw Exception( "glew error" );
			}

			SDL_GL_MakeCurrent( window, context );
			ImageContainer* image = new ImageContainer( 50, 50, 50 );
			SDL_GL_SetSwapInterval( 1 );
			bool run = true;
			SDL_Event ev;
			while( run )
			{
				while( SDL_PollEvent(&ev) )
				{
					switch ( ev.type )
					{
					case SDL_WINDOWEVENT:
						cout << "received window event" << endl;
						switch( ev.window.event )
						{
						case SDL_WINDOWEVENT_SHOWN:
							cout << "window shown " << endl;
							break;
						case SDL_WINDOWEVENT_HIDDEN:
							cout << "window hidden " << endl;
							break;
						case SDL_WINDOWEVENT_EXPOSED:
							cout << "window exposed " << endl;
							break;
						case SDL_WINDOWEVENT_MOVED:
							cout << "window moved " << endl;
							break;
						case SDL_WINDOWEVENT_RESIZED:
							cout << "window resized " << endl;
							break;
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							cout << "window size changed " << endl;
							break;
						case SDL_WINDOWEVENT_MINIMIZED:
							cout << "window minimized " << endl;
							break;
						case SDL_WINDOWEVENT_MAXIMIZED:
							cout << "window maximized " << endl;
							break;
						case SDL_WINDOWEVENT_RESTORED:
							cout << "window restored " << endl;
							break;
						case SDL_WINDOWEVENT_ENTER:
							cout << "window entered " << endl;
							break;
						case SDL_WINDOWEVENT_LEAVE:
							cout << "window left " << endl;
							break;
						case SDL_WINDOWEVENT_FOCUS_GAINED:
							cout << "window gained focus " << endl;
							break;
						case SDL_WINDOWEVENT_FOCUS_LOST:
							cout << "window lost focus " << endl;
							break;
						case SDL_WINDOWEVENT_CLOSE:
							cout << "window closed " << endl;
							break;
						default:
							cout << "other window event " << endl;
							cout << (unsigned int)ev.window.event << endl;
							break;
						}
						break;
					case SDL_SYSWMEVENT:
						cout << "received system event" << endl;
						break;
					case SDL_QUIT:
						cout << "received quit event" << endl;
						break;
					case SDL_USEREVENT:
						cout << "received user event" << endl;
						break;
					case SDL_MOUSEMOTION:
						cout << "received mouse motion event" << endl;
						break;
					case SDL_KEYUP:
						cout << "received key up event" << endl;
						break;
					case SDL_KEYDOWN:
						cout << "received key down event" << endl;
						switch(ev.key.keysym.sym)
						{
						case SDLK_UP:
							m_DeviceId-=1;
							m_System->setValue<int>(m_BlockId, "deviceIndexInlet", m_DeviceId );
							break;
						case SDLK_DOWN:
							m_DeviceId+=1;
							m_System->setValue<int>(m_BlockId, "deviceIndexInlet", m_DeviceId );
							break;
						}
						break;
					case SDL_MOUSEBUTTONUP:
						cout << "received mousebutton up event" << endl;
						break;
					case SDL_MOUSEBUTTONDOWN:
						cout << "received mousebutton down event" << endl;
						break;
					case SDL_MOUSEWHEEL:
						cout << "received mouse wheel event" << endl;
						break;
					default:
						cout << "received other event" << endl;
						break;
					}
				}

				SDL_GL_MakeCurrent( window, context );
				m_Mutex.lock();
				image->setImage(m_VideoImage);
				m_Mutex.unlock();
				image->updateTexture();
				image->renderTexture();
				SDL_GL_SwapWindow( window );

			}
			SDL_GL_DeleteContext( context );
			SDL_DestroyWindow( window );
			delete image;
			SDL_Quit();
		}
		catch ( _2Real::Exception &e )
		{
			std::cout << e.message() << std::endl;
		}
	}
};

int main(int argc, char *argv[])
{
	videoApp  theApp;
	theApp.startup();




	return 0;
};


