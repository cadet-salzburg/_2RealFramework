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

#include <list>
#include <iostream>

using namespace _2Real;

//std::string path = "D:\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";
std::string path = "C:\\Users\\Gigabyte\\Desktop\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";

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

void main(int argc, char** argv)
{
	try
	{
		//create a system
		System testSystem("TEST");

		Identifier sys = testSystem.getID();

		std::cout << sys.info() << std::endl;

		//load test plugin
#ifdef _DEBUG
	Identifier plugin = testSystem.loadPlugin("IMG_PROC", path, "ImageProcessing_d.dll", "ImageProcessing");
#else
	Identifier plugin = testSystem.loadPlugin("IMG_PROC", path, "ImageProcessing.dll", "ImageProcessing");
#endif

		//print plugin infos
		std::cout << "plugin info:" << std::endl;
		testSystem.dumpPluginInfo(plugin);

		//TODO function to query exported services by id / by name
		//right now 

		//printf service infos
		std::cout << "service info:" << std::endl;
		testSystem.dumpServiceInfo(plugin, "RandomImage2D_float");
		std::cout << "service info:" << std::endl;
		testSystem.dumpServiceInfo(plugin, "ImageAddition2D_float");

		//create a service
		Identifier rand1 = testSystem.createService("RAND_1", plugin, "RandomImage2D_float");
		std::cout << "created service: " << rand1.name() << std::endl;
		std::cout << rand1.info() << std::endl;

		//Identifiers setupRand1 = testSystem.getSetupParameters(rand1);
		//Identifiers inputRand1 = testSystem.getInputSlots(rand1);
		//Identifiers outputRand1 = testSystem.getOutputSlots(rand1);

		//for (Identifiers::iterator it = setupRand1.begin(); it != setupRand1.end(); it++)
		//{
		//	std::cout << "setup param: " << it->name() << std::endl;
		//}
		//for (Identifiers::iterator it = inputRand1.begin(); it != inputRand1.end(); it++)
		//{
		//	std::cout << "input slot: " << it->name() << std::endl;
		//}
		//for (Identifiers::iterator it = outputRand1.begin(); it != outputRand1.end(); it++)
		//{
		//	std::cout << "output slot: " << it->name() << std::endl;
		//}

		std::cout << std::endl;

		//create a service
		Identifier rand2 = testSystem.createService("RAND_2", plugin, "RandomImage2D_float");
		std::cout << "created service: " << rand2.name() << std::endl;
		std::cout << rand2.info() << std::endl;

		//Identifiers setupRand2 = testSystem.getSetupParameters(rand2);
		//Identifiers inputRand2 = testSystem.getInputSlots(rand2);
		//Identifiers outputRand2 = testSystem.getOutputSlots(rand2);

		//for (Identifiers::iterator it = setupRand2.begin(); it != setupRand2.end(); it++)
		//{
		//	std::cout << "setup param: " << it->name() << std::endl;
		//}
		//for (Identifiers::iterator it = inputRand2.begin(); it != inputRand2.end(); it++)
		//{
		//	std::cout << "input slot: " << it->name() << std::endl;
		//}
		//for (Identifiers::iterator it = outputRand2.begin(); it != outputRand2.end(); it++)
		//{
		//	std::cout << "output slot: " << it->name() << std::endl;
		//}

		std::cout << std::endl;

		//create a service
		Identifier add = testSystem.createService("ADD", plugin, "ImageAddition2D_float");
		std::cout << "created service: " << add.name() << std::endl;
		std::cout << add.info() << std::endl;

		//Identifiers setupRand2 = testSystem.getSetupParameters(rand2);
		//Identifiers inputRand2 = testSystem.getInputSlots(rand2);
		//Identifiers outputRand2 = testSystem.getOutputSlots(rand2);

		//for (Identifiers::iterator it = setupRand2.begin(); it != setupRand2.end(); it++)
		//{
		//	std::cout << "setup param: " << it->name() << std::endl;
		//}
		//for (Identifiers::iterator it = inputRand2.begin(); it != inputRand2.end(); it++)
		//{
		//	std::cout << "input slot: " << it->name() << std::endl;
		//}
		//for (Identifiers::iterator it = outputRand2.begin(); it != outputRand2.end(); it++)
		//{
		//	std::cout << "output slot: " << it->name() << std::endl;
		//}

		std::cout << std::endl;

//	/**
//	*	create third service
//	*/
//	std::list< Identifier > setupIDs3;
//	Identifier add = testEngine.createService("ADD", plugin, "ImageAddition2D_float", setupIDs3);
//	std::cout << "created service: " << add.name() << std::endl;
//	//std::cout << add.info() << std::endl;
//	for (Identifiers::iterator it = setupIDs3.begin(); it != setupIDs3.end(); it++)
//	{
//		std::cout << "returned setup param: " << it->name() << std::endl;
//	}
//	std::list< Identifier > inputIDs3 = testEngine.getInputSlots(add);
//	std::list< Identifier > outputIDs3 = testEngine.getOutputSlots(add);
//	for (Identifiers::iterator it = inputIDs3.begin(); it != inputIDs3.end(); it++)
//	{
//		std::cout << "returned input param: " << it->name() << std::endl;
//	}
//	for (Identifiers::iterator it = outputIDs3.begin(); it != outputIDs3.end(); it++)
//	{
//		std::cout << "returned output param: " << it->name() << std::endl;
//	}
//
//	std::cout << std::endl;
//
//	/**
//	*	synchronize rand1 & rand2
//	*/
//	Identifier sync = testEngine.createSynchronizationContainer("SYNC", rand1, rand2);
//	std::cout << "created syncronization " << sync.name() << std::endl;
//
//	/**
//	*	query output params & input params
//	*	input params should be empty (rand img has no input)
//	*/
//	std::list< Identifier > inputIDs4 = testEngine.getInputSlots(sync);
//	std::list< Identifier > outputIDs4 = testEngine.getOutputSlots(sync);
//	for (Identifiers::iterator it = inputIDs4.begin(); it != inputIDs4.end(); it++)
//	{
//		std::cout << "returned input param: " << it->name() << std::endl;
//	}
//	for (Identifiers::iterator it = outputIDs4.begin(); it != outputIDs4.end(); it++)
//	{
//		std::cout << "returned output param: " << it->name() << std::endl;
//	}
//
//	std::cout << std::endl;
//
//	/**
//	*	create sequence of sync & add
//	*/
//	Identifier seq = testEngine.createSequenceContainer("SEQ", sync, add);
//	std::cout << "created sequence " << seq.name() << std::endl;
//
//
//	/**
//	*	query input & output params
//	*	the input params are empty, obviously
//	*	('sync' is first in sequence, and has no input)
//	*	the output params are the output params of 'add'
//	*	!!!
//	*	also, creating a sequence will link input / output slots!
//	*	(and throw an exception if it does not work, i.e. b/c the slots do not match)
//	*/
//	std::list< Identifier > inputIDs5 = testEngine.getInputSlots(seq);
//	std::list< Identifier > outputIDs5 = testEngine.getOutputSlots(seq);
//	for (Identifiers::iterator it = inputIDs5.begin(); it != inputIDs5.end(); it++)
//	{
//		std::cout << "returned input param: " << it->name() << std::endl;
//	}
//	for (Identifiers::iterator it = outputIDs5.begin(); it != outputIDs5.end(); it++)
//	{
//		std::cout << "returned output param: " << it->name() << std::endl;
//	}
//
//	std::cout << std::endl;
//
//	/**
//	*	initialize random image setup params
//	*/
//	unsigned int w = 400;
//	unsigned int h = 400;
//	for (Identifiers::iterator it = setupIDs1.begin(); it != setupIDs1.end(); it++)
//	{
//		testEngine.setParameterValue(*it, w);
//	}
//
//	for (Identifiers::iterator it = setupIDs2.begin(); it != setupIDs2.end(); it++)
//	{
//		testEngine.setParameterValue(*it, w);
//	}
//
//	/**
//	*	initialize addition params
//	*/
//	//TODO: change identifiers from list to vector so that they can be accessed by []
//	for (Identifiers::iterator it = setupIDs3.begin(); it != setupIDs3.end(); it++)
//	{
//		testEngine.setParameterValue(*it, 1.0f);
//	}
//
//	std::list< Identifier > children1 = testEngine.getChildren(sync);
//	std::list< Identifier > children2 = testEngine.getChildren(seq);
//	for (Identifiers::iterator it = children1.begin(); it != children1.end(); it++)
//	{
//		std::cout << it->name() << std::endl;
//	}
//	for (Identifiers::iterator it = children2.begin(); it != children2.end(); it++)
//	{
//		std::cout << it->name() << std::endl;
//	}
//
//	testEngine.start(seq);
//
//	//testEngine.registerToException(seq, exceptionOccured);
//	//testEngine.registerToNewData(seq, dataAvailable);
//
//	while (1)
//	{
//		char end;
//		std::cin >> end;
//		if (end == 'b')
//		{
//			break;
//		}
//	}
//
//	testEngine.stop(seq);
//	std::cout << "stopped " << seq.name() << std::endl;
//
		while (1)
		{
			char end;
			std::cin >> end;
			if (end == 's')
			{
				break;
			}
		}
	}
	catch (Exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}