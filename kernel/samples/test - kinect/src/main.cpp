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
#include "_2RealPixelbuffer.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "SDL.h"

#include <windows.h>
#include <iostream>
#include <map>

#include "Poco\Mutex.h"

#include <vld.h>

using namespace _2Real;
using namespace std;
using namespace Poco;

bool run;
Buffer2D_float tmp;
Buffer2D_float buffer;
GLuint tex;
FastMutex mutex;

/**
*	tracks updated image data
*/
void imgDataAvailable(Data &data)
{
	FastMutex::ScopedLock lock(mutex);
	tmp = data.getData< Buffer2D_float >();
}

/**
*	tracks exceptions in the system
*/
void systemException(RunnableError &e)
{
	std::cout << "exception in " << e.system().name() << " by " << e.sender().name() << std::endl;
}

/**
*	for testing purposes, tracks the state changes of the color image service
*/
void stateChanged(std::string &newState)
{
	std::cout << "image generator state changed to : " << newState << std::endl;
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
		testSystem.setInstallDirectory("D:\\cadet\\trunk\\_2RealFramework\\kernel\\samples\\testplugins\\bin\\");
		testSystem.setLogfile("testsystem.txt");

		Identifier kinectPlugin = testSystem.loadPlugin("kinect plugin", "MultiKinectOpenNI");

		std::string kinectInfo = testSystem.getInfo(kinectPlugin);
		std::cout << kinectInfo << std::endl;

		Identifier imgPlugin = testSystem.loadPlugin("image processing plugin", "ImageProcessing");

		std::string imgInfo = testSystem.getInfo(imgPlugin);
		std::cout << imgInfo << std::endl;

		vector< string > genFlags;
		genFlags.push_back("color");
		genFlags.push_back("depth");
		genFlags.push_back("user colored");

		vector< string > imgFlags;
		imgFlags.push_back("color640X480");
		imgFlags.push_back("depth640X480");
		imgFlags.push_back("mirrored");

		//testSystem.setValue< bool >(kinectPlugin, "align color depth", true);
		testSystem.setValue< vector < string > >(kinectPlugin, "generator flags", genFlags);
		testSystem.setValue< vector < string > >(kinectPlugin, "image flags", imgFlags);
		testSystem.setValue< string >(kinectPlugin, "logfile", "kinectwrapper.txt");
		testSystem.setValue< string >(kinectPlugin, "loglevel", "info");
		testSystem.setup(kinectPlugin);

		testSystem.setValue< string >(imgPlugin, "logfile", "logfile.txt");
		testSystem.setup(imgPlugin);

		Identifier img = testSystem.createService("img generator", kinectPlugin, "Image Generator");

		testSystem.registerToStateChange(img, ::stateChanged);

		//testSystem.setValue< unsigned int >(depth, "device id", (unsigned int)0);
		//testSystem.setValue< string >(depth, "image type", "color");
		testSystem.setup(img);

		Identifier avg0 = testSystem.createService("image accum 0", imgPlugin, "ImageAccumulation_uchar");
		testSystem.setup(avg0);

		Identifier avg1 = testSystem.createService("image accum 1", imgPlugin, "ImageAccumulation");
		testSystem.setup(avg1);

		Identifier avg2 = testSystem.createService("image accum 2", imgPlugin, "ImageAccumulation");
		testSystem.setup(avg2);

		Identifier avg3 = testSystem.createService("image accum 3", imgPlugin, "ImageAccumulation");
		testSystem.setup(avg3);

		Identifier avg4 = testSystem.createService("image accum 4", imgPlugin, "ImageAccumulation");
		testSystem.setup(avg4);

		Identifier avg5 = testSystem.createService("image accum 5", imgPlugin, "ImageAccumulation");
		testSystem.setup(avg5);

		Identifier avg6 = testSystem.createService("image accum 6", imgPlugin, "ImageAccumulation");
		testSystem.setup(avg6);

		testSystem.linkSlots(avg0, "input image", img, "output image");
		testSystem.linkSlots(avg1, "input image", avg0, "output image");
		testSystem.linkSlots(avg2, "input image", avg1, "output image");
		testSystem.linkSlots(avg3, "input image", avg2, "output image");
		testSystem.linkSlots(avg4, "input image", avg3, "output image");
		testSystem.linkSlots(avg5, "input image", avg4, "output image");
		testSystem.linkSlots(avg6, "input image", avg5, "output image");

		testSystem.setUpdateRate(avg0, 100.0f);
		testSystem.start(avg0);

		testSystem.setUpdateRate(avg1, 100.0f);
		testSystem.start(avg1);

		testSystem.setUpdateRate(avg2, 100.0f);
		testSystem.start(avg2);

		testSystem.setUpdateRate(avg3, 100.0f);
		testSystem.start(avg3);

		testSystem.setUpdateRate(avg4, 100.0f);
		testSystem.start(avg4);

		testSystem.setUpdateRate(avg5, 100.0f);
		testSystem.start(avg5);

		testSystem.setUpdateRate(avg6, 100.0f);
		testSystem.start(avg6);

		testSystem.setUpdateRate(img, 100.0f);
		testSystem.start(img);

		testSystem.registerToNewData(avg6, "output image", ::imgDataAvailable);

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

		testSystem.stop(img);
		testSystem.stop(avg0);
		testSystem.stop(avg1);
		testSystem.stop(avg2);
		testSystem.stop(avg3);
		testSystem.stop(avg4);
		testSystem.stop(avg5);

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