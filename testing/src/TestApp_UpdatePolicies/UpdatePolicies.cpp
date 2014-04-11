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

#include <iomanip>

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

		auto loadedBundle = testEngine.loadBundle( "TestBundle_BasicBlocksAndCustomTypes" );

	// -------print the block metainfo---------

		_2Real::app::BundleMetainfo bundleinfo = loadedBundle.second;

		std::cout << "basic bundle info" << std::endl;
		std::cout << "description " << bundleinfo.getDescription() << std::endl;
		std::cout << "category " << bundleinfo.getCategory() << std::endl;
		std::cout << "author " << bundleinfo.getAuthor() << std::endl;
		std::cout << "contact " << bundleinfo.getContact() << std::endl;
		std::cout << "version " << bundleinfo.getVersion() << std::endl;

		std::cout << std::endl;

		std::vector< _2Real::app::BlockMetainfo > blockinfos = bundleinfo.getExportedBlocks();
		std::cout << "exported blocks" << std::endl;
		for ( auto it : blockinfos )
		{
			std::cout << "name " << it.getName() << std::endl;
			std::cout << "description " << it.getDescription() << std::endl;
			std::cout << "is singleton " << std::boolalpha << it.isSingleton() << std::endl;
			std::cout << "dependencies : "; for ( auto it : it.getDependenciesByName() ) std::cout << it << " "; std::cout << std::endl;

			std::vector< _2Real::app::InletMetainfo > inletinfos = it.getInlets();
			std::cout << "inlets" << std::endl;
			for ( auto it : inletinfos )
			{
				std::cout << "name " << it.getName() << std::endl;
				std::cout << "description " << it.getDescription() << std::endl;
				std::cout << "datatype " << it.getTypeMetainfo().getName() << std::endl;
				std::cout << "initial " << it.getInitialValue() << std::endl;
				std::cout << "is multi " << std::boolalpha << it.isMulti() << std::endl;
			}

			std::vector< _2Real::app::OutletMetainfo > outletinfos = it.getOutlets();
			std::cout << "outlets" << std::endl;
			for ( auto it : outletinfos )
			{
				std::cout << "name " << it.getName() << std::endl;
				std::cout << "description " << it.getDescription() << std::endl;
				std::cout << "datatype " << it.getTypeMetainfo().getName() << std::endl;
				std::cout << "initial " << it.getInitialValue() << std::endl;
			}

			std::vector< _2Real::app::ParameterMetainfo > paraminfos = it.getParameters();
			std::cout << "parameters" << std::endl;
			for ( auto it : paraminfos )
			{
				std::cout << "name " << it.getName() << std::endl;
				std::cout << "description " << it.getDescription() << std::endl;
				std::cout << "datatype " << it.getTypeMetainfo().getName() << std::endl;
				std::cout << "initial " << it.getInitialValue() << std::endl;
			}
		}

		// -------create block instances---------

		_2Real::app::BundleHandle bundle = loadedBundle.first;

		auto counter = bundle.createBlock( "counter" );
		auto io = counter.getBlockIo();
		auto counterinfo = bundleinfo.getExportedBlock( "counter" );
		auto inc = std::dynamic_pointer_cast< _2Real::app::InletHandle >( counter.getBlockIo().mInlets[ 0 ] );
		auto incinfo = counterinfo.getInlet( "increment" );
		auto stringy = std::dynamic_pointer_cast< _2Real::app::InletHandle >( counter.getBlockIo().mInlets[ 1 ] );
		auto stringyinfo = counterinfo.getInlet( "stringy" );

		auto multiinlet = std::dynamic_pointer_cast< _2Real::app::MultiInletHandle >( counter.getBlockIo().mInlets[ 2 ] );
		auto multiinletinfo = counterinfo.getInlet( "multi" );
		std::deque< _2Real::app::InletHandle > subinlets;

		// urgh. this has been sliced now... how the hell to deal with this?
		auto simpleinfo = incinfo.getTypeMetainfo();
		auto custominfo = bundleinfo.getExportedType( simpleinfo.getName() );

		auto basicinfo = stringyinfo.getTypeMetainfo();

		uint32_t aNumber = 0;
		auto aSimple = custominfo.makeCustomData();
		aSimple.set( "uint_field", aNumber );

		std::string aString = "";

		auto policy = counter.getUpdatePolicy();
		policy.set( _2Real::DefaultPolicy::ANY );

		auto timer = testEngine.createTimer( 4.0 );
		timer.start();

		std::future< _2Real::BlockState > setup = counter.setup();
		handleFuture( setup );

		while( 1 )
		{
			Sleep( 100 );

			std::string line;
			char lineEnd = '\n';
			std::getline( std::cin, line, lineEnd );
			if ( line == "q" )
				break;
			else if ( line == "test" )
			{
				//aNumber += 1;
				//aSimple.set( "uint_field", aNumber );
				//inc->setValue( aSimple );

				//aString.append( " next" );
				//stringy->setValue( aString );

				for ( auto it : subinlets )
					it.setValue( ( uint32_t )10 );

				Sleep( 200 );

				//aNumber += 1;
				//aSimple.set( "uint_field", aNumber );
				//inc->setValue( aSimple );

				//aString.append( " next" );
				//stringy->setValue( aString );

				Sleep( 200 );

				//aNumber += 1;
				//aSimple.set( "uint_field", aNumber );
				//inc->setValue( aSimple );

				//aString.append( " next" );
				//stringy->setValue( aString );
			}
			else if ( line == "policy" )
			{
				std::future< _2Real::BlockState > start = counter.startUpdating();
				handleFuture( start );
			}
			else if ( line == "timer" )
			{
				std::future< _2Real::BlockState > start = counter.startUpdating( timer );
				handleFuture( start );
			}
			else if ( line == "done" )
			{
				std::future< _2Real::BlockState > stop = counter.stopUpdating();
				handleFuture( stop );
			}
			else if ( line == "add" )
			{
				_2Real::app::InletHandle subinlet = multiinlet->add();
				subinlets.push_back( subinlet );
			}
			else if ( line == "del" )
			{
				if ( !subinlets.empty() )
				{
					multiinlet->remove( subinlets.back() );
					subinlets.pop_back();
				}
			}
		}

		std::future< _2Real::BlockState > stop = counter.stopUpdating();
		handleFuture( stop );
		std::future< _2Real::BlockState > shutdown = counter.shutdown();
		handleFuture( shutdown );

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