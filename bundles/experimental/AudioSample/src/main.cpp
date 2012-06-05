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

/*
* This tutorial application demonstrates the basic use of the Tutorial Plugin and its services.
* The sample uses a Counter service and prints its values out via another service.
* It also shows how the counter values are multiplied by 2 and then printed out again.
* 
*			TERMINATE THE PROGRAM WITH "q+ ENTER/RETURN"
*/

#include "_2RealEngine.h"
#include "_2RealSystem.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"
#include "_2RealData.h"
#include "_2RealUpdatePolicy.h"
#include "_2RealImageT.h"

#include "Poco/Mutex.h"

#include <windows.h>
#include <iostream>

Poco::Mutex m;

#ifndef _DEBUG
	#define shared_library_suffix "_32.dll"
#else
	#define shared_library_suffix "_32d.dll"
#endif

using namespace _2Real;
using namespace std;

void newData( void *userData, Data &data )
{
	m.lock();
	std::vector< unsigned char > const& vec = data.getData< std::vector< unsigned char > >();
	for ( unsigned int i=0; i<vec.size(); ++i )
	{
		std::cout << vec[i] << " ";
	}
	std::cout << "\n";
	m.unlock();
}

int main(int argc, char *argv[])
{
	Engine &TutorialEngine = Engine::instance();
	System tutorialSystem("Tutorial System");

	try 
	{
		list< string > classNames;

		TutorialEngine.setBaseDirectory( "..\\bin\\win" );

		string tut = "AudioBundle";
		tut.append(shared_library_suffix);

		const BundleIdentifier tutPlugin = TutorialEngine.load( tut );

		UpdatePolicy fpsTrigger;
		fpsTrigger.triggerByUpdateRate(10.0f);

		BlockIdentifier midiInput = tutorialSystem.createBlock(tutPlugin, "AudioOutputService", fpsTrigger);
		tutorialSystem.setup(midiInput);

		tutorialSystem.registerToNewData(midiInput, "test", newData );

	}
	catch (_2Real::Exception &e)
	{
		cout << e.message() << endl;
	}
	catch (std::exception &e)
	{
		cout << e.what() << endl;
	}

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

	tutorialSystem.clear();

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

	return 0;
}