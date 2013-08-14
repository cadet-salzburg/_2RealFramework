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

void printout( ostream &out, DataFields const& fields, const unsigned int offset )
{
	for ( DataFields::const_iterator it = fields.begin(); it != fields.end(); ++it )
	{
		DataFieldRef f = *it;
		for ( unsigned int i=0; i<offset; ++ i ) out << "\t";
		std::string name = f->getName();
		if ( name.length() >= 16 )
			out << f->getName() << "\t";
		else if ( name.length() >= 8 )
			out << f->getName() << "\t\t";
		else
			out << f->getName() << "\t\t\t";
		std::cout << f->getTypename().first << "::" << f->getTypename().second << std::endl;
		if ( !f->getSubFields().empty() )
		{
			printout( out, f->getSubFields(), offset+1 );
		}
	}
}

void printBundleInfo( app::BundleHandle const& h )
{
	BundleMetainfo info = h.getBundleMetainfo();

	std::vector< BlockMetainfo > blocks;
	info.getExportedBlocks( blocks );

	for ( std::vector< BlockMetainfo >::const_iterator it = blocks.begin(); it != blocks.end(); ++it )
	{
		BlockMetainfo block = *it;
		std::vector< InputMetainfo > inlets;		block.getInletMetainfo( inlets );
		std::vector< OutputMetainfo > outlets;		block.getOutletMetainfo( outlets );
		std::vector< InputMetainfo > parameters;	block.getParameterMetainfo( parameters );
		std::cout << "---------------------" << std::endl;
		std::cout << "-b\t" << block.getName() << " " << ( block.isContext() ? "context" : "regular" ) << std::endl;
		std::cout << "\t" << block.getDescription() << ", needs context: " << ( block.needsContext() ? "y" : "n" ) << std::endl;
		std::cout << "---------------------" << std::endl;
		std::cout << "inlets:" << std::endl;
		for ( std::vector< InputMetainfo >::const_iterator iIt = inlets.begin(); iIt != inlets.end(); ++iIt )
			std::cout << "-i\t" << iIt->getName() << " " << iIt->getTypeName().first << "::" << iIt->getTypeName().second << std::endl;
		std::cout << "parameters:" << std::endl;
		for ( std::vector< InputMetainfo >::const_iterator pIt = parameters.begin(); pIt != parameters.end(); ++pIt )
			std::cout << "-i\t" << pIt->getName() << " " << pIt->getTypeName().first << "::" << pIt->getTypeName().second << std::endl;
		std::cout << "outlets:" << std::endl;
		for ( std::vector< OutputMetainfo >::const_iterator oIt = outlets.begin(); oIt != outlets.end(); ++oIt )
			std::cout << "-o\t" << oIt->getName() << " " << oIt->getTypeName().first << "::" << oIt->getTypeName().second << std::endl;
	}
}

struct BlockInstance
{
	std::vector< InletHandle >			inlets;
	std::vector< OutletHandle >			outlets;
	std::vector< ParameterHandle >		parameters;
	BlockHandle block;
};

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();

	BundleHandle testBundle;

	const unsigned int numInstances = 5;
	std::vector< BlockInstance > blockInstances( numInstances );

	const unsigned int numTimers = 1;
	std::vector< TimerHandle > timers; timers.reserve( numTimers );

	try
	{
		testBundle = testEngine.loadBundle( "StateTesting" );

		std::cout << std::endl;
		printBundleInfo( testBundle );
		std::cout << std::endl;

		timers.push_back( testEngine.addTimer( 0.0 ) );
		timers.push_back( testEngine.addTimer( 1.0 ) );
		timers.push_back( testEngine.addTimer( 30.0 ) );

		for ( unsigned int i=0; i<numInstances; ++i )
		{
			BlockInstance &b = blockInstances[ i ];
			b.block = testBundle.createFunctionBlockInstance( "TestBlock" );

			b.block.getAllInletHandles( b.inlets );
			b.block.getAllOutletHandles( b.outlets );
			b.block.getAllParameterHandles( b.parameters );

			b.block.setup();
			b.block.start();
		}

		blockInstances[ 0 ].block.setUpdateTimer( timers[ 1 ] );
		blockInstances[ 1 ].block.setUpdateTimer( timers[ 0 ] );
		blockInstances[ 2 ].block.setUpdateTimer( timers[ 0 ] );
		blockInstances[ 3 ].block.setUpdateTimer( timers[ 0 ] );
		blockInstances[ 4 ].block.setUpdateTimer( timers[ 0 ] );

		for ( unsigned int i=0; i<numInstances-1; ++i )
		{
			blockInstances[ i ].outlets[ 0 ].link( blockInstances[ i+1 ].inlets[ 0 ] );
		}
	}
	catch ( Exception &e )
	{
		cout << "-------------exception caught in main------------" << std::endl;
		cout << e.what() << " " << e.message() << endl;
		cout << "-------------exception caught in main------------" << std::endl;
	}

	while( 1 )
	{
		Sleep( 100 );

		string line;
		char lineEnd = '\n';
		getline( cin, line, lineEnd );
		if ( line == "q" )
		{
			break;
		}
	}

	std::cout << "----------- full clear ----------------" << std::endl;
	testEngine.clearAll();
	std::cout << "----------- full clear ----------------" << std::endl;

	return 0;
}