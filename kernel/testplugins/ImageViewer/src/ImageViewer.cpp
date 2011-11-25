/*
 * Name of Plugin is "ImageViewer"
 * Name of Service is "DisplayWindow"
 * Name of input parameter: "input image"
 * Name of output parameter: "status"
 * m.
 */


#include "_2RealPluginContext.h"
#include "_2RealPluginMetadata.h"
#include "_2RealMetadataReader.h"
#include "_2RealImagebuffer.h"
#include "_2RealData.h"
#include "_2RealException.h"
#include "_2RealRunnableException.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "SDL.h"
#include "Poco\Mutex.h"
#include "ImageViewer.h"

#include <stdio.h>

using namespace _2Real;
using namespace Poco;
using namespace std;

// ui... these should be member variables, but are not...
	bool					run;
	GLuint						m_tex;
	FastMutex					m_mutex;
	Buffer2D_float				m_tmp;
	Buffer2D_float				m_buffer;
	SDL_Window					*m_mainwindow;
	SDL_GLContext				m_maincontext;

void imgDataAvailable(_2Real::Data &data)
{
	Poco::FastMutex::ScopedLock lock(m_mutex);
	m_tmp = data.getData< Buffer2D_float >();
}


void systemException(RunnableException &e)
{
	std::cout << "exception in " << e.system().name() << " by " << e.sender().name() << " : " << e.what() << std::endl;
}

void init()
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_tex);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void exit()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &m_tex);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 2.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
	glEnd();
}

void loop()
{
	{
		FastMutex::ScopedLock lock(m_mutex);
		m_buffer = m_tmp;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(-1.0f, -1.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480, 0, GL_RGB, GL_FLOAT, (const GLvoid *)m_buffer.rawData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void processEvent(SDL_Event *e)
{
	switch (e->type) 
	{
		case SDL_QUIT:
			run = false;
			break;
	}
}



_2Real::IService *const createservice_displaywindow()
{
	_2Real::IService *service = new displaywindow();
	return service;
}

void displaywindow::setup(_2Real::ServiceContext &context) {
    
	std::cout << "starting setup" << std::endl;
	
    m_Output = context.getOutputHandle("status");   
	m_InputImage = context.getInputHandle("input image");
	std::cout << "got input and output handle" << std::endl;

	

	try
	{
		SDL_Init(SDL_INIT_VIDEO);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		m_mainwindow = SDL_CreateWindow("gaudeamus igitur", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		m_maincontext = SDL_GL_CreateContext(m_mainwindow);

		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			throw _2Real::Exception("glew error");
		}

		SDL_GL_SetSwapInterval(1);

		init();
	}
	catch (_2Real::Exception &e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "an unknown exception occurred" << endl;
	}



}


void displaywindow::update() {
	//get input...






	//display it
	run = true;
	SDL_Event ev;
	while(SDL_PollEvent(&ev)) {
		processEvent(&ev);
	}

	loop();
	render();

	SDL_GL_SwapWindow(m_mainwindow);




}


displaywindow::~displaywindow() {

	SDL_GL_DeleteContext(m_maincontext);
	SDL_DestroyWindow(m_mainwindow);
	SDL_Quit();
}