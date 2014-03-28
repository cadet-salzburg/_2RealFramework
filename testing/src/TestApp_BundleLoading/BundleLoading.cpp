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

		std::cout << "testing bundle directory: " << testEngine.getBundleDirectory() << std::endl;

		{
			try
			{
				std::cout << "testing empty bundle" << std::endl;
				_2Real::app::BundleHandle handle = testEngine.loadBundle( "TestBundle_NoMetainfo" );
				std::cout << "testing malformed bundle: FAILED" << std::endl;
			}
			catch( _2Real::Exception &e )
			{
				std::cout << "testing malformed bundle: " << e.what() << " " << e.message() <<  std::endl;
			}
		}
		{
			try
			{
				std::cout << "testing bundle w. malformed metainfo" << std::endl;
				_2Real::app::BundleHandle handle = testEngine.loadBundle( "TestBundle_EmptyMetainfo" );
				std::cout << "testing malformed bundle: FAILED" << std::endl;
			}
			catch( _2Real::Exception &e )
			{
				std::cout << "testing malformed bundle: " << e.what() << " " << e.message() <<  std::endl;
			}
		}

		{
			try
			{
				std::cout << "testing bundle w. metainfo" << std::endl;
				_2Real::app::BundleHandle handle = testEngine.loadBundle( "TestBundle_FullMetainfo" );
				std::cout << "testing valid bundle: SUCCEEDED" << std::endl;
			}
			catch ( _2Real::Exception &e )
			{
				std::cout << "testing valid bundle: " << e.what() << " " << e.message() << std::endl;
			}
		}

		{
			try
			{
				std::cout << "testing bundle w. types" << std::endl;
				_2Real::app::BundleHandle handle = testEngine.loadBundle( "TestBundle_CustomTypes" );
				std::cout << "testing valid bundle: SUCCEEDED" << std::endl;
			}
			catch ( _2Real::Exception &e )
			{
				std::cout << "testing valid bundle: " << e.what() << " " << e.message() << std::endl;
			}
		}

		{
			try
			{
				std::cout << "testing bundle w. blocks" << std::endl;
				_2Real::app::BundleHandle bundle = testEngine.loadBundle( "TestBundle_BasicBlocks" );
				std::cout << "testing valid bundle: SUCCEEDED" << std::endl;
			}
			catch ( _2Real::Exception &e )
			{
				std::cout << "testing valid bundle: " << e.what() << " " << e.message() << std::endl;
			}
		}

	}
	catch ( _2Real::Exception const& e )
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