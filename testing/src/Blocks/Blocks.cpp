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

#define BOOST_ALL_DYN_LINK

#ifdef _WIN32
	#ifdef _DEBUG
		#include "vld.h"
	#endif
	#include <Windows.h>
#elif _WIN64
	#ifdef _DEBUG
		#include "vld.h"
	#endif
	#include <Windows.h>
#endif

#include "_2RealApplication.h"

int main( int argc, char *argv[] )
{
	try
	{
		_2Real::app::Engine testEngine;

		_2Real::app::BundleHandle bundle = testEngine.loadBundle( "TestBundle_SimpleBlock" );

		// -------print the block metainfo---------

		_2Real::app::BundleMetainfo bundleinfo = bundle.getMetainfo();

		std::cout << "basic bundle info" << std::endl;
		std::cout << "name " << bundleinfo.getName() << std::endl;
		std::cout << "description " << bundleinfo.getDescription() << std::endl;
		std::cout << "category " << bundleinfo.getCategory() << std::endl;
		std::cout << "author " << bundleinfo.getAuthor() << std::endl;
		std::cout << "contact " << bundleinfo.getContact() << std::endl;
		std::cout << "version " << bundleinfo.getVersion() << std::endl;
		std::cout << "filepath " << bundleinfo.getFilepath() << std::endl;

		std::vector< _2Real::app::BlockMetainfo > blockinfos;
		bundleinfo.getExportedBlocks( blockinfos );
		std::cout << "exported blocks " << blockinfos.size() << std::endl;
		for ( auto it : blockinfos )
		{
			std::cout << "\t" << it.getName() << std::endl;
			std::cout << "\t" << it.getDescription() << std::endl;
			std::cout << "\tis singleton " << std::boolalpha << it.isContext() << std::endl;
			std::vector< std::string > dependencies;
			it.getDependencies( dependencies );
			std::cout << "\tdependencies\t";
			for ( auto it : dependencies ) std::cout << it << " ";
			std::cout << std::endl;

			std::vector< _2Real::app::InletMetainfo > inletinfos;
			it.getInletMetainfos( inletinfos );

			std::cout << "\tinlets " << inletinfos.size() << std::endl;
			for ( auto it : inletinfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getDatatype() << std::endl;
				std::cout << "\t\tis multi " << std::boolalpha << it.isMultiInlet() << std::endl;
			}

			std::vector< _2Real::app::OutletMetainfo > outletinfos;
			it.getOutletMetainfos( outletinfos );

			std::cout << "\toutlets "  << outletinfos.size() << std::endl;
			for ( auto it : outletinfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getDatatype() << std::endl;
			}

			std::vector< _2Real::app::ParameterMetainfo > paraminfos;
			it.getParameterMetainfos( paraminfos );

			std::cout << "\tparameters " << paraminfos.size() << std::endl;
			for ( auto it : paraminfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getDatatype() << std::endl;
			}
		}

		// -------create block instances---------

		std::vector< _2Real::app::FunctionBlockHandle > blocks;
		for ( auto it : blockinfos )
		{
			if ( !it.isContext() )
			{
				auto handle = bundle.createBlock( blockinfos.front().getName() );
				blocks.push_back( handle );
			}
		}

		if ( blocks.empty() )
			return 0;

		_2Real::app::FunctionBlockHandle block = blocks.front();

		std::cout << "---- main: setup" << std::endl;

		std::future< _2Real::BlockState > setup0 = block.setup();
		std::future< _2Real::BlockState > setup1 = block.setup();

		setup0.wait();

		_2Real::BlockState s0 = setup0.get();
		switch ( s0 )
		{
		case _2Real::BlockState::PRE_SETUP:
			std::cout << "---- pre setup state" << std::endl;
			break;
		case _2Real::BlockState::POST_SETUP:
			std::cout << "---- post setup state" << std::endl;
			break;
		case _2Real::BlockState::POST_SETUP_RUNNING:
			std::cout << "---- post setup - running state" << std::endl;
			break;
		case _2Real::BlockState::POST_SHUTDOWN:
			std::cout << "---- pre shutdown state" << std::endl;
			break;
		}

		setup1.wait();

		_2Real::BlockState s1 = setup1.get();
		switch ( s1 )
		{
		case _2Real::BlockState::PRE_SETUP:
			std::cout << "---- pre setup state" << std::endl;
			break;
		case _2Real::BlockState::POST_SETUP:
			std::cout << "---- post setup state" << std::endl;
			break;
		case _2Real::BlockState::POST_SETUP_RUNNING:
			std::cout << "---- post setup - running state" << std::endl;
			break;
		case _2Real::BlockState::POST_SHUTDOWN:
			std::cout << "---- pre shutdown state" << std::endl;
			break;
		}

		std::cout << "---- main: update" << std::endl;

		std::future< _2Real::BlockState > update0 = block.singlestep();
		update0.wait();

		_2Real::BlockState u0 = update0.get();
		switch ( u0 )
		{
		case _2Real::BlockState::PRE_SETUP:
			std::cout << "---- pre setup state" << std::endl;
			break;
		case _2Real::BlockState::POST_SETUP:
			std::cout << "---- post setup state" << std::endl;
			break;
		case _2Real::BlockState::POST_SETUP_RUNNING:
			std::cout << "---- post setup - running state" << std::endl;
			break;
		case _2Real::BlockState::POST_SHUTDOWN:
			std::cout << "---- pre shutdown state" << std::endl;
			break;
		}

		// -------blocking cin---------

		while( 1 )
		{
			Sleep( 100 );

			std::string line;
			char lineEnd = '\n';
			std::getline( std::cin, line, lineEnd );
			if ( line == "q" )
				break;
		}
	}
	catch ( _2Real::Exception &e )
	{
		std::cout << "-------------exception caught in main------------" << std::endl;
		std::cout << e.what() << " " << e.message() << std::endl;
		std::cout << "-------------exception caught in main------------" << std::endl;
	}

	return 0;
}