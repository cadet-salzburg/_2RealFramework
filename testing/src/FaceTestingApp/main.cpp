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

#include "windows.h"
#include "GL/GL.h"
#include "GL/GLU.h"

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



bool exiting	=	false;

bool run		=	true;


SDL_GLContext context;
SDL_Window *window	=	NULL;

Engine &engine = Engine::instance();

BundleHandle oniBundle;

BlockHandle camera;

BlockHandle rgb;
BlockHandle depth;

OutletHandle rgbOutImg;
OutletHandle depthOutImg;


void initSDLGL()
{
	SDL_Init( SDL_INIT_VIDEO );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	window = SDL_CreateWindow( "yay", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	context = SDL_GL_CreateContext( window );
	SDL_GL_MakeCurrent( window, context );

	string error = SDL_GetError();
	cout << error << endl;

	GLenum err = glewInit();
	if ( err != GLEW_OK )
	{
		cout << "glew error" << endl;
	}

	SDL_GL_SetSwapInterval( 1 );
}

void setup()
{
	initSDLGL();

	engine.setBaseDirectory( "E:\\work\\source\\_2RealFramework\\bundles\\bin" );

	oniBundle = engine.loadBundle( "KinectOpenNIBundle" );

	rgb = oniBundle.createBlockInstance( "KinectOpenNIRgbBlock" );
	depth = oniBundle.createBlockInstance( "KinectOpenNIDepthBlock" );

	rgb.setUpdateRate( 30.0 );
	depth.setUpdateRate( 30.0 );

	rgbOutImg = rgb.getOutletHandle( "ImageData" );
	depthOutImg = depth.getOutletHandle( "ImageData" );

	rgb.setup();
	rgb.start();

	depth.setup();
	depth.start();
}

void cleanup()
{
	if( camera.isValid() )
		camera.stop();
	if( rgb.isValid() )
		rgb.stop();
	if( depth.isValid() )
		depth.stop();

	SDL_GL_DeleteContext( context );
	if( window )
	{
		SDL_DestroyWindow( window );
		window = NULL;
	}

	engine.safeConfig( "img_test.xml" );

	if( oniBundle.isValid() )
		oniBundle.unload();
}

void __cdecl onExit()
{
	exiting = true;

	std::cout << "on exit called" << std::endl;

	cleanup();
}

void __cdecl onTerminate()
{
	std::cerr << "process terminated" << std::endl;
	abort();
}

void __cdecl onUnexpected()
{
	std::cerr << "an unexpected error occured" << std::endl;
}

int main( int argc, char *argv[] )
{
	atexit( onExit );
	std::set_terminate( onTerminate );
	std::set_unexpected( onUnexpected );

	try
	{
		setup();

		SDL_Event ev;
		while( ::run )
		{
			while( SDL_PollEvent( &ev ) )
			{
				switch ( ev.type )
				{
				case SDL_KEYDOWN:
					if( ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
						run = false;
					break;
				case SDL_QUIT:
					run = false;

					break;
				default:
					break;
				}
			}

			SDL_GL_SwapWindow( window );
		}
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