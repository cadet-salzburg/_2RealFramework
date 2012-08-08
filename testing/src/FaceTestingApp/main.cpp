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

using namespace _2Real;

bool run = true;

namespace _2Real
{
	inline const Vec3 &white()
	{
		static Vec3 w( 1.0 );
		return w;
	}

	inline const Vec3 &black()
	{
		static Vec3 b( 0.0 );
		return b;
	}

	inline const Vec3 &red()
	{
		static Vec3 r( 1.0, 0.0, 0.0 );
		return r;
	}

	inline const Vec3 &green()
	{
		static Vec3 g( 0.0, 1.0, 0.0 );
		return g;
	}

	inline const Vec3 &blue()
	{
		static Vec3 b( 0.0, 0.0, 1.0 );
		return b;
	}

	inline const Vec3 &cyan()
	{
		static Vec3 c( 0.0, 1.0, 1.0 );
		return c;
	}

	inline const Vec3 &magenta()
	{
		static Vec3 m( 1.0, 0.0, 1.0 );
		return m;
	}

	inline const Vec3 &yellow()
	{
		static Vec3 y( 1.0, 1.0, 0.0 );
		return y;
	}

	inline const Vec3 &darkgrey()
	{
		static Vec3 dg( 0.25 );
		return dg;
	}

	inline const Vec3 &grey()
	{
		static Vec3 g( 0.5 );
		return g;
	}

