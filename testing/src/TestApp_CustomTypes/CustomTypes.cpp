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

int main( int argc, char *argv[] )
{
	try
	{
		_2Real::app::Engine engine;
		auto loadedBundle = engine.loadBundle( "TestBundle_CustomTypes" );

	// -------print the metainfos---------

		_2Real::app::BundleMetainfo bundleinfo = loadedBundle.second;

		std::cout << "basic bundle info" << std::endl;
		std::cout << "description " << bundleinfo.getDescription() << std::endl;
		std::cout << "category " << bundleinfo.getCategory() << std::endl;
		std::cout << "author " << bundleinfo.getAuthor() << std::endl;
		std::cout << "contact " << bundleinfo.getContact() << std::endl;
		std::cout << "version " << bundleinfo.getVersion() << std::endl;

		// i know that these are custom types, so no need for casting around
		auto typeinfos = bundleinfo.getExportedTypes();
		std::cout << "number of exported types : " << typeinfos.size() << std::endl;
		if ( !typeinfos.empty() ) std::cout << "--------------------------------" << std::endl;
		for ( auto it : typeinfos )
		{
			printCustomTypeMetainfo( it );
			std::cout << "--------------------------------" << std::endl;
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

		_2Real::CustomDataItem aSimple = typeinfos[ 0 ].makeCustomData();
		_2Real::CustomDataItem aComplex = typeinfos[ 1 ].makeCustomData();

		std::cout << "------" << std::endl;

		aSimple.set( "int_field", ( int32_t )10005 );
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