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

//class UserInput
//{
//
//public:
//
//	UserInput( std::function< void( std::string const& ) > func ) : mFunc( func ), mShouldRun( false ), mThread( &UserInput::loop, this ) {}
//
//	void loop()
//	{
//		while( mShouldRun )
//		{
//			Sleep( 100 );
//
//			std::string line;
//			char lineEnd = '\n';
//			std::getline( std::cin, line, lineEnd );
//			if ( line == "q" )
//				mShouldRun = false;
//			else
//				mFunc( line );
//		}
//	}
//
//private:
//
//	std::function< void( std::string const& ) >		mFunc;
//	std::atomic< bool >								mShouldRun;
//	std::thread										mThread;
//
//};

int main( int argc, char *argv[] )
{
	try
	{
		_2Real::app::Engine testEngine;

		std::cout << "loading bundle" << std::endl;
		auto loadedBundle = testEngine.loadBundle( "TestBundle_BasicBlocks" );
		std::cout << "bundle loaded" << std::endl;

	// -------print the block metainfo---------

		_2Real::app::BundleMetainfo bundleinfo = loadedBundle.second;

		std::cout << "basic bundle info" << std::endl;
		std::cout << "description " << bundleinfo.getDescription() << std::endl;
		std::cout << "category " << bundleinfo.getCategory() << std::endl;
		std::cout << "author " << bundleinfo.getAuthor() << std::endl;
		std::cout << "contact " << bundleinfo.getContact() << std::endl;
		std::cout << "version " << bundleinfo.getVersion() << std::endl;

		std::vector< _2Real::app::BlockMetainfo > blockinfos = bundleinfo.getExportedBlocks();
		_2Real::app::BlockMetainfo blockinfo = blockinfos.front();
		std::cout << "exported blocks " << blockinfos.size() << std::endl;
		for ( auto it : blockinfos )
		{
			std::cout << "\t" << it.getName() << std::endl;
			std::cout << "\t" << it.getDescription() << std::endl;
			std::cout << "\tis singleton " << std::boolalpha << it.isSingleton() << std::endl;
			std::vector< std::string > dependencies = it.getDependenciesByName();
			std::cout << "\tdependencies\t";
			for ( auto it : dependencies ) std::cout << it << " ";
			std::cout << std::endl;

			std::vector< _2Real::app::InletMetainfo > inletinfos = it.getInlets();

			std::cout << "\tinlets " << inletinfos.size() << std::endl;
			for ( auto it : inletinfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getTypeMetainfo().getName() << std::endl;
				std::cout << "\t\tinitial " << it.getInitialValue() << std::endl;
				std::cout << "\t\tis multi " << std::boolalpha << it.isMulti() << std::endl;
			}

			std::vector< _2Real::app::OutletMetainfo > outletinfos = it.getOutlets();

			std::cout << "\toutlets "  << outletinfos.size() << std::endl;
			for ( auto it : outletinfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getTypeMetainfo().getName() << std::endl;
				std::cout << "\t\tinitial " << it.getInitialValue() << std::endl;
			}

			std::vector< _2Real::app::ParameterMetainfo > paraminfos = it.getParameters();

			std::cout << "\tparameters " << paraminfos.size() << std::endl;
			for ( auto it : paraminfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getTypeMetainfo().getName() << std::endl;
				std::cout << "\t\tinitial " << it.getInitialValue() << std::endl;
			}
		}

		//// -------create block instances---------

		_2Real::app::TimerHandle timer = testEngine.createTimer( 5.0 );

		_2Real::app::BundleHandle bundle = loadedBundle.first;

		auto aCounter = bundle.createBlock( "counterBlock" );
		auto aCounterIo = aCounter.getBlockIo();
		auto anIncInlet = std::dynamic_pointer_cast< _2Real::app::InletHandle >( aCounterIo.mInlets[ 0 ] );
		auto anInitParam = aCounterIo.mParameters[ 0 ];

		auto otherCounter = bundle.createBlock( "counterBlock" );
		auto otherCounterIo = otherCounter.getBlockIo();
		auto otherIncInlet = std::dynamic_pointer_cast< _2Real::app::InletHandle >( otherCounterIo.mInlets[ 0 ] );
		auto otherInitParam = otherCounterIo.mParameters[ 0 ];

		{	
			std::future< _2Real::BlockState > setup = aCounter.setup();
			handleFuture( setup );
			std::future< _2Real::BlockState > start = aCounter.startUpdating( timer );
			handleFuture( start );
		}

		{	
			std::future< _2Real::BlockState > setup = otherCounter.setup();
			handleFuture( setup );
			std::future< _2Real::BlockState > start = otherCounter.startUpdating( timer );
			handleFuture( start );
		}

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

		{
			std::future< _2Real::BlockState > stop = aCounter.stopUpdating();
			handleFuture( stop );
			std::future< _2Real::BlockState > shutdown = aCounter.shutdown();
			handleFuture( shutdown );
		}

		{
			std::future< _2Real::BlockState > stop = otherCounter.stopUpdating();
			handleFuture( stop );
			std::future< _2Real::BlockState > shutdown = otherCounter.shutdown();
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
		else
			std::cout << "doh" << std::endl;
	}

	return 0;
}