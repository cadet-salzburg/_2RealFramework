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

#include <list>
#include <iostream>

#include "Image.h"

using namespace _2Real;

unsigned int outID;

//path to test plugins
std::string path = "D:\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";
//std::string path = "C:\\Users\\Gigabyte\\Desktop\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";

void dataAvailable(Data &_data)
{
	std::cout << "new data available from " << _data.id().name() << std::endl;
	//Image< float, 2 > img = _data.get< Image< float, 2 > >(outID);
}

void main(int argc, char** argv)
{
	Image< float, 2 > img;
	try
	{
		//some identifier vectors for querying stuff
		Identifiers setup;
		Identifiers input;
		Identifiers output;
		Identifiers children;

		//some setup params
		unsigned int width = 640;
		unsigned int heigth = 480;
		float f1 = 1.0f;
		float f2 = 2.0f;
		float f3 = 4.0f;

		//create a system
		System testSystem("TEST");
		Identifier sys = testSystem.getID();

#ifdef _DEBUG
		Identifier plugin = testSystem.loadPlugin("IMG_PROC", path, "ImageProcessing_d.dll", "ImageProcessing");
#else
		Identifier plugin = testSystem.loadPlugin("IMG_PROC", path, "ImageProcessing.dll", "ImageProcessing");
#endif

		std::cout << "plugin info:" << std::endl;
		testSystem.dumpPluginInfo(plugin);

		setup = testSystem.getSetupParameters(plugin);

		std::vector< std::string > vec;
		vec.push_back("this is a setup parameter.");
		vec.push_back("and this one too.");
		vec.push_back("and they both suck.");

		testSystem.setParameterValue(setup[0], vec);

		testSystem.startPlugin(plugin);

		//TODO: check if service was actually exported
		std::cout << "service info:" << std::endl;
		testSystem.dumpServiceInfo(plugin, "RandomImage2D_float");
		std::cout << "service info:" << std::endl;
		testSystem.dumpServiceInfo(plugin, "ImageAddition2D_float");
		std::cout << "service info:" << std::endl;
		testSystem.dumpServiceInfo(plugin, "ImageScaling2D_float");

		//create some services
		//& initialize them
		Identifier scale1 = testSystem.createService("SCALE_1", plugin, "ImageScaling2D_float");
		setup = testSystem.getSetupParameters(scale1);
		//testSystem.setParameterValue(setup[0], f2);

		//Identifier scale2 = testSystem.createService("SCALE_2", plugin, "ImageScaling2D_float");
		//setup = testSystem.getSetupParameters(scale2);
		//testSystem.setParameterValue(setup[0], f3);

		//Identifier rand1 = testSystem.createService("RAND_1", plugin, "RandomImage2D_float");
		//setup = testSystem.getSetupParameters(rand1);
		//testSystem.setParameterValue(setup[0], width);
		//testSystem.setParameterValue(setup[1], heigth);

		//Identifier rand2 = testSystem.createService("RAND_2", plugin, "RandomImage2D_float");
		//setup = testSystem.getSetupParameters(rand2);
		//testSystem.setParameterValue(setup[0], width);
		//testSystem.setParameterValue(setup[1], heigth);

		//Identifier add = testSystem.createService("ADD", plugin, "ImageAddition2D_float");
		//setup = testSystem.getSetupParameters(add);
		//testSystem.setParameterValue(setup[0], f1);
		//testSystem.setParameterValue(setup[1], f1);

		//children = testSystem.getChildren(sys);
		//std::cout << "system's children: ";
		//for (std::vector< Identifier >::iterator it = children.begin(); it != children.end(); it++)
		//{
		//	std::cout << (*it).name() << " ";
		//}
		//std::cout << std::endl;
		//std::cout << std::endl;

		////make a sync of rand 1 and rand 2
		//Identifier sync = testSystem.createSynchronization("SYNC", rand1, rand2);

		//children = testSystem.getChildren(sys);
		//std::cout << "system's children: ";
		//for (std::vector< Identifier >::iterator it = children.begin(); it != children.end(); it++)
		//{
		//	std::cout << (*it).name() << " ";
		//}
		//std::cout << std::endl;
		//children = testSystem.getChildren(sync);
		//std::cout << "sync's children:   ";
		//for (std::vector< Identifier >::iterator it = children.begin(); it != children.end(); it++)
		//{
		//	std::cout << (*it).name() << " ";
		//}
		//std::cout << std::endl;
		//std::cout << std::endl;
		//
		////make a sequence of sync & add
		//Identifier seq = testSystem.createSequence("SEQ", sync, add);

		//children = testSystem.getChildren(sys);
		//std::cout << "system's children: ";
		//for (std::vector< Identifier >::iterator it = children.begin(); it != children.end(); it++)
		//{
		//	std::cout << (*it).name() << " ";
		//}
		//std::cout << std::endl;
		//children = testSystem.getChildren(sync);
		//std::cout << "sync's children:   ";
		//for (std::vector< Identifier >::iterator it = children.begin(); it != children.end(); it++)
		//{
		//	std::cout << (*it).name() << " ";
		//}
		//std::cout << std::endl;
		//children = testSystem.getChildren(seq);
		//std::cout << "seq's children:    ";
		//for (std::vector< Identifier >::iterator it = children.begin(); it != children.end(); it++)
		//{
		//	std::cout << (*it).name() << " ";
		//}
		//std::cout << std::endl;
		//std::cout << std::endl;
		//
		////put scale1 into seq as last element
		//testSystem.append(seq, scale1);
		////this command would have the same result
		////testSystem.insert(seq, 2, scale);

		//children = testSystem.getChildren(sys);
		//std::cout << "system's children: ";
		//for (std::vector< Identifier >::iterator it = children.begin(); it != children.end(); it++)
		//{
		//	std::cout << (*it).name() << " ";
		//}
		//std::cout << std::endl;
		//children = testSystem.getChildren(sync);
		//std::cout << "sync's children:   ";
		//for (std::vector< Identifier >::iterator it = children.begin(); it != children.end(); it++)
		//{
		//	std::cout << (*it).name() << " ";
		//}
		//std::cout << std::endl;
		//children = testSystem.getChildren(seq);
		//std::cout << "seq's children:    ";
		//for (std::vector< Identifier >::iterator it = children.begin(); it != children.end(); it++)
		//{
		//	std::cout << (*it).name() << " ";
		//}
		//std::cout << std::endl;
		//std::cout << std::endl;

		////link the output of sequence with the input of scale2
		////assuming they match
		//testSystem.link(scale2, seq);

		////register a callback
		//output = testSystem.getOutputSlots(seq);
		//outID = output[0].id();
		//testSystem.registerToNewData(scale2, ::dataAvailable);

		while (1)
		{
			char thingie;
			std::cin >> thingie;
			//if (thingie == 'a')
			//{
			//	//starts seq only
			//	testSystem.start(seq);
			//}
			//else if (thingie == 'b')
			//{
			//	//stops seq
			//	testSystem.stop(seq);
			//}
			//else if (thingie == 'c')
			//{
			//	//starts scale 2 only
			//	//this alone will not produce an output
			//	//as seq is needed as well
			//	testSystem.start(scale2);
			//}
			//else if (thingie == 'd')
			//{
			//	//stops scale 2
			//	testSystem.stop(scale2);
			//}
			//else if (thingie == 'e')
			//{
			//	//starts both seq & scale 2
			//	testSystem.startAll();
			//}
			//else if (thingie == 'f')
			//{
			//	//stops everything
			//	testSystem.stopAll();
			//}
			if (thingie == 's')
			{
				break;
			}
		}
	}
	catch (Exception &e)
	{
		std::cout << e.what() << std::endl;

		while (1)
		{
			char thingie;
			std::cin >> thingie;
			if (thingie == 's')
			{
				break;
			}
		}
	}
}