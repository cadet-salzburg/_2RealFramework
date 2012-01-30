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

#include <windows.h>
#include <iostream>
#include <string>

#include "vld.h"

#include "Poco\Mutex.h"

using namespace _2Real;
using namespace std;
using namespace Poco;

bool run;
FastMutex mutex1;
FastMutex mutex2;
FastMutex mutex3;
FastMutex mutex4;
FastMutex mutex5;
vector< int > vec1Data;
vector< int > vec2Data;
vector< int > vec3Data;
vector< int > addData;
vector< int > subData;

void vec1DataAvailable(Data &data)
{
	//ScopedLock< FastMutex > lock(mutex1);
	//vec1Data = data.getData< vector < int > >();
	//cout << "vec1 data: " << vec1Data.size() << endl;
	//for (unsigned int i=0; i<vec1Data.size(); ++i)
	//{
	//	cout << vec1Data.at(i) << " ";
	//}
	//cout << endl;
}

void vec2DataAvailable(Data &data)
{
	//ScopedLock< FastMutex > lock(mutex2);
	//vec2Data = data.getData< vector < int > >();
	//cout << "vec2 data: " << vec2Data.size() << endl;
	//for (unsigned int i=0; i<vec2Data.size(); ++i)
	//{
	//	cout << vec2Data.at(i) << " ";
	//}
	//cout << endl;
}

void vec3DataAvailable(Data &data)
{
	//ScopedLock< FastMutex > lock(mutex3);
	//vec3Data = data.getData< vector < int > >();
	//cout << "vec3 data: " << vec3Data.size() << endl;
	//for (unsigned int i=0; i<vec3Data.size(); ++i)
	//{
	//	cout << vec3Data.at(i) << " ";
	//}
	//cout << endl;
}

void subDataAvailable(Data &data)
{
	//ScopedLock< FastMutex > lock(mutex4);
	//subData = data.getData< vector < int > >();
	//cout << "sub data: " << subData.size() << endl;
	//for (unsigned int i=0; i<subData.size(); ++i)
	//{
	//	cout << subData.at(i) << " ";
	//}
	//cout << endl;
}

void addDataAvailable(Data &data)
{
	//ScopedLock< FastMutex > lock(mutex5);
	//addData = data.getData< vector < int > >();
	//cout << "add data: " << addData.size() << endl;
	//for (unsigned int i=0; i<addData.size(); ++i)
	//{
	//	cout << addData.at(i) << " ";
	//}
	//cout << endl;
}

void systemException(RunnableError &e)
{
	cout << "exception in " << e.system().name() << " by " << e.sender().name() << endl;
	cout << e.exception().what() << " " << e.exception().message() << endl;
}

