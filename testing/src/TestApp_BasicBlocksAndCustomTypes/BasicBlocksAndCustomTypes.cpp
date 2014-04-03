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

void handleFuture( std::future< _2Real::BlockState > &obj, std::string const& info = "" )
{
	obj.wait();
	_2Real::BlockState val = obj.get();
	switch ( val )
	{
	case _2Real::BlockState::PRE_SETUP:
		std::cout << "---- " << info << " pre setup state" << std::endl;
		break;
	case _2Real::BlockState::POST_SETUP:
		std::cout << "---- " << info << " post setup state" << std::endl;
		break;
	case _2Real::BlockState::POST_SETUP_RUNNING:
		std::cout << "---- " << info << " running state" << std::endl;
		break;
	case _2Real::BlockState::POST_SHUTDOWN:
		std::cout << "---- " << info << " post shutdown state" << std::endl;
		break;
	}
}

int main( int argc, char *argv[] )
{
	try
	{
		_2Real::app::Engine testEngine;

		_2Real::app::BundleHandle bundle = testEngine.loadBundle( "TestBundle_BasicBlocksAndCustomTypes" );

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
				std::cout << "\t\tinitial " << it.getInitialValue() << std::endl;
				std::cout << "\t\tis multi " << std::boolalpha << it.isMultiInlet() << std::noboolalpha << std::endl;
			}

			std::vector< _2Real::app::OutletMetainfo > outletinfos;
			it.getOutletMetainfos( outletinfos );

			std::cout << "\toutlets "  << outletinfos.size() << std::endl;
			for ( auto it : outletinfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getDatatype() << std::endl;
				std::cout << "\t\tinitial " << it.getInitialValue() << std::endl;
			}

			std::vector< _2Real::app::ParameterMetainfo > paraminfos;
			it.getParameterMetainfos( paraminfos );

			std::cout << "\tparameters " << paraminfos.size() << std::endl;
			for ( auto it : paraminfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getDatatype() << std::endl;
				std::cout << "\t\tinitial " << it.getInitialValue() << std::endl;
			}
		}

		// -------create block instances---------

		std::map< std::string, _2Real::app::FunctionBlockHandle > blocks;
		for ( auto it : blockinfos )
		{
			if ( !it.isContext() )
			{
				auto handle = bundle.createBlock( it.getName() );
				blocks[ it.getName() ] = handle;
			}
		}

		if ( blocks.empty() )
			return 0;

		_2Real::app::TimerHandle timer = testEngine.createTimer( 5.0 );
		_2Real::app::FunctionBlockHandle counter = blocks[ "counter" ];

		_2Real::app::BlockIo counterio = counter.getBlockIo();
		_2Real::app::InletHandle *incInlet = dynamic_cast< _2Real::app::InletHandle * >( counterio.mInlets[ 0 ] );

		// should io slots really hand out type metainfo?
		//_2Real::app::TypeMetainfo info = incInlet->getTypeMetainfo();
		//std::cout << std::boolalpha << info.isBasicType() << std::noboolalpha << std::endl;

		//_2Real::app::CustomTypeMetainfo &custominfo = dynamic_cast< _2Real::app::CustomTypeMetainfo & >( info );

		//std::cout << custominfo.getName() << std::endl;
		//std::cout << custominfo.getDescription() << std::endl;

		//for ( auto it : counterio.mInlets )
		//{
		//	if ( it->isMultiInlet() )
		//	{
		//		auto inlet = dynamic_cast< _2Real::app::MultiInletHandle * >( it );
		//		std::cout << " inlet " << inlet->getName() << " is a multiinlet, size " << inlet->getSize() << std::endl;
		//	}
		//	else
		//	{
		//		auto inlet = dynamic_cast< _2Real::app::InletHandle * >( it );
		//		std::cout << " inlet " << inlet->getName() << " is an regular inlet " << std::endl;
		//	}
		//}

		//for ( auto it : counterio.mParameters )
		//{
		//	std::cout << "parameter " << it->getName() << std::endl;
		//}

		//for ( auto it : counterio.mOutlets )
		//{
		//	std::cout << "outlet " << it->getName() << std::endl;
		//}

		std::future< _2Real::BlockState > setup = counter.setup();
		handleFuture( setup );
		std::future< _2Real::BlockState > start = counter.startUpdating( timer );
		handleFuture( start );

		while( 1 )
		{
			Sleep( 100 );

			std::string line;
			char lineEnd = '\n';
			std::getline( std::cin, line, lineEnd );
			if ( line == "q" )
				break;
			else if ( line == "stop" )
			{
				timer.stop();
			}
			else if ( line == "start" )
			{
				timer.start();
			}
		}

		for ( auto it : blocks )
		{
			std::future< _2Real::BlockState > stop = it.second.stopUpdating();
			handleFuture( stop );
			std::future< _2Real::BlockState > shutdown = it.second.shutdown();
			handleFuture( shutdown );
		}

		while( 1 )
		{
			Sleep( 100 );

			std::string line;
			char lineEnd = '\n';
			std::getline( std::cin, line, lineEnd );
			if ( line == "q" )
				break;
		}

		testEngine.clear();

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
	catch ( std::exception const& e )
	{
		std::cout << "-------------exception caught in main------------" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "-------------exception caught in main------------" << std::endl;
	}

	while( 1 )
	{
		Sleep( 100 );

		std::string line;
		char lineEnd = '\n';
		std::getline( std::cin, line, lineEnd );
		if ( line == "q" )
			break;
	}

	return 0;
}