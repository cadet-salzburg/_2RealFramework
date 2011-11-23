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
#include "_2RealData.h"

#include "GL/glew.h"
#include "GL/wglew.h"
#include "SDL.h"

#include <windows.h>
#include <iostream>
#include <map>

#include "_2RealImagebuffer.h"

using namespace _2Real;
using namespace std;

/**
*	stuff
*/

unsigned int outID;
string path = "D:\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";
//string path = "C:\\Users\\Gigabyte\\Desktop\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";
bool run;
Buffer2D_float tmp;
Buffer2D_float buffer;
GLuint tex;

void depthDataAvailable(Data &data)
{
}

void imgDataAvailable(Data &data)
{
	tmp = data.getData< Buffer2D_float >();
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
	buffer = tmp;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(-1.0f, -1.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480, 0, GL_RGB, GL_FLOAT, (const GLvoid*)buffer.rawData());
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
			throw Exception("glew error");
		}

		SDL_GL_SetSwapInterval(1);

		init();

		Identifiers setup;
		Identifiers output;

		System testSystem("TEST");

#ifdef _DEBUG
		Identifier kinectPlugin = testSystem.loadPlugin("KINECT", path, "MultiKinectOpenNI_d.dll", "MultiKinect");
		Identifier imgPlugin = testSystem.loadPlugin("IMG", path, "ImageProcessing_d.dll", "ImageProcessing");
#else
		Identifier kinectPlugin = testSystem.loadPlugin("KINECT", path, "MultiKinectOpenNI.dll", "MultiKinectOpenNI");
		Identifier imgPlugin = testSystem.loadPlugin("IMG", path, "ImageProcessing.dll", "ImageProcessing");
#endif

		cout << "main: PLUGINS LOADED" << endl;

		testSystem.dumpPluginInfo(kinectPlugin);
		testSystem.dumpPluginInfo(imgPlugin);
		testSystem.dumpServiceInfo(kinectPlugin, "Image Generator");
		testSystem.dumpServiceInfo(imgPlugin, "ImageAccumulation_uchar");
		testSystem.dumpServiceInfo(imgPlugin, "ToFloat_uchar");

		vector< string > genFlags;
		genFlags.push_back("color");
		genFlags.push_back("depth");
		genFlags.push_back("user colored");

		vector< string > imgFlags;
		imgFlags.push_back("color640X480");
		imgFlags.push_back("depth640X480");
		imgFlags.push_back("mirrored");

		testSystem.setParameterValue< bool >(kinectPlugin, "align color depth", true);
		testSystem.setParameterValue< vector < string > >(kinectPlugin, "generator flags", genFlags);
		testSystem.setParameterValue< vector < string > >(kinectPlugin, "image flags", imgFlags);
		testSystem.setParameterValue< string >(kinectPlugin, "log file", std::string("kinectwrapper.txt"));
		testSystem.setParameterValue< string >(kinectPlugin, "log level", std::string("info"));

		cout << "main: KINECT PLUGIN SETUP PARAMS SET" << endl;

		testSystem.startPlugin(kinectPlugin);

		cout << "main: KINECT PLUGIN STARTED" << endl;

		testSystem.startPlugin(imgPlugin);

		cout << "main: IMG PLUGIN STARTED" << endl;

		Identifier depth = testSystem.createService("DEPTH", kinectPlugin, "Image Generator");

		cout << "main: DEPTH SERVICE CREATED" << endl;

		testSystem.setParameterValue< unsigned int >(depth, "device id", (unsigned int)0);
		testSystem.setParameterValue< string >(depth, "image type", string("color"));

		cout << "main: DEPTH SERVICE SETUP PARAMS SET" << endl;

		//testSystem.registerToNewData(depth, "output image", ::depthDataAvailable);

		//cout << "main: DEPTH LISTENER REGISTERED" << endl;

		testSystem.start(depth);

		cout << "main: DEPTH SERVICE STARTED" << endl;

		Identifier avg = testSystem.createService("AVG", imgPlugin, "ImageAccumulation_uchar");

		cout << "main: AVG SERVICE CREATED" << endl;

		testSystem.setParameterValue< unsigned char >(avg, "buffer size", unsigned char(50));

		cout << "main: AVG SERVICE SETUP PARAMS SET" << endl;

		testSystem.linkSlots(avg, "input image", depth, "output image");

		cout << "main: AVG & DEPTH LINKED" <<  endl;

		testSystem.registerToNewData(avg, "output image", ::imgDataAvailable);

		cout << "main: AVG LISTENER REGISTERED" << endl;

		testSystem.start(avg);

		cout << "main: AVG STARTED" << endl;

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

		cout << "main: DEPTH SERVICE STOPPED" << endl << endl;

		testSystem.stop(avg);

		cout << "main: AVG SERVICE STOPPED" << endl << endl;

		//test system falls out of scope here
		//->services are deleted
	}
	catch (_2Real::Exception &e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "an exception occurred" << endl;
	}

	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();

	while(1)
	{
		unsigned char thingie;
		std::cin >> thingie;
		if (thingie == 'a')
		{
			break;
		}
	}

	return 0;

	//engine dtor is called
	//plugins are uninstalled.
}