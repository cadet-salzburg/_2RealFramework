#include "ServiceImpl.h"

#include "_2RealFrameworkContext.h"
#include "_2RealException.h"
#include "_2RealEnum.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "SDL.h"

#include <iostream>
#include <string>

using _2Real::FrameworkContext;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

SDLManager::SDLManager() :
	Block()
{
	cout << "sdl manager: hi" << endl;
}

void SDLManager::setup( FrameworkContext &context )
{
	try
	{
		cout << "sdl manager: setup" << endl;
		SDL_Init( SDL_INIT_VIDEO );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_GL_SetSwapInterval( 1 );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void SDLManager::update()
{
	try
	{
		cout << "sdl manager: update" << endl;
		SDL_Event ev;
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
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void SDLManager::shutdown()
{
	try
	{
		cout << "sdl manager: shutdown" << endl;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

SDLManager::~SDLManager()
{
	cout << "sdl manager: bye" << endl;
}

void ImageDisplay::setup( FrameworkContext &context )
{
	try
	{
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};

void ImageDisplay::update()
{
	try
	{
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
};