	inline const Vec3 &lightgrey()
	{
		static Vec3 lg( 0.75 );
		return lg;
	}
}

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

		this->updateDimensions( 320, 240, 24 );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );

		glDisable( GL_DEPTH_TEST );

		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

		m_colors.push_back( red() );
		m_colors.push_back( green() );
		m_colors.push_back( blue() );
		m_colors.push_back( cyan() );
		m_colors.push_back( magenta() );
		m_colors.push_back( yellow() );
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
		case SDL_SCANCODE_ESCAPE:
			::run = false;
			break;
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

	void drawRect( float x0, float y0, float x1, float y1 )
	{
		x0 = ( x0 - 0.5f ) * 2.0f;	//lower left is -1/-1
		y0 = ( 0.5f - y0 ) * 2.0f;
		x1 = ( x1 - 0.5f ) * 2.0f;	//upper right is 1/1
		y1 = ( 0.5f - y1 ) * 2.0f;

		glPushAttrib( GL_ALL_ATTRIB_BITS );
		{
			glEnable( GL_COLOR_MATERIAL );
			glDisable( GL_TEXTURE_2D );

			glBegin( GL_LINE_LOOP );
			{
				glVertex3f( x0, y0, 0.0f );
				glVertex3f( x0, y1, 0.0f );
				glVertex3f( x1, y1, 0.0f );
				glVertex3f( x1, y0, 0.0f );
			}
			glEnd();
		}
		glPopAttrib();
	}

	void drawRect( const _2Real::Vec2 &v0, const _2Real::Vec2 &v1 )
	{
		drawRect( v0[0], v0[1], v1[0], v1[1] );
	}

	void drawFaceRects()
	{
		glPushAttrib( GL_ALL_ATTRIB_BITS );
		{
			int colorCntr = 0;
			glLineWidth( 4.0f );
			for( std::vector< _2Real::Space2D >::const_iterator it = m_FaceData.faces.begin(); it != m_FaceData.faces.end(); ++it, colorCntr++ )
			{
				glColor3fv( m_colors[colorCntr % m_colors.size()].begin() );
				drawRect( it->getP0(), it->getP1() );
			}

			colorCntr = 0;
			glLineWidth( 3.0f );
			for( std::vector< _2Real::Space2D >::const_iterator it = m_FaceData.eyes.begin(); it != m_FaceData.eyes.end(); ++it, colorCntr++ )
			{
				glColor3fv( m_colors[colorCntr % m_colors.size()].begin() );
				drawRect( it->getP0(), it->getP1() );
			}

			colorCntr = 0;
			glLineWidth( 2.0f );
			for( std::vector< _2Real::Space2D >::const_iterator it = m_FaceData.noses.begin(); it != m_FaceData.noses.end(); ++it, colorCntr++ )
			{
				glColor3fv( m_colors[colorCntr % m_colors.size()].begin() );
				drawRect( it->getP0(), it->getP1() );
			}

			colorCntr = 0;
			glLineWidth( 1.0f );
			for( std::vector< _2Real::Space2D >::const_iterator it = m_FaceData.mouths.begin(); it != m_FaceData.mouths.end(); ++it, colorCntr++ )
			{
				glColor3fv( m_colors[colorCntr % m_colors.size()].begin() );
				drawRect( it->getP0(), it->getP1() );
			}
		}
		glPopAttrib();
	}

	void receiveFaces( AppData const &data )
	{
		m_Access.lock();

		m_FaceData.faces = data.getData< std::vector< _2Real::Space2D > >();
		m_FaceData.dirty = true;
		std::cout << "received " << m_FaceData.faces.size() << " faces" << std::endl;

		m_Access.unlock();
	}

	void receiveEyes( AppData const &data )
	{
		m_Access.lock();

		m_FaceData.eyes = data.getData< std::vector< _2Real::Space2D > >();
		m_FaceData.dirty = true;
		std::cout << "received " << m_FaceData.eyes.size() << " eyes" << std::endl;

		m_Access.unlock();
	}

	void receiveNoses( AppData const &data )
	{
		m_Access.lock();

		m_FaceData.noses = data.getData< std::vector< _2Real::Space2D > >();
		m_FaceData.dirty = true;
		std::cout << "received " << m_FaceData.noses.size() << " noses" << std::endl;

		m_Access.unlock();
	}

	void receiveMouths( AppData const &data )
	{
		m_Access.lock();

		m_FaceData.mouths = data.getData< std::vector< _2Real::Space2D > >();
		m_FaceData.dirty = true;
		std::cout << "received " << m_FaceData.mouths.size() << " mouths" << std::endl;

		m_Access.unlock();
	}

	void receiveImage( AppData const &data )
	{
		m_Access.lock();

		//TODO: handle (changes in) image type
		//TODO: handle (changes in) bpp
		//TODO: handle (changes in) byte order

		_2Real::Image img = data.getData< _2Real::Image >();
		if( img.getWidth() != m_ImageData.w || img.getHeight() != m_ImageData.h || img.getBitsPerPixel() / 8 != m_ImageData.d ) //TODO:  //TODO: this return 8 when in fact it should return 24
			this->updateDimensions( img.getWidth(), img.getHeight(), img.getBitsPerPixel() ); //TODO: this return 8 when in fact it should return 24

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
		glClear( GL_COLOR_BUFFER_BIT );

		m_Access.lock();
		if( m_ImageData.dirty )
		{
			updateTexture();
			m_ImageData.dirty = false;
		}
		m_Access.unlock();

		glColor3fv( white().begin() );
		glBegin( GL_QUADS );
		{
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, 0.0f );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, 0.0f );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f,  1.0f, 0.0f );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f,  1.0f, 0.0f );
		}
		glEnd();

		m_Access.lock();

		drawFaceRects();
		m_FaceData.dirty = false;

		m_Access.unlock();
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
	ImageData			m_ImageData;

	struct FaceData
	{
		std::vector< _2Real::Space2D >	faces;
		std::vector< _2Real::Space2D >	eyes;
		std::vector< _2Real::Space2D >	noses;
		std::vector< _2Real::Space2D >	mouths;

		bool			dirty;
	};
	FaceData			m_FaceData;

	std::vector< _2Real::Vec3 >		m_colors;

	GLuint					m_Texture;
	Poco::FastMutex			m_Access;
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

		Engine &engine = Engine::instance();
		engine.setBaseDirectory( "E:\\work\\source\\cadetFH\\_2RealFramework\\bundles\\bin" );

		BundleHandle camBundle = engine.loadBundle( "CameraCaptureBundle" );

		BlockHandle camera = camBundle.createBlockInstance( "CameraCaptureBlock" );
		camera.setUpdateRate( 30.0 );
		//camera.setUpdateRate( 1.0 );

		InletHandle inDeviceIndex = camera.getInletHandle( "DeviceIndex" );
		InletHandle inWidth = camera.getInletHandle( "Width" );
		InletHandle inHeight = camera.getInletHandle( "Height" );
		InletHandle inFPS = camera.getInletHandle( "Fps" );

		OutletHandle camOutImg = camera.getOutletHandle( "ImageData" );
		OutletHandle camOutWidth = camera.getOutletHandle( "Width" );
		OutletHandle camOutHeight = camera.getOutletHandle( "Height" );

		BundleHandle ftBundle = engine.loadBundle( "FaceTrackingBundle" );

		BlockHandle faceFeatures = ftBundle.createBlockInstance( "FaceFeaturesBlock" );
		faceFeatures.setUpdateRate( 30.0f );

		InletHandle ffInImg = faceFeatures.getInletHandle( "image_in" );

		faceFeatures.getInletHandle( "use_face" ).setValue( true );
		faceFeatures.getInletHandle( "use_eyes" ).setValue( true );
		faceFeatures.getInletHandle( "use_nose" ).setValue( true );
		faceFeatures.getInletHandle( "use_mouth" ).setValue( true );

		ffInImg.setUpdatePolicy( InletHandle::OR_NEWER_DATA );

		if ( !ffInImg.link( camOutImg ) )
		{
			std::cerr << "WARNING: conversion failed, trying to link with conversion" << std::endl;
			ffInImg.linkWithConversion( camOutImg );
			std::cerr << "succeeded!" << std::endl;
		}

		faceFeatures.getOutletHandle( "face_out" ).registerToNewData( receiver, &Receiver::receiveFaces );
		faceFeatures.getOutletHandle( "eyes_out" ).registerToNewData( receiver, &Receiver::receiveEyes );
		faceFeatures.getOutletHandle( "nose_out" ).registerToNewData( receiver, &Receiver::receiveNoses );
		faceFeatures.getOutletHandle( "mouth_out" ).registerToNewData( receiver, &Receiver::receiveMouths );

		camOutImg.registerToNewData( receiver, &Receiver::receiveImage );

		camera.setup();
		camera.start();

		faceFeatures.setup();
		faceFeatures.start();

		SDL_Event ev;
		while( ::run )
		{
			while( SDL_PollEvent( &ev ) )
			{
				switch ( ev.type )
				{
				case SDL_KEYDOWN:
					receiver.onKey( ev.key );
					break;
				case SDL_QUIT:
					run = false;

					break;
				default:
					break;
				}
			}

			receiver.display();
			SDL_GL_SwapWindow( window );
		}

		camera.stop();
		faceFeatures.stop();

		camOutImg.unregisterFromNewData( receiver, &Receiver::receiveFaces );
		camOutImg.unregisterFromNewData( receiver, &Receiver::receiveEyes );
		camOutImg.unregisterFromNewData( receiver, &Receiver::receiveNoses );
		camOutImg.unregisterFromNewData( receiver, &Receiver::receiveMouths );

		camOutImg.unregisterFromNewData( receiver, &Receiver::receiveImage );

		SDL_GL_DeleteContext( context );
		SDL_DestroyWindow( window );

		engine.safeConfig( "img_test.xml" );

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