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

void printBasicTypeMetainfo( _2Real::app::TypeMetainfo const& );
void printCustomTypeMetainfo( _2Real::app::CustomTypeMetainfo const& );

void printBasicTypeMetainfo( _2Real::app::TypeMetainfo const& meta )
{
	assert( meta.isValid() );
	std::cout << "basic type " << meta.getName() << std::endl;
}

void printCustomTypeMetainfo( _2Real::app::CustomTypeMetainfo const& meta )
{
	assert( meta.isValid() );

	std::cout << "custom type " << meta.getName() << std::endl;
	std::cout << meta.getDescription() << std::endl;

	auto fields = meta.getDataFields();
	for ( auto it : fields )
	{
		std::cout << "has field " << it.first << std::endl;
		if ( it.second->isBasicType() )
			printBasicTypeMetainfo( *( static_cast< const _2Real::app::TypeMetainfo * >( it.second.get() ) ) );
		else
			printCustomTypeMetainfo( *( static_cast< const _2Real::app::CustomTypeMetainfo * >( it.second.get() ) ) );
	}
}

void handleFuture( std::future< _2Real::BlockResult > &obj, std::string const& info = "" )
{
	obj.wait();
	_2Real::BlockResult val = obj.get();
	switch ( val )
	{
	case _2Real::BlockResult::CARRIED_OUT:
		std::cout << "---- " << info << " was carried out" << std::endl;
		break;
	case _2Real::BlockResult::IGNORED:
		std::cout << "---- " << info << " was ignored" << std::endl;
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

		auto bundleinfo = loadedBundle.second;

		std::cout << "basic bundle info" << std::endl;
		std::cout << "description " << bundleinfo.getDescription() << std::endl;
		std::cout << "category " << bundleinfo.getCategory() << std::endl;
		std::cout << "author " << bundleinfo.getAuthor() << std::endl;
		std::cout << "contact " << bundleinfo.getContact() << std::endl;
		std::cout << "version " << bundleinfo.getVersion() << std::endl;

		std::cout << std::endl;

		// i know that these are custom types, so no need for casting around
		auto typeinfos = bundleinfo.getExportedTypes();
		std::cout << "number of exported types : " << typeinfos.size() << std::endl;
		if ( !typeinfos.empty() ) std::cout << "--------------------------------" << std::endl;
		for ( auto it : typeinfos )
		{
			printCustomTypeMetainfo( it );
			std::cout << "--------------------------------" << std::endl;
		}

		std::cout << std::endl;

		auto blockinfos = bundleinfo.getExportedBlocks();
		std::cout << "exported blocks " << blockinfos.size() << std::endl;
		for ( auto it : blockinfos )
		{
			std::cout << "\t" << it.getName() << std::endl;
			std::cout << "\t" << it.getDescription() << std::endl;
			auto dependencies = it.getDependenciesByName();
			std::cout << "\tdependencies\t";
			for ( auto it : dependencies ) std::cout << it << " ";
			std::cout << std::endl;

			std::vector< _2Real::app::InletMetainfo > inletinfos = it.getInlets();

			std::cout << "\tinlets " << inletinfos.size() << std::endl;
			for ( auto it : inletinfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getTypeMetainfo()->getName() << std::endl;
				std::cout << "\t\tinitial "; boost::apply_visitor< _2Real::PrintOutVisitor >( _2Real::PrintOutVisitor( std::cout ), it.getInitialValue() ); std::cout << std::endl;
				std::cout << "\t\tis multi " << std::boolalpha << it.isMulti() << std::endl;
			}

			std::vector< _2Real::app::OutletMetainfo > outletinfos = it.getOutlets();
			std::cout << "\toutlets "  << outletinfos.size() << std::endl;
			for ( auto it : outletinfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getTypeMetainfo()->getName() << std::endl;
				std::cout << "\t\tinitial "; boost::apply_visitor< _2Real::PrintOutVisitor >( _2Real::PrintOutVisitor( std::cout ), it.getInitialValue() ); std::cout << std::endl;
			}

			std::vector< _2Real::app::ParameterMetainfo > paraminfos = it.getParameters();
			std::cout << "\tparameters " << paraminfos.size() << std::endl;
			for ( auto it : paraminfos )
			{
				std::cout << "\t\tname " << it.getName() << std::endl;
				std::cout << "\t\tdescription " << it.getDescription() << std::endl;
				std::cout << "\t\tdatatype " << it.getTypeMetainfo()->getName() << std::endl;
				std::cout << "\t\tinitial "; boost::apply_visitor< _2Real::PrintOutVisitor >( _2Real::PrintOutVisitor( std::cout ), it.getInitialValue() ); std::cout << std::endl;
			}
		}

		// -------create block instances---------

		auto bundle = loadedBundle.first;

		auto threadpool = testEngine.createThreadpool( _2Real::ThreadpoolPolicy::FIFO );
		auto counter = loadedBundle.first.createBlock( "counter", threadpool, std::vector< _2Real::app::BlockHandle >() );
		auto io = counter.getBlockIo();
		auto counterinfo = bundleinfo.getExportedBlock( "counter" );
		auto inc = std::static_pointer_cast< _2Real::app::InletHandle >( counter.getBlockIo().mInlets[ 0 ] );
		auto incinfo = counterinfo.getInlet( "increment" );
		auto stringy = std::static_pointer_cast< _2Real::app::InletHandle >( counter.getBlockIo().mInlets[ 1 ] );
		auto stringyinfo = counterinfo.getInlet( "stringy" );

		auto multiinlet = std::static_pointer_cast< _2Real::app::MultiInletHandle >( counter.getBlockIo().mInlets[ 2 ] );
		auto multiinletinfo = counterinfo.getInlet( "multi" );
		std::deque< _2Real::app::InletHandle > subinlets;

		auto info = incinfo.getTypeMetainfo();
		auto custominfo = std::static_pointer_cast< _2Real::app::CustomTypeMetainfo >( info );

		auto basicinfo = stringyinfo.getTypeMetainfo();

		uint32_t aNumber = 0;
		auto aSimple = custominfo->makeCustomData();
		aSimple.set( "uint_field", aNumber );

		std::string aString = "";

		auto policy = counter.getUpdatePolicy();
		policy.set( _2Real::DefaultUpdatePolicy::ANY );

		auto timer = testEngine.createTimer( 4.0 );
		timer.start();

		std::future< _2Real::BlockResult > setup = counter.setup();
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

				//aString.appeqnd( " next" );
				//stringy->setValue( aString );

				for ( auto it : subinlets )
				{
					std::shared_ptr< _2Real::DataItem > data( new _2Real::DataItem( ( uint32_t ) 10 ) );
					it.setValue( data );
				}

				Sleep( 200 );

				//aNumber += 1;
				//aSimple.set( "uint_field", aNumber );
				//inc->setValue( aSimple );

				//aString.append( " next" );
				//stringy->setValue( aString );q

				Sleep( 200 );

				//aNumber += 1;
				//aSimple.set( "uint_field", aNumber );
				//inc->setValue( aSimple );

				//aString.append( " next" );
				//stringy->setqValue( aString );
			}
			else if ( line == "policy" )
			{
				std::future< _2Real::BlockResult > start = counter.startUpdating();
				handleFuture( start );
			}
			else if ( line == "timer" )
			{
				std::future< _2Real::BlockResult > start = counter.startUpdating( timer );
				handleFuture( start );
			}
			else if ( line == "done" )
			{
				std::future< _2Real::BlockResult > stop = counter.stopUpdating();
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

		std::future< _2Real::BlockResult > stop = counter.stopUpdating();
		handleFuture( stop );
		std::future< _2Real::BlockResult > shutdown = counter.shutdown();
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