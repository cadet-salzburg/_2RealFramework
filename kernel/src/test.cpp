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

unsigned int outID1;
unsigned int outID2;

//path to test plugins
std::string path = "D:\\cadet\\trunk\\_2RealFramework\\kernel\\testplugins\\bin\\";

void exceptionOccured(Identifier &_id)
{
	std::cout << "an exception occured in " << _id.name() << std::endl;
}

void dataAvailable1(Data &_data)
{
	std::cout << "new data available from " << _data.id().name() << std::endl;
	Image< float, 2 > img = _data.get< Image< float, 2 > >(outID1);
	std::cout << img.data()[0] << std::endl;
}

void dataAvailable2(Data &_data)
{
	std::cout << "new data available from " << _data.id().name() << std::endl;
	Image< float, 2 > img = _data.get< Image< float, 2 > >(outID2);
	std::cout << img.data()[0] << std::endl;
}

void main(int argc, char** argv)
{
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

		////plugin infos
		//std::cout << "plugin info:" << std::endl;
		//testSystem.dumpPluginInfo(plugin);

		////some service infos
		//std::cout << "service info:" << std::endl;
		//testSystem.dumpServiceInfo(plugin, "RandomImage2D_float");
		//std::cout << "service info:" << std::endl;
		//testSystem.dumpServiceInfo(plugin, "ImageAddition2D_float");
		//std::cout << "service info:" << std::endl;
		//testSystem.dumpServiceInfo(plugin, "ImageScaling2D_float");

		//create some services
		Identifier scale1 = testSystem.createService("SCALE_1", plugin, "ImageScaling2D_float");
		setup = testSystem.getSetupParameters(scale1);
		testSystem.setParameterValue(setup[0], f2);

		Identifier scale2 = testSystem.createService("SCALE_2", plugin, "ImageScaling2D_float");
		setup = testSystem.getSetupParameters(scale2);
		testSystem.setParameterValue(setup[0], f3);

		Identifier rand1 = testSystem.createService("RAND_1", plugin, "RandomImage2D_float");
		setup = testSystem.getSetupParameters(rand1);
		testSystem.setParameterValue(setup[0], width);
		testSystem.setParameterValue(setup[1], heigth);

		Identifier rand2 = testSystem.createService("RAND_2", plugin, "RandomImage2D_float");
		setup = testSystem.getSetupParameters(rand2);
		testSystem.setParameterValue(setup[0], width);
		testSystem.setParameterValue(setup[1], heigth);

		Identifier add = testSystem.createService("ADD", plugin, "ImageAddition2D_float");
		setup = testSystem.getSetupParameters(add);
		testSystem.setParameterValue(setup[0], f1);
		testSystem.setParameterValue(setup[1], f1);

		//make a sync of rand 1 and rand 2
		Identifier sync = testSystem.createSynchronization("SYNC", rand1, rand2);
		//make a sequence of sync & add
		Identifier seq = testSystem.createSequence("SEQ", sync, add);
		//put scale1 into seq as last element
		testSystem.append(seq, scale1);
		//this command would have the same result
		//testSystem.insert(seq, 2, scale);

		//link the output of sequence with the input of scale2
		//assuming they match
		testSystem.link(scale2, seq);

		output = testSystem.getOutputSlots(seq);
		outID1 = output[0].id();

		output = testSystem.getOutputSlots(scale2);
		outID2 = output[0].id();

		testSystem.registerToNewData(seq, ::dataAvailable1);
		testSystem.registerToNewData(scale2, ::dataAvailable2);

		while (1)
		{
			char thingie;
			std::cin >> thingie;
			if (thingie == 'a')
			{
				testSystem.start(seq);
			}
			else if (thingie == 'b')
			{
				testSystem.stop(seq);
			}
			else if (thingie == 'c')
			{
				testSystem.start(scale2);
			}
			else if (thingie == 'd')
			{
				testSystem.stop(scale2);
			}
			else if (thingie == 'e')
			{
				testSystem.startAll();
			}
			else if (thingie == 'f')
			{
				testSystem.stopAll();
			}
			else if (thingie == 's')
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