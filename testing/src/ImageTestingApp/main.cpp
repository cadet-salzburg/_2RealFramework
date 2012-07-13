/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2012 Fachhochschule Salzburg GmbH

		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "_2RealApplication.h"

#include <iostream>
#include <map>
#include <list>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "SDL.h"

#include "Poco/Mutex.h"

#include "vld.h"
#include <sstream>

#include "windows.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;

using _2Real::Exception;

using _2Real::app::Engine;
using _2Real::app::BundleInfo;
using _2Real::app::BlockInfo;
using _2Real::app::BlockHandle;
using _2Real::app::BundleHandle;
using _2Real::app::InletHandle;
using _2Real::app::OutletHandle;
using _2Real::app::AppData;
using _2Real::ImageT;

using Poco::ScopedLock;
using Poco::FastMutex;

class Receiver
{

public:

	Receiver()
	{
		ImageT< float > img( 8, 6, _2Real::ImageChannelOrder::RGBA );
		ImageT< float >::iterator it = img.iter();
		while( it.nextLine() )
		{
			while( it.nextPixel() )
			{
				it.r() = float( rand()%255 + 1 ) / 255.0f;
				it.g() = float( rand()%255 + 1 ) / 255.0f;
				it.b() = float( rand()%255 + 1 ) / 255.0f;
				it.a() = float( rand()%255 + 1 ) / 255.0f;
			}
		}

		m_ImageData.data = img.getData();
		m_ImageData.w = 4;
		m_ImageData.h = 3;


		glGenTextures( 1, &m_Texture );
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_Texture );

		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, 8, 6, 0, GL_RGBA, GL_FLOAT, (const GLvoid *)m_ImageData.data );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );
	}

	~Receiver()
	{
		glDeleteTextures( 1, &m_Texture );
	}

	void updateTexture()
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, 8, 6, 0, GL_RGBA, GL_FLOAT, (const GLvoid *)m_ImageData.data );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );
	}

	void receiveData( AppData const& data )
	{
		m_Access.lock();

		m_ImageData.data = data.getData< ImageT< float > >().getData();
		m_ImageData.w = 4;
		m_ImageData.h = 3;

		// no ImageT dtor will ever be called like this
		//m_Old.push_back( m_Data );
		m_Data = data;

		m_Access.unlock();
	}

	void useData()
	{
		m_Access.lock();

		if ( m_Data.getTypename() == "img_float" )
		{
			// the reason this has to happen here is
			// the opengl context + exection thread problem
			updateTexture();
		}

		m_Access.unlock();

		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );
		glBegin( GL_QUADS );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, 0.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, 0.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f,  1.0f, 0.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f,  1.0f, 0.0f );
		glEnd();
	}

private:

	struct ImageData
	{
		float *data;
		unsigned int w;
		unsigned int h;
	};
	ImageData				m_ImageData;

	AppData					m_Data;
	GLuint					m_Texture;
	Poco::FastMutex			m_Access;
	std::list< AppData >	m_Old;

};

int main( int argc, char *argv[] )
{
	try
	{
		SDL_Init( SDL_INIT_VIDEO );

		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

		SDL_Window *window = SDL_CreateWindow( "yay", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		SDL_GLContext context = SDL_GL_CreateContext( window );
		SDL_GL_MakeCurrent( window, context );

		string error = SDL_GetError();
		cout << error << endl;

		GLenum err = glewInit();
		if ( err != GLEW_OK )
		{
			cout << "glew error" << endl;
		}

		SDL_GL_SetSwapInterval( 1 );

		Engine &testEngine = Engine::instance();

		BundleHandle testBundle = testEngine.loadBundle( "ImageTesting" );

		BlockHandle out = testBundle.createBlockInstance( "image out" );
		out.setUpdateRate( 1.0 );
		out.setup();
		out.start();

		OutletHandle oOut = out.getOutletHandle( "image outlet" );
		Receiver receiver;
		oOut.registerToNewData( receiver, &Receiver::receiveData );

		bool run = true;
		SDL_Event *ev = new SDL_Event;
		while( run )
		{
			while( SDL_PollEvent( ev ) )
			{
				switch ( ev->type )
				{
				case SDL_QUIT:
					oOut.unregisterFromNewData( receiver, &Receiver::receiveData );
					run = false;
					break;
				default:
					break;
				}
			}

			receiver.useData();
			SDL_GL_SwapWindow( window );
		}

		delete ev;
		SDL_GL_DeleteContext( context );
		SDL_DestroyWindow( window );
		SDL_QUIT;
	}
	catch ( std::exception &e )
	{
		cout << e.what() << endl;
	}

	return 0;
}