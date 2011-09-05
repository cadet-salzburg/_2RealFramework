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

/**
*	test callback for exceptions in container
*/
void exceptionOccured(Identifier const& _sender, Exception const& _exception)
{
	std::cout << "an exception occured" << std::endl;
}

/**
*	test callback for data available from container
*/
void dataAvailable(Identifier const& _sender, Data const& _data)
{
	std::cout << "new data available" << std::endl;
}

/**
*	querying every little shit is kinda unnecessary
*	but i'm doing it anyway
*/
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
	//std::cout << rand1.info() << std::endl;
	for (Identifiers::iterator it = setupIDs1.begin(); it != setupIDs1.end(); it++)
	{
		std::cout << "returned setup param: " << it->name() << std::endl;
	}

	/**
	*	get input params
	*/
	std::list< Identifier > inputIDs1 = testEngine.getInputSlots(rand1);
	for (Identifiers::iterator it = inputIDs1.begin(); it != inputIDs1.end(); it++)
	{
		std::cout << "returned input param: " << it->name() << std::endl;
	}

	/**
	*	get output params
	*/
	std::list< Identifier > outputIDs1 = testEngine.getOutputSlots(rand1);
	for (Identifiers::iterator it = outputIDs1.begin(); it != outputIDs1.end(); it++)
	{
		std::cout << "returned output param: " << it->name() << std::endl;
	}

	std::cout << std::endl;

	/**
	*	create second service
	*/
	std::list< Identifier > setupIDs2;
	Identifier rand2 = testEngine.createService("random image service 2", plugin, "RandomImage2D_float", setupIDs2);
	std::cout << "created service: " << rand2.name() << std::endl;
	//std::cout << rand2.info() << std::endl;
	for (Identifiers::iterator it = setupIDs2.begin(); it != setupIDs2.end(); it++)
	{
		std::cout << "returned setup param: " << it->name() << std::endl;
	}
	std::list< Identifier > inputIDs2 = testEngine.getInputSlots(rand2);
	std::list< Identifier > outputIDs2 = testEngine.getOutputSlots(rand2);
	for (Identifiers::iterator it = inputIDs2.begin(); it != inputIDs2.end(); it++)
	{
		std::cout << "returned input param: " << it->name() << std::endl;
	}
	for (Identifiers::iterator it = outputIDs2.begin(); it != outputIDs2.end(); it++)
	{
		std::cout << "returned output param: " << it->name() << std::endl;
	}

	std::cout << std::endl;

	/**
	*	create third service
	*/
	std::list< Identifier > setupIDs3;
	Identifier add = testEngine.createService("addition service", plugin, "ImageAddition2D_float", setupIDs3);
	std::cout << "created service: " << add.name() << std::endl;
	//std::cout << add.info() << std::endl;
	for (Identifiers::iterator it = setupIDs3.begin(); it != setupIDs3.end(); it++)
	{
		std::cout << "returned setup param: " << it->name() << std::endl;
	}
	std::list< Identifier > inputIDs3 = testEngine.getInputSlots(add);
	std::list< Identifier > outputIDs3 = testEngine.getOutputSlots(add);
	for (Identifiers::iterator it = inputIDs3.begin(); it != inputIDs3.end(); it++)
	{
		std::cout << "returned input param: " << it->name() << std::endl;
	}
	for (Identifiers::iterator it = outputIDs3.begin(); it != outputIDs3.end(); it++)
	{
		std::cout << "returned output param: " << it->name() << std::endl;
	}

	std::cout << std::endl;

	/**
	*	synchronize rand1 & rand2
	*/
	Identifier sync = testEngine.createSynchronizationContainer("man this really stinks", rand1, rand2);
	std::cout << "created syncronization " << sync.name() << std::endl;

	/**
	*	query output params & input params
	*	input params should be empty (rand img has no input)
	*/
	std::list< Identifier > inputIDs4 = testEngine.getInputSlots(sync);
	std::list< Identifier > outputIDs4 = testEngine.getOutputSlots(sync);
	for (Identifiers::iterator it = inputIDs4.begin(); it != inputIDs4.end(); it++)
	{
		std::cout << "returned input param: " << it->name() << std::endl;
	}
	for (Identifiers::iterator it = outputIDs4.begin(); it != outputIDs4.end(); it++)
	{
		std::cout << "returned output param: " << it->name() << std::endl;
	}

	std::cout << std::endl;

	/**
	*	create sequence of sync & add
	*/
	Identifier seq = testEngine.createSequenceContainer("yay, my life sucks less than this program", sync, add);
	std::cout << "created sequence " << seq.name() << std::endl;


	/**
	*	query input & output params
	*	the input params are empty, obviously
	*	('sync' is first in sequence, and has no input)
	*	the output params are the output params of 'add'
	*	!!!
	*	also, creating a sequence will link input / output slots!
	*	(and throw an exception if it does not work, i.e. b/c the slots do not match)
	*/
	std::list< Identifier > inputIDs5 = testEngine.getInputSlots(seq);
	std::list< Identifier > outputIDs5 = testEngine.getOutputSlots(seq);
	for (Identifiers::iterator it = inputIDs5.begin(); it != inputIDs5.end(); it++)
	{
		std::cout << "returned input param: " << it->name() << std::endl;
	}
	for (Identifiers::iterator it = outputIDs5.begin(); it != outputIDs5.end(); it++)
	{
		std::cout << "returned output param: " << it->name() << std::endl;
	}

	std::cout << std::endl;

	/**
	*	initialize random image setup params
	*/
	unsigned int w = 400;
	unsigned int h = 400;
	for (Identifiers::iterator it = setupIDs1.begin(); it != setupIDs1.end(); it++)
	{
		testEngine.setParameterValue(*it, w);
	}

	for (Identifiers::iterator it = setupIDs2.begin(); it != setupIDs2.end(); it++)
	{
		testEngine.setParameterValue(*it, w);
	}

	/**
	*	initialize addition params
	*/
	//TODO: change identifiers from list to vector so that they can be accessed by []
	for (Identifiers::iterator it = setupIDs3.begin(); it != setupIDs3.end(); it++)
	{
		testEngine.setParameterValue(*it, 1.0f);
	}

	//testEngine.registerToException(seq, exceptionOccured);
	//testEngine.registerToNewData(seq, dataAvailable);
	std::list< Identifier > children1 = testEngine.getChildren(sync);
	std::list< Identifier > children2 = testEngine.getChildren(seq);
	for (Identifiers::iterator it = children1.begin(); it != children1.end(); it++)
	{
		std::cout << it->name() << std::endl;
	}
	for (Identifiers::iterator it = children2.begin(); it != children2.end(); it++)
	{
		std::cout << it->name() << std::endl;
	}

	testEngine.start(seq);

	Sleep(100000);

}