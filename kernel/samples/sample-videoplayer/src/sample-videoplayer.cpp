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



// This plugin is using OpenCV to read a video file and output it's frames as highgui (which does not work) and soon pixelbuffers.

#include "_2RealSystem.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"
#include "_2RealData.h"

#include <windows.h>
#include <iostream>
#include <map>

#include "_2RealImagebuffer.h"

using namespace _2Real;
using namespace std;

string path = "D:\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";

void imagedatacallback(Data &data)
{
	//get this into a texture 
	data.getData< Buffer2D_uchar >();
}

void framedatacallback(Data &data)
{
	cout << data.getData< unsigned int >() << endl;
}

int main(int argc, char *argv[])
{
	//what's the purpose of the argument??
		System videoSystem("not needed");

		cout << "main: trying to load PLUGINS" << endl;

#ifdef _DEBUG
		//load the plugins: last argument needs to fit xml name
		Identifier videoPlugin = videoSystem.loadPlugin("(totally) useless name", path, "OpenCVvideoplayer_d.dll", "OpenCVvideoplayer");

		cout << "main: load videoPLUGIN" << endl;

		Identifier displayPlugin = videoSystem.loadPlugin("(totally) useless name", path, "ImageViewer_d.dll", "ImageViewer");
#else
		Identifier videoPlugin = videoSystem.loadPlugin("OpenCVvideoplayer", path, "OpenCVvideoplayer.dll", "OpenCVvideoplayer");
		Identifier displayPlugin = videoSystem.loadPlugin("(totally) useless name", path, "ImageViewer.dll", "ImageViewer");
#endif

		cout << "main: PLUGINS LOADED" << endl << endl;

		//maybe not write to std::out?
		videoSystem.dumpPluginInfo(videoPlugin);
		videoSystem.dumpPluginInfo(displayPlugin);

		//just testing, not really used...
		videoSystem.setParameterValue< string >(videoPlugin, "first parameter", "to who it may concern: please use me!");

		cout << "main: SETUP PARAMS SET" << endl << endl;

		videoSystem.startPlugin(videoPlugin);
		videoSystem.startPlugin(displayPlugin);

		cout << "main: PLUGIN STARTED" << endl;

		//arguments: some name, name ofpugin, name of service 
		Identifier playvideo = videoSystem.createService("very useful OpenCVvideoplayer", videoPlugin, "playvideo");

		cout << "main: playvideo SETUP PARAMS SET" << endl;

		// starte service
		videoSystem.start(playvideo);
		cout << "main: playvideo SERVICE STARTED" << endl;

		videoSystem.registerToNewData(playvideo, "output image", ::imagedatacallback);
		//videoSystem.registerToNewData(playvideo, "framenumber", ::framedatacallback);

		cout << "main: showvideo LISTENER REGISTERED" << endl;


		videoSystem.linkSlots(displayPlugin, "input image", videoPlugin, "output image");

		cout << "main: AVG & DEPTH LINKED" <<  endl;

		//videoSystem.registerToNewData(videoPlugin, "output image", ::imgDataAvailable);


		//la vita e bella
		while(1)
		{
			string line;
			char lineEnd = '\n';
			getline(cin, line, lineEnd);
			if (line == "q")
			{
				break;
			}
		}

		videoSystem.stop(playvideo);

		cout << "main: showvideo SERVICE STOPPED" << endl << endl;

		return 0;
	}