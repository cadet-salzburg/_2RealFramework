/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH

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

#include "_2RealSystem.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"
#include "_2RealRunnableError.h"
#include "_2RealData.h"
#include "_2RealImagebuffer.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "SDL.h"

#include <windows.h>
#include <iostream>
#include <map>

#include "Poco\Mutex.h"

using namespace _2Real;
using namespace std;
using namespace Poco;

bool run;
Buffer2D_float tmp;
Buffer2D_float buffer;
GLuint tex;
FastMutex mutex;

void imgDataAvailable(Data &data)
{
	FastMutex::ScopedLock lock(mutex);
	tmp = data.getData< Buffer2D_float >();
}

void systemException(RunnableError &e)
{
	std::cout << "exception in " << e.system().name() << " by " << e.sender().name() << std::endl;
}

void init()
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &tex);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void exit()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &tex);
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
		FastMutex::ScopedLock lock(mutex);
		buffer = tmp;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(-1.0f, -1.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480, 0, GL_RGB, GL_FLOAT, (const GLvoid *)buffer.rawData());
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

int main(int argc, char *argv[])
{

	SDL_Window *mainwindow;
	SDL_GLContext maincontext;

	try
	{
		SDL_Init(SDL_INIT_VIDEO);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		mainwindow = SDL_CreateWindow("gaudiamus igitur", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		maincontext = SDL_GL_CreateContext(mainwindow);

		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			throw _2Real::Exception("glew error");
		}

		SDL_GL_SetSwapInterval(1);

		init();

		//////////////////////////////////////////////framework part starts here/////////////////////////////////////////////////////////////////////////////////

		System testSystem("test system");

		testSystem.registerToException(::systemException);
		testSystem.setInstallDirectory("D:\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\");
		testSystem.setLogfile("testsystem.txt");

		/**
		*	assumes that dll name = classname.dll for relase or classname_d.dll for debug
		*/

		Identifier kinectPlugin = testSystem.load("kinect plugin", "MultiKinectOpenNI");

		//std::string kinectInfo = testSystem.getInfo(kinectPlugin);
		//std::cout << kinectInfo << std::endl;

		Identifier imgPlugin = testSystem.load("image processing plugin", "ImageProcessing");

		//std::string imgInfo = testSystem.getInfo(imgPlugin);
		//std::cout << imgInfo << std::endl;

		vector< string > genFlags;
		genFlags.push_back("color");
		genFlags.push_back("depth");
		genFlags.push_back("user colored");

		vector< string > imgFlags;
		imgFlags.push_back("color640X480");
		imgFlags.push_back("depth640X480");
		imgFlags.push_back("mirrored");

		testSystem.setValue< bool >(kinectPlugin, "align color depth", true);
		testSystem.setValue< vector < string > >(kinectPlugin, "generator flags", genFlags);
		testSystem.setValue< vector < string > >(kinectPlugin, "image flags", imgFlags);
		testSystem.setValue< string >(kinectPlugin, "logfile", "kinectwrapper.txt");
		testSystem.setValue< string >(kinectPlugin, "loglevel", "info");
		testSystem.setup(kinectPlugin);

		cout << "main: KINECT PLUGIN SET UP" << endl;

		testSystem.setValue< string >(imgPlugin, "logfile", "logfile.txt");
		testSystem.setup(imgPlugin);

		cout << "main: IMG PLUGIN SET UP" << endl;

		Identifier depth = testSystem.createService("depth generator", kinectPlugin, "Image Generator");
		testSystem.setValue< unsigned int >(depth, "device id", (unsigned int)0);
		testSystem.setValue< string >(depth, "image type", "color");
		testSystem.setup(depth);

		cout << "main: DEPTH SERVICE SET UP" << endl;

		testSystem.start(depth);
		testSystem.setUpdateRate(depth, 100.0f);

		cout << "main: DEPTH SERVICE STARTED" << endl;

		Identifier avg = testSystem.createService("image accumulation", imgPlugin, "ImageAccumulation_uchar");
		testSystem.setValue< unsigned char >(avg, "buffer size", unsigned char(10));
		testSystem.setup(avg);

		cout << "main: AVG SERVICE SET UP" << endl;

		testSystem.linkSlots(avg, "input image", depth, "output image");

		testSystem.start(avg);
		testSystem.setUpdateRate(avg, 100.0f);

		cout << "main: AVG STARTED" << endl;

		testSystem.registerToNewData(avg, "output image", ::imgDataAvailable);

		run = true;
		SDL_Event ev;
		while(run)
		{
			while(SDL_PollEvent(&ev)) 
			{
				processEvent(&ev);
			}

			loop();
			render();

			SDL_GL_SwapWindow(mainwindow);
		}

		testSystem.stop(depth);

		cout << "main: DEPTH SERVICE STOPPED" << endl;

		testSystem.stop(avg);

		cout << "main: AVG SERVICE STOPPED" << endl << endl;

		//test system falls out of scope here
		//->services are deleted, plugins uninstalled
	}
	catch (_2Real::Exception &e)
	{
		cout << e.message() << endl;
	}
	catch (std::exception &e)
	{
		cout << e.what() << endl;
	}

	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();

	while(1)
	{
		string line;
		char lineEnd = '\n';
		getline(cin, line, lineEnd);
		if (line == "quit")
		{
			break;
		}
	}

	return 0;

	//engine is destroyed here
}