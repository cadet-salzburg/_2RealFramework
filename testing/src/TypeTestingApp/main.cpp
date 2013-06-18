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

#include "_2RealApplication.h"
#include "_2RealDatatypes.h"

#include <windows.h>
#include <iostream>
#include <list>
#include <deque>
#include <vector>
#ifndef _UNIX
#ifdef _DEBUG
	#include "vld.h"
#endif
#endif

using namespace std;
using namespace _2Real;
using namespace _2Real::app;

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	testEngine.setBaseDirectory( "." );

	InletHandle i00, i10, i20;
	OutletHandle o0, o1, o2;

	try
	{
		BundleHandle testBundle = testEngine.loadBundle( "TypeTesting" );

		BundleInfo info = testBundle.getBundleInfo();
		BundleInfo::BlockInfos blocks = info.exportedBlocks;
		for ( BundleInfo::BlockInfoIterator it = blocks.begin(); it != blocks.end(); ++it )
		{
			std::cout << "-b\t" << it->name << std::endl;
			BlockInfo::InletInfos inlets = it->inlets;
			for ( BlockInfo::InletInfoIterator iIt = inlets.begin(); iIt != inlets.end(); ++iIt )
			{
				std::cout << "-i\t" << iIt->name << ( iIt->isMultiInlet ? "\t+m " : "\t-m " ) << std::endl;
			}
			BlockInfo::OutletInfos outlets = it->outlets;
			for ( BlockInfo::OutletInfoIterator oIt = outlets.begin(); oIt != outlets.end(); ++oIt )
			{
				std::cout << "-o\t" << oIt->name << std::endl;
			}
		}

		BlockHandle testBlock0 = testBundle.createBlockInstance( "TypeTestingBlock" );
		BlockHandle testBlock1 = testBundle.createBlockInstance( "TypeTestingBlock" );
		BlockHandle testBlock2 = testBundle.createBlockInstance( "TypeTestingBlock" );

		i00 = testBlock0.getInletHandle( "customInlet0" );
		InletHandle i01 = testBlock0.getInletHandle( "customInlet1" );
		o0 = testBlock0.getOutletHandle( "customOutlet0" );

		i10 = testBlock1.getInletHandle( "customInlet0" );
		InletHandle i11 = testBlock1.getInletHandle( "customInlet1" );
		o1 = testBlock1.getOutletHandle( "customOutlet0" );

		i20 = testBlock2.getInletHandle( "customInlet0" );
		InletHandle i21 = testBlock2.getInletHandle( "customInlet1" );
		o2 = testBlock2.getOutletHandle( "customOutlet0" );

		o0.link( i10 );
		o1.link( i20 );

		testBlock0.setup();
		testBlock0.start();

		testBlock1.setup();
		testBlock1.start();

		testBlock2.setup();
		testBlock2.start();
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;
	}

	int cnt = 0;

	while( 1 )
	{
		std::shared_ptr< const CustomType > o0data = o0.getCurrentData();
		std::cout << "o0: " << o0data->get< int >( "test int" ) << std::endl;

		// this receives o0s outputs. it's extremely unlikely that it would ever be higher than o0
		std::shared_ptr< const CustomType > i10data = i10.getCurrentData();
		std::cout << "i10: " << i10data->get< int >( "test int" ) << std::endl;

		std::shared_ptr< CustomType > t = i00.makeData();
		t->set< int >( "test int", ++cnt );
		i00.receiveData( t );

		app::TypeMetainfo info = i00.getType();
		TypeMetainfo::FieldDesc d = info.getFieldInfo();
		//for ( TypeMetainfo::FieldDesc::const_iterator it = d.begin(); it != d.end(); ++it )
		//{
		//	std::cout << *it << std::endl;
		//}

		string line;
		char lineEnd = '\n';
		getline( cin, line, lineEnd );
		if ( line == "q" )
		{
			break;
		}
	}

	return 0;
}