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
#include <sstream>
#include <map>
#include <list>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "SDL.h"

#include "Poco/Mutex.h"

/*
#ifndef _UNIX
	#include "vld.h"
#endif
*/
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
		m_ImageData.data = NULL;
		m_ImageData.dirty = false;

		glGenTextures( 1, &m_Texture );
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_Texture );

		this->updateDimensions( 640, 480, 24 );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );

		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	}

	~Receiver()
	{
		if( m_ImageData.data )
		{
			delete m_ImageData.data;
			m_ImageData.data = NULL;
		}
		glDeleteTextures( 1, &m_Texture );
	}

	void onKey( const SDL_KeyboardEvent &key )
	{
		switch( key.keysym.scancode )
		{
		case SDL_SCANCODE_SPACE:
			break;
		}
	}

	void updateDimensions( unsigned int width, unsigned int height, unsigned int bpp )
	{
		if( m_ImageData.data )
			delete m_ImageData.data;

		m_ImageData.w = width;
		m_ImageData.h = height;
		m_ImageData.d = bpp / 8;

		std::cout << "set texture dimensions to " << m_ImageData.w << " x " << m_ImageData.h << " x " << m_ImageData.d << std::endl;

		m_ImageData.dataSize = m_ImageData.w * m_ImageData.h * m_ImageData.d;
		m_ImageData.data = new unsigned char[m_ImageData.dataSize];

		//TODO: set format according to color channel order and bpp
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, m_ImageData.w, m_ImageData.h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	}

	void updateTexture()
	{
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_ImageData.w, m_ImageData.h, GL_RGB, GL_UNSIGNED_BYTE, m_ImageData.data );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );
	}

	void receiveFaces( AppData const &data )
	{
		std::cout << "received faces" << std::endl;
	}

	void receiveImage( AppData const &data )
	{
		std::cout << "received " << data.getName() << std::endl;

		m_Access.lock();

		//TODO: handle (changes in) image type
		//TODO: handle (changes in) bpp
		//TODO: handle (changes in) byte order

		_2Real::Image img = data.getData< _2Real::Image >();
		if( img.getWidth() != m_ImageData.w || img.getHeight() != m_ImageData.h )// || img.getBitsPerPixel() / 8 != m_ImageData.d ) //TODO:  //TODO: this return 8 when in fact it should return 24
			this->updateDimensions( img.getWidth(), img.getHeight(), 
										24 );// img.getBitsPerPixel() ); //TODO: this return 8 when in fact it should return 24

		if( img.getByteSize() != m_ImageData.dataSize )
			std::cerr << "ERROR: received image with unexpected data size" << std::endl;
		else
		{
			memcpy( m_ImageData.data, img.getData(), img.getByteSize() );
			m_ImageData.dirty = true;
		}

		m_Access.unlock();
	}

	void display()
	{
		m_Access.lock();
		if( m_ImageData.dirty )
		{
			updateTexture();
			m_ImageData.dirty = false;
		}
		m_Access.unlock();

		glClear( GL_COLOR_BUFFER_BIT );
		glBegin( GL_QUADS );
		{
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, 0.0f );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, 0.0f );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f,  1.0f, 0.0f );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f,  1.0f, 0.0f );
		}
		glEnd();
	}

private:

	struct ImageData
	{
		unsigned char	*data;

		unsigned int	dataSize;

		unsigned int	w;
		unsigned int	h;
		unsigned int	d;

		bool			dirty;
	};
	ImageData				m_ImageData;

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

		Receiver receiver;

		Engine &testEngine = Engine::instance();
		testEngine.setBaseDirectory( "E:\\work\\source\\cadetFH\\_2RealFramework\\bundles\\bin" );

		BundleHandle camBundle = testEngine.loadBundle( "CameraCaptureBundle" );

		BlockHandle camera = camBundle.createBlockInstance( "CameraCaptureBlock" );
		camera.setUpdateRate( 30.0 );

		InletHandle inDeviceIndex = camera.getInletHandle( "DeviceIndex" );
		InletHandle inWidth = camera.getInletHandle( "Width" );
		InletHandle inHeight = camera.getInletHandle( "Height" );
		InletHandle inFPS = camera.getInletHandle( "Fps" );

		OutletHandle camOutImg = camera.getOutletHandle( "ImageData" );
		OutletHandle camOutWidth = camera.getOutletHandle( "Width" );
		OutletHandle camOutHeight = camera.getOutletHandle( "Height" );

		BundleHandle ftBundle = testEngine.loadBundle( "FaceTrackingBundle" );

		BlockHandle faceFeatures = ftBundle.createBlockInstance( "FaceFeaturesBlock" );
		faceFeatures.setUpdateRate( 30.0f );

		InletHandle ffInImg = faceFeatures.getInletHandle( "image_in" );
		ffInImg.setUpdatePolicy( InletHandle::OR_NEWER_DATA );

		OutletHandle ffOutFaces = faceFeatures.getOutletHandle( "faces_out" );
		if ( !ffInImg.tryLink( camOutImg ) )
		{
			std::cerr << "WARNING: conversion failed, trying to link with conversion" << std::endl;
			ffInImg.tryLinkWithConversion( camOutImg );
			std::cerr << "succeeded!" << std::endl;
		}

		ffOutFaces.registerToNewData( receiver, &Receiver::receiveFaces );

		camOutImg.registerToNewData( receiver, &Receiver::receiveImage );

		camera.setup();
		camera.start();

		faceFeatures.setup();
		faceFeatures.start();

		bool run = true;
		SDL_Event *ev = new SDL_Event;
		while( run )
		{
			while( SDL_PollEvent( ev ) )
			{
				switch ( ev->type )
				{
				case SDL_KEYDOWN:
					receiver.onKey( ev->key );
					break;
				case SDL_QUIT:
					camera.stop();
					faceFeatures.stop();

					camOutImg.unregisterFromNewData( receiver, &Receiver::receiveFaces );
					camOutImg.unregisterFromNewData( receiver, &Receiver::receiveImage );

					run = false;

					break;
				default:
					break;
				}
			}

			receiver.display();
			SDL_GL_SwapWindow( window );
		}

		delete ev;
		SDL_GL_DeleteContext( context );
		SDL_DestroyWindow( window );
		SDL_QUIT;

		testEngine.safeConfig( "img_test.xml" );

		camBundle.unload();
		ftBundle.unload();
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;

#ifdef _DEBUG
		getchar();
#endif
	}

	return 0;
}