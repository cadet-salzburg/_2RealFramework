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
		_2Real::app::Engine engine;
		_2Real::app::BundleHandle bundle = engine.loadBundle( "TestBundle_CustomTypes" );

	// -------print the metainfos---------

		_2Real::app::BundleMetainfo bundleinfo = bundle.getMetainfo();

		std::cout << "basic bundle info" << std::endl;
		std::cout << "name " << bundleinfo.getName() << std::endl;
		std::cout << "description " << bundleinfo.getDescription() << std::endl;
		std::cout << "category " << bundleinfo.getCategory() << std::endl;
		std::cout << "author " << bundleinfo.getAuthor() << std::endl;
		std::cout << "contact " << bundleinfo.getContact() << std::endl;
		std::cout << "version " << bundleinfo.getVersion() << std::endl;
		std::cout << "filepath " << bundleinfo.getFilepath() << std::endl;

		std::vector< _2Real::app::CustomTypeMetainfo > typeinfos;
		bundleinfo.getExportedTypes( typeinfos );
		std::cout << "number of exported types : " << typeinfos.size() << std::endl;
		for ( auto it : typeinfos )
		{
			std::cout << "\t" << it.getName() << std::endl;
			std::cout << "\t" << it.getDescription() << std::endl;
		}

	// -------print the metainfos---------

		while( 1 )
		{
			Sleep( 100 );

			std::string line;
			char lineEnd = '\n';
			std::getline( std::cin, line, lineEnd );
			if ( line == "q" )
				break;
		}

	// -------init some types---------

		_2Real::CustomDataItem aSimple = typeinfos[ 1 ].makeCustomData();
		_2Real::CustomDataItem aComplex = typeinfos[ 0 ].makeCustomData();

		std::cout << "------" << std::endl;

		//aSimple.set( "int_field", ( int32_t )10005 );
		aComplex.set( "simple_field1", aSimple );

		std::cout << "------" << std::endl;

		std::cout << aSimple << std::endl;
		std::cout << aComplex << std::endl;

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