int main(int argc, char *argv[])
{
	try
	{
		System sys("sys");

		sys.registerToException(::systemException);
		sys.setInstallDirectory("D:\\cadet\\trunk\\_2RealFramework\\kernel\\samples\\testplugins\\bin\\");
		//sys.setInstallDirectory("C:\\users\\Cthulhu\\Desktop\\cadet\\trunk\\_2RealFramework\\kernel\\samples\\testplugins\\bin\\");
		sys.setLogfile("simplesystem.txt");

		Identifier plugin = sys.loadPlugin("plugin", "SimpleStuff");
		std::string info = sys.getInfo(plugin);
		std::cout << info << std::endl;

		sys.setup(plugin);

		Identifier vec1 = sys.createService("vector 1", plugin, "VecInit");
		Identifier vec2 = sys.createService("vector 2", plugin, "VecInit");
		Identifier vec3 = sys.createService("vector 3", plugin, "VecInit");

		sys.setValue(vec1, "element value", 5);
		sys.setValue(vec2, "element value", 15);
		sys.setValue(vec3, "element value", 30);

		sys.setValue(vec1, "vector size", (unsigned int)3);
		sys.setValue(vec2, "vector size", (unsigned int)3);
		sys.setValue(vec3, "vector size", (unsigned int)3);

		sys.setup(vec1);
		sys.setup(vec2);
		sys.setup(vec3);

		Identifier add = sys.createService("addition", plugin, "VecAdd");
		Identifier sub = sys.createService("subtraction", plugin, "VecSub");

		sys.linkSlots(add, "add A", vec1, "init out");
		sys.linkSlots(add, "add B", vec2, "init out");
		sys.linkSlots(sub, "sub A", vec3, "init out");
		sys.linkSlots(sub, "sub B", add, "add out");

		sys.setup(add);
		sys.setup(sub);

		//sys.setUpdateRate(vec1, 30.0f);
		//sys.setUpdateRate(vec2, 30.0f);
		//sys.setUpdateRate(vec3, 30.0f);
		//sys.setUpdateRate(add, 30.0f);
		//sys.setUpdateRate(sub, 30.0f);

		sys.registerToNewData(sub, "sub out", ::subDataAvailable);
		sys.registerToNewData(add, "add out", ::addDataAvailable);
		sys.registerToNewData(vec1, "init out", ::vec1DataAvailable);
		sys.registerToNewData(vec2, "init out", ::vec2DataAvailable);
		sys.registerToNewData(vec3, "init out", ::vec3DataAvailable);

		//sys.start(vec1);
		//sys.start(vec2);
		//sys.start(vec3);
		//sys.start(add);
		//sys.start(sub);

		Identifier sync0 = sys.createSynchronization("s 0", vec1, vec2);

		//cout << "main: SYNC 0 CREATED" << endl;

		sys.setUpdateRate(sync0, 100.0f);
		sys.start(sync0);

		Identifier sync1 = sys.createSynchronization("s 1", add, vec3);

		//cout << "main: SYNC 1 CREATED" << endl;

		sys.setUpdateRate(sync1, 100.0f);
		sys.start(sync1);

		Identifier seq = sys.createSequence("test sequence", sync1, sub);

		//cout << "main: SEQ CREATED" << endl;

		sys.setUpdateRate(seq, 100.0f);
		sys.start(seq);

		cout << "main: ALL STARTED" << endl;

		while(1)
		{

			string line;
			unsigned int sz;
			int elem;
			char lineEnd = '\n';
			getline(cin, line, lineEnd);

			if (line == "")
			{
			}
			else if (line == "size 1")
			{
				cin >> sz;
				sys.setValue(vec1, "vector size", sz);
			}
			else if (line == "size 2")
			{
				cin >> sz;
				sys.setValue(vec2, "vector size", sz);
			}
			else if (line == "size 3")
			{
				cin >> sz;
				sys.setValue(vec3, "vector size", sz);
			}
			else if (line == "elem 1")
			{
				cin >> elem;
				sys.setValue(vec1, "element value", elem);
			}
			else if (line == "elem 2")
			{
				cin >> elem;
				sys.setValue(vec2, "element value", elem);
			}
			else if (line == "elem 3")
			{
				cin >> elem;
				sys.setValue(vec3, "element value", elem);
			}
			else if (line == "stop 1")
			{
				sys.stop(vec1);
			}
			else if (line == "stop 2")
			{
				sys.stop(vec2);
			}
			else if (line == "stop 3")
			{
				sys.stop(vec3);
			}
			else if (line == "stop a")
			{
				sys.stop(add);
			}
			else if (line == "stop s")
			{
				sys.stop(sub);
			}
			else if (line == "start 1")
			{
				sys.start(vec1);
			}
			else if (line == "start 2")
			{
				sys.start(vec2);
			}
			else if (line == "start 3")
			{
				sys.start(vec3);
			}
			else if (line == "start a")
			{
				sys.start(add);
			}
			else if (line == "start s")
			{
				sys.start(sub);
			}
			else if (line == "query a")
			{
				vector< int > current = sys.getValue< vector < int > >(sub, "sub A");
				for (unsigned int i=0; i<current.size(); i++)
				{
					cout << current.at(i) << endl;
				}
			}
			else if (line == "query b")
			{
				vector< int > current = sys.getValue< vector < int > >(sub, "sub B");
				for (unsigned int i=0; i<current.size(); i++)
				{
					cout << current.at(i) << endl;
				}
			}
			else if (line == "quit")
			{
				break;
			}
		}

		sys.stopAll();

		cout << "XXXXXXXXXXXXXXXXXXXXXXXXX END XXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
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
		if (line == "quit")
		{
			break;
		}
	}

	return 0;
}