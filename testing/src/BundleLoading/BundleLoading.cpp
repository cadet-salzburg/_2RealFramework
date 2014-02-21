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
		_2Real::app::Engine &testEngine = _2Real::app::Engine::instance();

		// testing the bundle directory from environment vars
		std::cout << "testing bundle directory: " << testEngine.getBundleDirectory() << std::endl;

		// testing dll without required metainfo function
		// exception should state this, otherwise sth else went wrong! ( i.e., dll not found at all :) )
		//{
		//	try
		//	{
		//		std::cout << "testing empty bundle" << std::endl;
		//		_2Real::app::BundleHandle handle = testEngine.loadBundle( "TestBundle_NoMetainfo" );
		//		std::cout << "testing malformed bundle: FAILED" << std::endl;
		//	}
		//	catch( _2Real::Exception &e )
		//	{
		//		std::cout << "testing malformed bundle: " << e.what() << " " << e.message() <<  std::endl;
		//	}
		//}

		// testing dll with metainfo function, but no info
		// exception should state this ( incomplete metainfo ), otherwise sth else went wrong! ( i.e., dll not found at all :) )
		//{
		//	try
		//	{
		//		std::cout << "testing bundle w. malformed metainfo" << std::endl;
		//		_2Real::app::BundleHandle handle = testEngine.loadBundle( "TestBundle_EmptyMetainfo" );
		//		std::cout << "testing malformed bundle: FAILED" << std::endl;
		//	}
		//	catch( _2Real::Exception &e )
		//	{
		//		std::cout << "testing malformed bundle: " << e.what() << " " << e.message() <<  std::endl;
		//	}
		//}

		//{
		//	try
		//	{
		//		std::cout << "testing bundle w. metainfo" << std::endl;
		//		_2Real::app::BundleHandle handle = testEngine.loadBundle( "TestBundle_FullMetainfo" );
		//		std::cout << "testing valid bundle: SUCCEEDED" << std::endl;
		//	}
		//	catch ( _2Real::Exception &e )
		//	{
		//		std::cout << "testing valid bundle: " << e.what() << " " << e.message() << std::endl;
		//	}
		//}

		//{
		//	try
		//	{
		//		std::cout << "testing bundle w. types" << std::endl;
		//		_2Real::app::BundleHandle handle = testEngine.loadBundle( "TestBundle_Types" );
		//		std::cout << "testing valid bundle: SUCCEEDED" << std::endl;
		//	}
		//	catch ( _2Real::Exception &e )
		//	{
		//		std::cout << "testing valid bundle: " << e.what() << " " << e.message() << std::endl;
		//	}
		//}

		{
			try
			{
				std::cout << "testing bundle w. blocks" << std::endl;
				_2Real::app::BundleHandle bundle = testEngine.loadBundle( "TestBundle_Types" );
				std::cout << "testing valid bundle: SUCCEEDED" << std::endl;

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

					std::cout << blockinfos.front().getName() << std::endl;

					// could be the context! -> check
					_2Real::app::FunctionBlockHandle block = bundle.createBlock( blockinfos.front().getName() );

				}
			}
			catch ( _2Real::Exception &e )
			{
				std::cout << "testing valid bundle: " << e.what() << " " << e.message() << std::endl;
			}
		}

	}
	catch ( _2Real::Exception &e )
	{
		std::cout << "-------------exception caught in main------------" << std::endl;
		std::cout << e.what() << " " << e.message() << std::endl;
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