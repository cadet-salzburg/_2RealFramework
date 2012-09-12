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

#include <windows.h>
#include <iostream>
#include <list>
#include <deque>
#include <vector>
#ifndef _UNIX
	#include "vld.h"
#endif

using namespace std;
using namespace _2Real;
using namespace _2Real::app;

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	testEngine.setBaseDirectory( "D:\\git\\trunk\\_2RealFramework\\testing\\bin" );

	unsigned int count = 5;
	std::vector< BlockHandle > blockInstances;
	blockInstances.resize( 3*count );
	std::deque< InletHandle > multiinInlets;

	InletHandle multiinIn;

	try
	{
		BundleHandle testBundle = testEngine.loadBundle( "ThreadpoolTesting" );
		testEngine.loadConfig( "threadpooltest.xml" );

		//BundleInfo info = testBundle.getBundleInfo();
		//BundleInfo::BlockInfos blocks = info.exportedBlocks;
		//for ( BundleInfo::BlockInfoIterator it = blocks.begin(); it != blocks.end(); ++it )
		//{
		//	std::cout << it->name << std::endl;
		//	BlockInfo::InletInfos inlets = it->inlets;
		//	for ( BlockInfo::InletInfoIterator iIt = inlets.begin(); iIt != inlets.end(); ++iIt )
		//	{
		//		std::cout << iIt->name << ( iIt->hasOptionCheck ? " +o " : " -o " ) << ( iIt->isMultiInlet ? " +m " : " -m " ) << std::endl;
		//	}
		//	BlockInfo::OutletInfos outlets = it->outlets;
		//	for ( BlockInfo::OutletInfoIterator oIt = outlets.begin(); oIt != outlets.end(); ++oIt )
		//	{
		//		std::cout << oIt->name << std::endl;
		//	}
		//}

		//BlockHandle multiin = testBundle.createBlockInstance( "multiin" );
		//InletHandle multiinMsg = multiin.getInletHandle( "multiin_msg" );
		//multiinMsg.setDefaultValue< string >( "multiin" );
		//multiinIn = multiin.getInletHandle( "multiin_inlet" );
		//InletHandle multiinInOptions = multiin.getInletHandle( "multiin_inlet_options" );
		//multiin.setUpdateRate( 0.5 );
		//multiin.setup();
		//multiin.start();

		//for ( unsigned int i=0; i<count; ++i )
		//{
		//	std::ostringstream inmsg, inoutmsg, outmsg;
		//	inmsg << "in_instance_" << i;
		//	inoutmsg << "inout_instance_" << i;
		//	outmsg << "out_instance_" << i;

		//	BlockHandle out = testBundle.createBlockInstance( "out" );

		//	InletHandle outMsg = out.getInletHandle( "out_msg" );
		//	outMsg.setBufferSize( 10 );
		//	outMsg.setUpdatePolicy( InletPolicy::ALWAYS );

		//	OutletHandle outOut = out.getOutletHandle( "out_outlet" );
		//	out.setUpdateRate( 0.5 );

		//	BlockHandle inout = testBundle.createBlockInstance( "in_out" );

		//	InletHandle inoutIn = inout.getInletHandle( "inout_inlet" );
		//	inoutIn.setBufferSize( 10 );
		//	inoutIn.setUpdatePolicy( InletPolicy::OR_NEWER_DATA );

		//	InletHandle inoutMsg = inout.getInletHandle( "inout_msg" );
		//	inoutMsg.setBufferSize( 10 );
		//	inoutMsg.setUpdatePolicy( InletPolicy::ALWAYS );

		//	OutletHandle inoutOut = inout.getOutletHandle( "inout_outlet" );
		//	inout.setUpdateRate( 0.5 );

		//	BlockHandle in = testBundle.createBlockInstance( "in" );

		//	InletHandle inIn = in.getInletHandle( "in_inlet" );
		//	inIn.setBufferSize( 10 );
		//	inIn.setUpdatePolicy( InletPolicy::OR_NEWER_DATA );

		//	InletHandle inMsg = in.getInletHandle( "in_msg" );
		//	inMsg.setBufferSize( 10 );
		//	inMsg.setUpdatePolicy( InletPolicy::ALWAYS );

		//	in.setUpdateRate( 0.5 );

		//	if ( !inoutIn.link( outOut ) )
		//	{
		//		std::cout << "failed to link inout_in with out_out, trying with conversion" << std::endl;

		//		if ( !inoutIn.linkWithConversion( outOut ) )
		//		{
		//			std::cout << "failed to link inout_in with out_out" << std::endl;
		//		}
		//		else std::cout << "linked inout_in with out_out with conversion" << std::endl;
		//	}
		//	else std::cout << "linked inout_in with out_out" << std::endl;
		//	if ( !inIn.link( inoutOut ) )
		//	{
		//		std::cout << "failed to link inout_out with in_in, trying with conversion" << std::endl;

		//		if ( !inIn.linkWithConversion( inoutOut ) )
		//		{
		//			std::cout << "failed to link inout_out with in_in" << std::endl;
		//		}
		//		else std::cout << "linked inout_out with in_in with conversion" << std::endl;
		//	}
		//	else std::cout << "linked inout_out with in_in" << std::endl;

		//	InletHandle next;
		//	unsigned int sz = multiinIn.getSize();
		//	if ( i < sz )
		//	{
		//		next = multiinIn[ i ];
		//		std::cout << next.getName() << std::endl;
		//	}
		//	else
		//	{
		//		next = multiinIn.add();
		//		std::cout << next.getName() << std::endl;
		//	}

		//	multiinInlets.push_back( next );

		//	if ( !next.link( inoutOut ) )
		//	{
		//		std::cout << "failed to link inout_out with multiin_in, trying with conversion" << std::endl;

		//		if ( !next.linkWithConversion( inoutOut ) )
		//		{
		//			std::cout << "failed to link inout_out with multiin_in" << std::endl;
		//		}
		//		else std::cout << "linked inout_out with multiin_in with conversion" << std::endl;
		//	}
		//	else std::cout << "linked inout_out with multiin_in" << std::endl;

		//	inMsg.setDefaultValue< string >( inmsg.str() );
		//	in.setup();
		//	inoutMsg.setDefaultValue< string >( inoutmsg.str() );
		//	inout.setup();
		//	outMsg.setDefaultValue< string >( outmsg.str() );
		//	out.setup();

		//	blockInstances[ i ] = in;
		//	blockInstances[ count + i ] = inout;
		//	blockInstances[ 2*count + i ] = out;
		//}

		//for ( std::vector< BlockHandle >::iterator it = blockInstances.begin(); it != blockInstances.end(); ++it )
		//{
		//	it->start();
		//}
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;
	}

	while( 1 )
	{
		string line;
		char lineEnd = '\n';
		getline( cin, line, lineEnd );
		if ( line == "q" )
		{
			break;
		}
		//else if ( line == "r" )
		//{
		//	InletHandle h = multiinInlets.front();
		//	multiinInlets.pop_front();
		//	multiinIn.remove( h );
		//}
	}

	//testEngine.safeConfig( "threadpooltest.xml" );

	return 0;
}