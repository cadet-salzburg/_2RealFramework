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
		std::cout << "-b\t" << block.getName() << " " << ( block.isContext() ? "regular" : "context" ) << std::endl;
		std::cout << "\t" << block.getDescription() << ", needs context: " << " unknown" << std::endl;
		std::cout << "---------------------" << std::endl;
		std::cout << "inlets:" << std::endl;
		for ( std::vector< InputMetainfo >::const_iterator iIt = inlets.begin(); iIt != inlets.end(); ++iIt )
		{
			std::cout << "-i\t" << iIt->getName() << " " << iIt->getTypeName().first << "::" << iIt->getTypeName().second << std::endl;
			std::cout << "\t" << std::boolalpha << " -b " << iIt->isBuffered() << " -l " << iIt->canLink() << " -m " << iIt->canExpand() << std::endl;
		}
		std::cout << "parameters:" << std::endl;
		for ( std::vector< InputMetainfo >::const_iterator pIt = parameters.begin(); pIt != parameters.end(); ++pIt )
		{
			std::cout << "-i\t" << pIt->getName() << " " << pIt->getTypeName().first << "::" << pIt->getTypeName().second << std::endl;
			std::cout << "\t" << std::boolalpha << " -b " << pIt->isBuffered() << " -l " << pIt->canLink() << " -m " << pIt->canExpand() << std::endl;
		}
		std::cout << "outlets:" << std::endl;
		for ( std::vector< OutputMetainfo >::const_iterator oIt = outlets.begin(); oIt != outlets.end(); ++oIt )
		{
			std::cout << "-o\t" << oIt->getName() << " " << oIt->getTypeName().first << "::" << oIt->getTypeName().second << std::endl;
			std::cout << "\t" << std::boolalpha << " -l " << oIt->canLink() << " -m " << oIt->canExpand() << std::endl;
		}
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

	const unsigned int numInstances = 1;
	std::vector< BlockInstance > blockInstancesA( numInstances );
	std::vector< BlockInstance > blockInstancesB( numInstances );

	BundleHandle testBundleA, testBundleB;

	try
	{
		testBundleA = testEngine.loadBundle( "TypeTestingA" );
		testBundleB = testEngine.loadBundle( "TypeTestingB" );

		// get bundle info

		std::cout << std::endl;
		std::cout << "--------- bundle infos -----------------------------------------------" << std::endl;
		std::cout << std::endl;
		std::cout << "--------- bundle info of bundle a-----------------" << std::endl;
		std::cout << std::endl;
		printBundleInfo( testBundleA );
		std::cout << std::endl;
		std::cout << "--------- bundle info of bundle b-----------------" << std::endl;
		std::cout << std::endl;
		printBundleInfo( testBundleB );
		std::cout << std::endl;
		std::cout << "--------- bundle infos -----------------------------------------------" << std::endl;
		std::cout << std::endl;

		for ( unsigned int i=0; i<numInstances; ++i )
		{
			BlockInstance &b = blockInstancesA[ i ];
			b.block = testBundleA.createFunctionBlockInstance( "TypeTestingBlock" );

			b.block.getAllInletHandles( b.inlets );
			b.block.getAllOutletHandles( b.outlets );
			b.block.getAllParameterHandles( b.parameters );

			b.block.setup();
			b.block.start();
		}

		for ( unsigned int i=0; i<numInstances; ++i )
		{
			BlockInstance &b = blockInstancesB[ i ];
			b.block = testBundleB.createFunctionBlockInstance( "TypeTestingBlock" );

			b.block.getAllInletHandles( b.inlets );
			b.block.getAllOutletHandles( b.outlets );
			b.block.getAllParameterHandles( b.parameters );

			b.block.setup();
			b.block.start();
		}

		for ( unsigned int i=0; i<numInstances-1; ++i )
		{
			blockInstancesA[ i ].outlets[ 0 ].link( blockInstancesA[ i+1 ].inlets[ 0 ] );
			blockInstancesA[ i ].outlets[ 1 ].link( blockInstancesA[ i+1 ].inlets[ 2 ] );
			blockInstancesA[ i ].outlets[ 2 ].link( blockInstancesA[ i+1 ].inlets[ 4 ] );
			blockInstancesA[ i ].outlets[ 3 ].link( blockInstancesA[ i+1 ].inlets[ 6 ] );
			blockInstancesA[ i ].outlets[ 4 ].link( blockInstancesA[ i+1 ].inlets[ 8 ] );
		}

		for ( unsigned int i=0; i<numInstances-1; ++i )
		{
			blockInstancesB[ i ].outlets[ 0 ].link( blockInstancesB[ i+1 ].inlets[ 0 ] );
			blockInstancesB[ i ].outlets[ 1 ].link( blockInstancesB[ i+1 ].inlets[ 2 ] );
			blockInstancesB[ i ].outlets[ 2 ].link( blockInstancesB[ i+1 ].inlets[ 4 ] );
			blockInstancesB[ i ].outlets[ 3 ].link( blockInstancesB[ i+1 ].inlets[ 6 ] );
			blockInstancesB[ i ].outlets[ 4 ].link( blockInstancesB[ i+1 ].inlets[ 8 ] );
		}


		// linking of base types
		blockInstancesA[ 0 ].outlets[ 0 ].link( blockInstancesB[ 0 ].inlets[ 0 ] );
		// linking of complex types
		blockInstancesA[ 0 ].outlets[ 1 ].link( blockInstancesB[ 0 ].inlets[ 2 ] );
		// linking of simple types
		blockInstancesA[ 0 ].outlets[ 2 ].link( blockInstancesB[ 0 ].inlets[ 4 ] );
		// linking of vectors
		blockInstancesA[ 0 ].outlets[ 3 ].link( blockInstancesB[ 0 ].inlets[ 6 ] );
		// linking of vector vectors
		blockInstancesA[ 0 ].outlets[ 4 ].link( blockInstancesB[ 0 ].inlets[ 8 ] );
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
		else if ( line == "out B 0" )
		{
			BlockInstance &b = blockInstancesB[ 0 ];

			std::cout << std::endl;
			std::cout << "--------- type metainfo of --B-- basetype ------------------------------------" << std::endl;
			std::cout << std::endl;

			{
				TypeMetainfo info = b.inlets[ 0 ].getType();
				std::cout << info.getTypename().first << " " << info.getTypename().second << std::endl;
				DataFields fields; info.getDataFields( fields );
				printout( std::cout, fields, 0 );
			}

			//std::cout << std::endl;
			//std::cout << "--------- current data of i0 ( basetype ) ----------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.inlets[ 0 ].getCurrentData();
			//	std::cout << "char:\t\t\t" << ( int ) *( d->get< char >( "char" ).get() ) << std::endl;
			//	std::cout << "uchar:\t\t\t" << ( int ) *( d->get< unsigned char >( "uchar" ).get() ) << std::endl;
			//	std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
			//	std::cout << "uint:\t\t\t" << ( int ) *( d->get< unsigned int >( "uint" ).get() ) << std::endl;
			//	std::cout << "float:\t\t\t" << *( d->get< float >( "float" ).get() ) << std::endl;
			//	std::cout << "double:\t\t\t" << *( d->get< double >( "double" ).get() ) << std::endl;
			//	std::cout << "string:\t\t\t" << *( d->get< std::string >( "string" ).get() ) << std::endl;
			//	std::cout << "bool:\t\t\t" << std::boolalpha << *( d->get< bool >( "bool" ).get() ) << std::endl;
			//	std::vector< int > const& vi = *( d->get< std::vector< int > >( "int vector" ).get() );
			//	std::cout << "int vector:\t\t" << "size " <<  vi.size() << std::endl;
			//	std::vector< std::vector< int > > const& vvi = *( d->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
			//	std::cout << "int vector vector:\t" << "size " <<  vvi.size() << std::endl;
			//}

			//std::cout << std::endl;
			//std::cout << "--------- current data of i1 ( basetype ) ---------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.inlets[ 1 ].getCurrentData();
			//	std::cout << "char:\t\t\t" << ( int ) *( d->get< char >( "char" ).get() ) << std::endl;
			//	std::cout << "uchar:\t\t\t" << ( int ) *( d->get< unsigned char >( "uchar" ).get() ) << std::endl;
			//	std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
			//	std::cout << "uint:\t\t\t" << ( int ) *( d->get< unsigned int >( "uint" ).get() ) << std::endl;
			//	std::cout << "float:\t\t\t" << *( d->get< float >( "float" ).get() ) << std::endl;
			//	std::cout << "double:\t\t\t" << *( d->get< double >( "double" ).get() ) << std::endl;
			//	std::cout << "string:\t\t\t" << *( d->get< std::string >( "string" ).get() ) << std::endl;
			//	std::cout << "bool:\t\t\t" << std::boolalpha << *( d->get< bool >( "bool" ).get() ) << std::endl;
			//	std::vector< int > const& vi = *( d->get< std::vector< int > >( "int vector" ).get() );
			//	std::cout << "int vector:\t\t" << "size " <<  vi.size() << std::endl;
			//	std::vector< std::vector< int > > const& vvi = *( d->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
			//	std::cout << "int vector vector:\t" << "size " <<  vvi.size() << std::endl;
			//}

			std::cout << std::endl;
			std::cout << "--------- type metainfo of --B-- complextype -----------------------------------" << std::endl;
			std::cout << std::endl;

			{
				TypeMetainfo info = b.inlets[ 2 ].getType();
				std::cout << info.getTypename().first << " " << info.getTypename().second << std::endl;
				DataFields fields; info.getDataFields( fields );
				printout( std::cout, fields, 0 );
			}

			std::cout << std::endl;
			std::cout << "--------- current data of i2 (  --B-- complextype ) ----------------" << std::endl;
			std::cout << std::endl;

			{
				std::shared_ptr< const CustomType > d = b.inlets[ 2 ].getCurrentData();
				std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "--B-- int" ).get() ) << std::endl;
				std::cout << "string:\t\t\t" << *( d->get< std::string >( "--B-- string" ).get() ) << std::endl;
				std::vector< float > const& vf = *( d->get< std::vector< float > >( "--B-- float vector" ).get() );
				std::cout << "float vector:\t\t" << "size " <<  vf.size() << std::endl;
				std::shared_ptr< const CustomType > i = d->get< CustomType >( "--B-- image" );
					std::shared_ptr< const Image > img = Image::asImage( i );
					std::cout << "image:" << std::endl;
					std::cout << "\twidth:\t\t\t" << img->getWidth() << std::endl;
					std::cout << "\theight:\t\t\t" << img->getHeight() << std::endl;
					std::cout << "\tchannel order:\t\t" << ( std::string ) img->getChannelOrder() << std::endl;
					std::cout << "\tformat:\t\t\t" << ( std::string ) img->getDatatype() << std::endl;
				std::shared_ptr< const CustomType > b = d->get< CustomType >( "--B-- basetype" );
					std::cout << "basetype:" << std::endl;
					std::cout << "\tchar:\t\t\t" << ( int ) *( b->get< char >( "--B-- char" ).get() ) << std::endl;
					std::cout << "\tuchar:\t\t\t" << ( int ) *( b->get< unsigned char >( "--B-- uchar" ).get() ) << std::endl;
					std::cout << "\tint:\t\t\t" << ( int ) *( b->get< int >( "--B-- int" ).get() ) << std::endl;
					std::cout << "\tuint:\t\t\t" << ( int ) *( b->get< unsigned int >( "--B-- uint" ).get() ) << std::endl;
					std::cout << "\tfloat:\t\t\t" << *( b->get< float >( "--B-- float" ).get() ) << std::endl;
					std::cout << "\tdouble:\t\t\t" << *( b->get< double >( "--B-- double" ).get() ) << std::endl;
					std::cout << "\tstring:\t\t\t" << *( b->get< std::string >( "--B-- string" ).get() ) << std::endl;
					std::cout << "\tbool:\t\t\t" << std::boolalpha << *( b->get< bool >( "--B-- bool" ).get() ) << std::endl;
					std::vector< int > const& vi = *( b->get< std::vector< int > >( "--B-- int vector" ).get() );
					std::cout << "\tint vector:\t\t" << "size " << vi.size() << std::endl;
					std::vector< std::vector< int > > const& vvi = *( b->get< std::vector< std::vector< int > > >( "--B-- int vector vector" ).get() );
					std::cout << "\tint vector vector:\t" << "size " << vvi.size() << std::endl;
				std::shared_ptr< const CustomType > s = d->get< CustomType >( "--B-- simpletype" );
					std::cout << "simpletype:" << std::endl;
					std::cout << "\tint:\t\t\t" << ( int ) *( s->get< int >( "--B-- int" ).get() ) << std::endl;
					std::cout << "\tfloat:\t\t\t" << ( int ) *( s->get< float >( "--B-- float" ).get() ) << std::endl;
			}

			std::cout << std::endl;
			std::cout << "--------- current data of i3 (  --B-- complextype ) ----------------" << std::endl;
			std::cout << std::endl;

			{
				std::shared_ptr< const CustomType > d = b.inlets[ 3 ].getCurrentData();
				std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "--B-- int" ).get() ) << std::endl;
				std::cout << "string:\t\t\t" << *( d->get< std::string >( "--B-- string" ).get() ) << std::endl;
				std::vector< float > const& vf = *( d->get< std::vector< float > >( "--B-- float vector" ).get() );
				std::cout << "float vector:\t\t" << "size " <<  vf.size() << std::endl;
				std::shared_ptr< const CustomType > i = d->get< CustomType >( "--B-- image" );
					std::shared_ptr< const Image > img = Image::asImage( i );
					std::cout << "image:" << std::endl;
					std::cout << "\twidth:\t\t\t" << img->getWidth() << std::endl;
					std::cout << "\theight:\t\t\t" << img->getHeight() << std::endl;
					std::cout << "\tchannel order:\t\t" << ( std::string ) img->getChannelOrder() << std::endl;
					std::cout << "\tformat:\t\t\t" << ( std::string ) img->getDatatype() << std::endl;
				std::shared_ptr< const CustomType > b = d->get< CustomType >( "--B-- basetype" );
					std::cout << "basetype:" << std::endl;
					std::cout << "\tchar:\t\t\t" << ( int ) *( b->get< char >( "--B-- char" ).get() ) << std::endl;
					std::cout << "\tuchar:\t\t\t" << ( int ) *( b->get< unsigned char >( "--B-- uchar" ).get() ) << std::endl;
					std::cout << "\tint:\t\t\t" << ( int ) *( b->get< int >( "--B-- int" ).get() ) << std::endl;
					std::cout << "\tuint:\t\t\t" << ( int ) *( b->get< unsigned int >( "--B-- uint" ).get() ) << std::endl;
					std::cout << "\tfloat:\t\t\t" << *( b->get< float >( "--B-- float" ).get() ) << std::endl;
					std::cout << "\tdouble:\t\t\t" << *( b->get< double >( "--B-- double" ).get() ) << std::endl;
					std::cout << "\tstring:\t\t\t" << *( b->get< std::string >( "--B-- string" ).get() ) << std::endl;
					std::cout << "\tbool:\t\t\t" << std::boolalpha << *( b->get< bool >( "--B-- bool" ).get() ) << std::endl;
					std::vector< int > const& vi = *( b->get< std::vector< int > >( "--B-- int vector" ).get() );
					std::cout << "\tint vector:\t\t" << "size " << vi.size() << std::endl;
					std::vector< std::vector< int > > const& vvi = *( b->get< std::vector< std::vector< int > > >( "--B-- int vector vector" ).get() );
					std::cout << "\tint vector vector:\t" << "size " << vvi.size() << std::endl;
				std::shared_ptr< const CustomType > s = d->get< CustomType >( "--B-- simpletype" );
					std::cout << "simpletype:" << std::endl;
					std::cout << "\tint:\t\t\t" << ( int ) *( s->get< int >( "--B-- int" ).get() ) << std::endl;
					std::cout << "\tfloat:\t\t\t" << ( int ) *( s->get< float >( "--B-- float" ).get() ) << std::endl;
			}

			//std::cout << std::endl;
			//std::cout << "--------- current data of p2 ( complextype ) ----------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.parameters[ 2 ].getCurrentData();
			//	std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "test int" ).get() ) << std::endl;
			//	std::cout << "string:\t\t\t" << *( d->get< std::string >( "test string" ).get() ) << std::endl;
			//	std::vector< float > const& vf = *( d->get< std::vector< float > >( "test float vector" ).get() );
			//	std::cout << "float vector:\t\t" << "size " << vf.size() << std::endl;
			//	std::shared_ptr< const CustomType > i = d->get< CustomType >( "test image" );
			//	std::shared_ptr< const Image > img = Image::asImage( i );
			//	std::cout << "image:" << std::endl;
			//	std::cout << "\twidth:\t\t\t" << img->getWidth() << std::endl;
			//	std::cout << "\theight:\t\t\t" << img->getHeight() << std::endl;
			//	std::cout << "\tchannel order:\t\t" << ( std::string ) img->getChannelOrder() << std::endl;
			//	std::cout << "\tformat:\t\t\t" << ( std::string ) img->getDatatype() << std::endl;
			//	std::shared_ptr< const CustomType > b = d->get< CustomType >( "test basetype" );
			//	std::cout << "basetype:" << std::endl;
			//	std::cout << "\tchar:\t\t\t" << ( int ) *( b->get< char >( "char" ).get() ) << std::endl;
			//	std::cout << "\tuchar:\t\t\t" << ( int ) *( b->get< unsigned char >( "uchar" ).get() ) << std::endl;
			//	std::cout << "\tint:\t\t\t" << ( int ) *( b->get< int >( "int" ).get() ) << std::endl;
			//	std::cout << "\tuint:\t\t\t" << ( int ) *( b->get< unsigned int >( "uint" ).get() ) << std::endl;
			//	std::cout << "\tfloat:\t\t\t" << *( b->get< float >( "float" ).get() ) << std::endl;
			//	std::cout << "\tdouble:\t\t\t" << *( b->get< double >( "double" ).get() ) << std::endl;
			//	std::cout << "\tstring:\t\t\t" << *( b->get< std::string >( "string" ).get() ) << std::endl;
			//	std::cout << "\tbool:\t\t\t" << std::boolalpha << *( b->get< bool >( "bool" ).get() ) << std::endl;
			//	std::vector< int > const& vi = *( b->get< std::vector< int > >( "int vector" ).get() );
			//	std::cout << "\tint vector:\t\t" << "size " << vi.size() << std::endl;
			//	std::vector< std::vector< int > > const& vvi = *( b->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
			//	std::cout << "\tint vector vector:\t" << "size " << vvi.size() << std::endl;
			//	std::shared_ptr< const CustomType > s = d->get< CustomType >( "test simpletype" );
			//	std::cout << "simpletype:" << std::endl;
			//	std::cout << "\tint:\t\t\t" << ( int ) *( s->get< int >( "int" ).get() ) << std::endl;
			//	std::cout << "\tfloat:\t\t\t" << ( int ) *( s->get< float >( "float" ).get() ) << std::endl;
			//}

			//std::cout << std::endl;
			//std::cout << "--------- current data of p3 ( complextype ) ----------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.parameters[ 3 ].getCurrentData();
			//	std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "test int" ).get() ) << std::endl;
			//	std::cout << "string:\t\t\t" << *( d->get< std::string >( "test string" ).get() ) << std::endl;
			//	std::vector< float > const& vf = *( d->get< std::vector< float > >( "test float vector" ).get() );
			//	std::cout << "float vector:\t\t" << "size " <<  vf.size() << std::endl;
			//	std::shared_ptr< const CustomType > i = d->get< CustomType >( "test image" );
			//	std::shared_ptr< const Image > img = Image::asImage( i );
			//	std::cout << "image:" << std::endl;
			//	std::cout << "\twidth:\t\t\t" << img->getWidth() << std::endl;
			//	std::cout << "\theight:\t\t\t" << img->getHeight() << std::endl;
			//	std::cout << "\tchannel order:\t\t" << ( std::string ) img->getChannelOrder() << std::endl;
			//	std::cout << "\tformat:\t\t\t" << ( std::string ) img->getDatatype() << std::endl;
			//	std::shared_ptr< const CustomType > b = d->get< CustomType >( "test basetype" );
			//	std::cout << "basetype:" << std::endl;
			//	std::cout << "\tchar:\t\t\t" << ( int ) *( b->get< char >( "char" ).get() ) << std::endl;
			//	std::cout << "\tuchar:\t\t\t" << ( int ) *( b->get< unsigned char >( "uchar" ).get() ) << std::endl;
			//	std::cout << "\tint:\t\t\t" << ( int ) *( b->get< int >( "int" ).get() ) << std::endl;
			//	std::cout << "\tuint:\t\t\t" << ( int ) *( b->get< unsigned int >( "uint" ).get() ) << std::endl;
			//	std::cout << "\tfloat:\t\t\t" << *( b->get< float >( "float" ).get() ) << std::endl;
			//	std::cout << "\tdouble:\t\t\t" << *( b->get< double >( "double" ).get() ) << std::endl;
			//	std::cout << "\tstring:\t\t\t" << *( b->get< std::string >( "string" ).get() ) << std::endl;
			//	std::cout << "\tbool:\t\t\t" << std::boolalpha << *( b->get< bool >( "bool" ).get() ) << std::endl;
			//	std::vector< int > const& vi = *( b->get< std::vector< int > >( "int vector" ).get() );
			//	std::cout << "\tint vector:\t\t" << "size " << vi.size() << std::endl;
			//	std::vector< std::vector< int > > const& vvi = *( b->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
			//	std::cout << "\tint vector vector:\t" << "size " << vvi.size() << std::endl;
			//	std::shared_ptr< const CustomType > s = d->get< CustomType >( "test simpletype" );
			//	std::cout << "simpletype:" << std::endl;
			//	std::cout << "\tint:\t\t\t" << ( int ) *( s->get< int >( "int" ).get() ) << std::endl;
			//	std::cout << "\tfloat:\t\t\t" << ( int ) *( s->get< float >( "float" ).get() ) << std::endl;
			//}

			std::cout << std::endl;
			std::cout << "--------- type metainfo of --B-- simpletype -----------------------------------" << std::endl;
			std::cout << std::endl;

			{
				TypeMetainfo info = b.inlets[ 4 ].getType();
				std::cout << info.getTypename().first << " " << info.getTypename().second << std::endl;
				DataFields fields; info.getDataFields( fields );
				printout( std::cout, fields, 0 );
			}

			//std::cout << std::endl;
			//std::cout << "--------- current data of i4 ( simpletype ) ----------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.inlets[ 4 ].getCurrentData();
			//	std::cout << "\tint:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
			//	std::cout << "\tfloat:\t\t\t" << ( int ) *( d->get< float >( "float" ).get() ) << std::endl;
			//}

			//std::cout << std::endl;
			//std::cout << "--------- current data of i5 ( simpletype ) ----------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.inlets[ 5 ].getCurrentData();
			//	std::cout << "\tint:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
			//	std::cout << "\tfloat:\t\t\t" << ( int ) *( d->get< float >( "float" ).get() ) << std::endl;
			//}

			std::cout << std::endl;
			std::cout << "--------- type metainfo of int vector -----------------------------------" << std::endl;
			std::cout << std::endl;

			{
				TypeMetainfo info = b.inlets[ 6 ].getType();
				std::cout << info.getTypename().first << " " << info.getTypename().second << std::endl;
				DataFields fields; info.getDataFields( fields );
				printout( std::cout, fields, 0 );
			}

			//std::cout << std::endl;
			//std::cout << "--------- current data of i6 ( int vector ) ------------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.inlets[ 6 ].getCurrentData();
			//	std::vector< int > const& vi = *( d->get< std::vector< int > >( "default" ).get() );
			//	std::cout << "int vector:\t\t" << "size " << vi.size() << std::endl;
			//}

			//std::cout << std::endl;
			//std::cout << "--------- current data of i7 ( int vector ) ------------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.inlets[ 7 ].getCurrentData();
			//	std::vector< int > const& vi = *( d->get< std::vector< int > >( "default" ).get() );
			//	std::cout << "int vector:\t\t" << "size " << vi.size() << std::endl;
			//}

			std::cout << std::endl;
			std::cout << "--------- type metainfo of int vector vector -----------------------------------" << std::endl;
			std::cout << std::endl;

			{
				TypeMetainfo info = b.inlets[ 6 ].getType();
				std::cout << info.getTypename().first << " " << info.getTypename().second << std::endl;
				DataFields fields; info.getDataFields( fields );
				printout( std::cout, fields, 0 );
			}

			//std::cout << std::endl;
			//std::cout << "--------- current data of i8 ( int vector vector ) ----------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.inlets[ 8 ].getCurrentData();
			//	std::vector< std::vector< int > > const& vvi = *( d->get< std::vector< std::vector< int > > >( "default" ).get() );
			//	std::cout << "int vector:\t\t" << "size " << vvi.size() << std::endl;
			//}

			//std::cout << std::endl;
			//std::cout << "--------- current data of i9 ( int vector vector ) ----------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.inlets[ 9 ].getCurrentData();
			//	std::vector< std::vector< int > > const& vvi = *( d->get< std::vector< std::vector< int > > >( "default" ).get() );
			//	std::cout << "int vector:\t\t" << "size " << vvi.size() << std::endl;
			//}

			//std::cout << std::endl;
			//std::cout << "--------- type metainfo of recursivetype ------------------------------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	app::TypeMetainfo info = b.inlets[ 8 ].getType();
			//	_2Real::Fields fields; info.getFieldInfo( fields );
			//	printout( std::cout, fields, 0 );
			//	for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
			//		delete *it;
			//}

			//std::cout << std::endl;
			//std::cout << "--------- current data of i8 ( recursivetype ) ----------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.inlets[ 8 ].getCurrentData();
			//	//std::cout << "char:\t\t\t" << ( int ) *( d->get< char >( "char" ).get() ) << std::endl;
			//	//std::cout << "uchar:\t\t\t" << ( int ) *( d->get< unsigned char >( "uchar" ).get() ) << std::endl;
			//	//std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
			//	//std::cout << "uint:\t\t\t" << ( int ) *( d->get< unsigned int >( "uint" ).get() ) << std::endl;
			//	//std::cout << "float:\t\t\t" << *( d->get< float >( "float" ).get() ) << std::endl;
			//	//std::cout << "double:\t\t\t" << *( d->get< double >( "double" ).get() ) << std::endl;
			//	//std::cout << "string:\t\t\t" << *( d->get< std::string >( "string" ).get() ) << std::endl;
			//	//std::cout << "bool:\t\t\t" << std::boolalpha << *( d->get< bool >( "bool" ).get() ) << std::endl;
			//	//std::vector< int > const& vi = *( d->get< std::vector< int > >( "int vector" ).get() );
			//	//std::cout << "int vector:\t\t" << "size " <<  vi.size() << std::endl;
			//	//std::vector< std::vector< int > > const& vvi = *( d->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
			//	//std::cout << "int vector vector:\t" << "size " <<  vvi.size() << std::endl;
			//}

			//std::cout << std::endl;
			//std::cout << "--------- current data of i9 ( recursivetype ) ---------------" << std::endl;
			//std::cout << std::endl;

			//{
			//	std::shared_ptr< const CustomType > d = b.inlets[ 9 ].getCurrentData();
			//	//std::cout << "char:\t\t\t" << ( int ) *( d->get< char >( "char" ).get() ) << std::endl;
			//	//std::cout << "uchar:\t\t\t" << ( int ) *( d->get< unsigned char >( "uchar" ).get() ) << std::endl;
			//	//std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
			//	//std::cout << "uint:\t\t\t" << ( int ) *( d->get< unsigned int >( "uint" ).get() ) << std::endl;
			//	//std::cout << "float:\t\t\t" << *( d->get< float >( "float" ).get() ) << std::endl;
			//	//std::cout << "double:\t\t\t" << *( d->get< double >( "double" ).get() ) << std::endl;
			//	//std::cout << "string:\t\t\t" << *( d->get< std::string >( "string" ).get() ) << std::endl;
			//	//std::cout << "bool:\t\t\t" << std::boolalpha << *( d->get< bool >( "bool" ).get() ) << std::endl;
			//	//std::vector< int > const& vi = *( d->get< std::vector< int > >( "int vector" ).get() );
			//	//std::cout << "int vector:\t\t" << "size " <<  vi.size() << std::endl;
			//	//std::vector< std::vector< int > > const& vvi = *( d->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
			//	//std::cout << "int vector vector:\t" << "size " <<  vvi.size() << std::endl;
			//}

		}
		else if ( line == "s" )
		{
		}
		else if ( line == "unload a" )
		{
			testBundleA.unload();
		}
		else if ( line == "unload b" )
		{
			testBundleB.unload();
		}
	}

	std::cout << "----------- full clear ----------------" << std::endl;
	testEngine.clearAll();
	std::cout << "----------- full clear ----------------" << std::endl;

	return 0;
}