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

#include "_2RealEngine.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"

#include <windows.h>

#include <list>

using namespace _2Real;

std::string path = "D:\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";

void main(int argc, char** argv)
{

	/**
	*	get instance of 2 real thingie
	*/
	Engine testEngine("my test engine sucks");

	/**
	*	load a test plugin
	*	get identifiers of all services
	*/
	std::list< Identifier > ids;
	Identifier plugin = testEngine.loadPlugin("my test plugin sucks more", path, "ImageProcessing_d.dll", "ImageProcessing", ids);

	/**
	*	print plugin infos
	*/
	std::cout << "plugin info by id " << plugin.id() << std::endl;
	testEngine.dumpPluginInfo(plugin);

	/**
	*	print service infos twice:
	*	-by service id
	*	-by plugin id + service name
	*/
	for (std::list< Identifier >::iterator it = ids.begin(); it != ids.end(); it++)
	{
		std::cout << "service info by id: " << it->id() << std::endl;
		testEngine.dumpServiceInfo(*it);
		std::cout << "service info by plugin-id: " << plugin.id() << " & name: " << it->name() << std::endl;
		testEngine.dumpServiceInfo(plugin, it->name());
	}

	/**
	*	create a service by plugin id + service name
	*	also returns setup params
	*/
	std::list< Identifier > setupIDs1;
	Identifier rand1 = testEngine.createService("random image service 1", plugin, "RandomImage2D_float", setupIDs1);

	std::cout << "created service: " << rand1.name() << std::endl;
	std::cout << rand1.info() << std::endl;
	for (Identifiers::iterator it = setupIDs1.begin(); it != setupIDs1.end(); it++)
	{
		std::cout << "returned setup param: " << it->name() << std::endl;
	}

	/**
	*	get input params
	*/
	//std::list< Identifier > inputIDs1 = testEngine.getInputSlots(rand1);
	//for (Identifiers::iterator it = setupIDs1.begin(); it != setupIDs1.end(); it++)
	//{
	//	std::cout << "returned input param: " << it->name() << std::endl;
	//}

	/**
	*	get output params
	*/
	//std::list< Identifier > outputIDs1 = testEngine.getOutputSlots(rand1);
	//for (Identifiers::iterator it = setupIDs1.begin(); it != setupIDs1.end(); it++)
	//{
	//	std::cout << "returned output param: " << it->name() << std::endl;
	//}

	/**
	*	create second service
	*/
	//std::list< Identifier > setupIDs2;
	//Identifier rand2 = testEngine.createService("random image service 2", plugin, "RandomImage_float", setupIDs1);
	//std::list< Identifier > inputIDs2 = testEngine.getInputSlots(rand2);
	//std::list< Identifier > outputIDs2 = testEngine.getOutputSlots(rand2);

	Sleep(100000);

}