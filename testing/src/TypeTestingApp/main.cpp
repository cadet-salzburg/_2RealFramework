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

void printout( ostream &out, _2Real::Fields const& fields, const unsigned int offset )
{
	for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
	{
		_2Real::Field const& f = **it;
		for ( unsigned int i=0; i<offset; ++ i ) out << "\t";
		std::string name = f.getName();
		if ( name.length() >= 16 )
			out << f.getName() << "\t";
		else if ( name.length() >= 8 )
			out << f.getName() << "\t\t";
		else
			out << f.getName() << "\t\t\t";
		std::cout << f.getTypename() << std::endl;
		if ( !f.getSubFields().empty() )
		{
			printout( out, f.getSubFields(), offset+1 );
		}
	}
}

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	testEngine.setBaseDirectory( "." );

	struct BlockInstance
	{
		std::vector< InletHandle > inlets;
		std::vector< OutletHandle > outlets;
		BlockHandle block;
	};

	const unsigned int numInstances = 3;
	std::vector< BlockInstance > blockInstances( numInstances );

	try
	{
		BundleHandle testBundle = testEngine.loadBundle( "TypeTesting" );

		BundleInfo info = testBundle.getBundleInfo();
		BundleInfo::BlockInfos blocks = info.exportedBlocks;
		for ( BundleInfo::BlockInfoIterator it = blocks.begin(); it != blocks.end(); ++it )
		{
			std::cout << "-b\t" << it->name << std::endl;
			BlockInfo::InletInfos inlets = it->inlets;
			for ( BlockInfo::InletInfoIterator iIt = inlets.begin(); iIt != inlets.end(); ++iIt )
			{
				std::cout << "-i\t" << iIt->name << ( iIt->isMultiInlet ? "\t+m " : "\t-m " ) << std::endl;
			}
			BlockInfo::OutletInfos outlets = it->outlets;
			for ( BlockInfo::OutletInfoIterator oIt = outlets.begin(); oIt != outlets.end(); ++oIt )
			{
				std::cout << "-o\t" << oIt->name << std::endl;
			}
		}

		for ( unsigned int i=0; i<numInstances; ++i )
		{
			BlockInstance &b = blockInstances[ i ];
			b.block = testBundle.createBlockInstance( "TypeTestingBlock" );

			std::vector< InletHandle > const& inlets = b.block.getAllInletHandles();
			for ( unsigned int i=0; i<inlets.size(); ++i )
				b.inlets.push_back( inlets[ i ] );
			std::vector< OutletHandle > const& outlets = b.block.getAllOutletHandles();
			for ( unsigned int o=0; o<outlets.size(); ++o )
				b.outlets.push_back( outlets[ o ] );

			b.block.setup();
			b.block.start();
		}

		for ( unsigned int i=0; i<numInstances-1; ++i )
		{
			blockInstances[ i ].outlets[ 0 ].link( blockInstances[ i+1 ].inlets[ 0 ] );
			blockInstances[ i ].outlets[ 1 ].link( blockInstances[ i+1 ].inlets[ 2 ] );
			blockInstances[ i ].outlets[ 2 ].link( blockInstances[ i+1 ].inlets[ 4 ] );
		}
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;
	}

	while( 1 )
	{
		string line;
		char lineEnd = '\n';
		getline( cin, line, lineEnd );
		if ( line == "q" )
		{
			break;
		}
		else if ( line == "p" )
		{
			BlockInstance &b = blockInstances[ 1 ];

			std::cout << std::endl;
			std::cout << "--------- type metainfo of basetype ------------------------------------" << std::endl;
			std::cout << std::endl;

			{
				app::TypeMetainfo info = b.inlets[ 0 ].getType();
				_2Real::Fields fields; info.getFieldInfo( fields );
				printout( std::cout, fields, 0 );
				for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
					delete *it;
			}

			std::cout << std::endl;
			std::cout << "--------- current data of i0 ( basetype ) ----------------" << std::endl;
			std::cout << std::endl;

			{
				std::shared_ptr< const CustomType > d = b.inlets[ 0 ].getCurrentData();
				std::cout << "char:\t\t\t" << ( int ) *( d->get< char >( "char" ).get() ) << std::endl;
				std::cout << "uchar:\t\t\t" << ( int ) *( d->get< unsigned char >( "uchar" ).get() ) << std::endl;
				std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
				std::cout << "uint:\t\t\t" << ( int ) *( d->get< unsigned int >( "uint" ).get() ) << std::endl;
				std::cout << "float:\t\t\t" << *( d->get< float >( "float" ).get() ) << std::endl;
				std::cout << "double:\t\t\t" << *( d->get< double >( "double" ).get() ) << std::endl;
				std::cout << "string:\t\t\t" << *( d->get< std::string >( "string" ).get() ) << std::endl;
				std::cout << "bool:\t\t\t" << std::boolalpha << *( d->get< bool >( "bool" ).get() ) << std::endl;
				std::vector< int > const& vi = *( d->get< std::vector< int > >( "int vector" ).get() );
				std::cout << "int vector:\t\t" << "size " <<  vi.size() << std::endl;
				std::vector< std::vector< int > > const& vvi = *( d->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
				std::cout << "int vector vector:\t" << "size " <<  vvi.size() << std::endl;
			}

			std::cout << std::endl;
			std::cout << "--------- current data of i1 ( basetype ) ---------------" << std::endl;
			std::cout << std::endl;

			{
				std::shared_ptr< const CustomType > d = b.inlets[ 1 ].getCurrentData();
				std::cout << "char:\t\t\t" << ( int ) *( d->get< char >( "char" ).get() ) << std::endl;
				std::cout << "uchar:\t\t\t" << ( int ) *( d->get< unsigned char >( "uchar" ).get() ) << std::endl;
				std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
				std::cout << "uint:\t\t\t" << ( int ) *( d->get< unsigned int >( "uint" ).get() ) << std::endl;
				std::cout << "float:\t\t\t" << *( d->get< float >( "float" ).get() ) << std::endl;
				std::cout << "double:\t\t\t" << *( d->get< double >( "double" ).get() ) << std::endl;
				std::cout << "string:\t\t\t" << *( d->get< std::string >( "string" ).get() ) << std::endl;
				std::cout << "bool:\t\t\t" << std::boolalpha << *( d->get< bool >( "bool" ).get() ) << std::endl;
				std::vector< int > const& vi = *( d->get< std::vector< int > >( "int vector" ).get() );
				std::cout << "int vector:\t\t" << "size " <<  vi.size() << std::endl;
				std::vector< std::vector< int > > const& vvi = *( d->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
				std::cout << "int vector vector:\t" << "size " <<  vvi.size() << std::endl;
			}

			std::cout << std::endl;
			std::cout << "--------- type metainfo of complextype -----------------------------------" << std::endl;
			std::cout << std::endl;

			{
				app::TypeMetainfo info = b.inlets[ 2 ].getType();
				_2Real::Fields fields; info.getFieldInfo( fields );
				printout( std::cout, fields, 0 );
				for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
					delete *it;
			}

			std::cout << std::endl;
			std::cout << "--------- current data of i2 ( complextype ) ----------------" << std::endl;
			std::cout << std::endl;

			{
				std::shared_ptr< const CustomType > d = b.inlets[ 2 ].getCurrentData();
				std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
				std::cout << "string:\t\t\t" << *( d->get< std::string >( "string" ).get() ) << std::endl;
				std::vector< float > const& vf = *( d->get< std::vector< float > >( "float vector" ).get() );
				std::cout << "float vector:\t\t" << "size " << vf.size() << std::endl;
				std::shared_ptr< const CustomType > i = d->get< CustomType >( "image" );
				std::shared_ptr< const Image > img = Image::asImage( i );
				std::cout << "image:" << std::endl;
				std::cout << "\twidth:\t\t\t" << img->getWidth() << std::endl;
				std::cout << "\theight:\t\t\t" << img->getHeight() << std::endl;
				std::cout << "\tchannel order:\t\t" << ( std::string ) img->getChannelOrder() << std::endl;
				std::cout << "\tformat:\t\t\t" << ( std::string ) img->getDatatype() << std::endl;
				std::shared_ptr< const CustomType > b = d->get< CustomType >( "basetype" );
				std::cout << "basetype:" << std::endl;
				std::cout << "\tchar:\t\t\t" << ( int ) *( b->get< char >( "char" ).get() ) << std::endl;
				std::cout << "\tuchar:\t\t\t" << ( int ) *( b->get< unsigned char >( "uchar" ).get() ) << std::endl;
				std::cout << "\tint:\t\t\t" << ( int ) *( b->get< int >( "int" ).get() ) << std::endl;
				std::cout << "\tuint:\t\t\t" << ( int ) *( b->get< unsigned int >( "uint" ).get() ) << std::endl;
				std::cout << "\tfloat:\t\t\t" << *( b->get< float >( "float" ).get() ) << std::endl;
				std::cout << "\tdouble:\t\t\t" << *( b->get< double >( "double" ).get() ) << std::endl;
				std::cout << "\tstring:\t\t\t" << *( b->get< std::string >( "string" ).get() ) << std::endl;
				std::cout << "\tbool:\t\t\t" << std::boolalpha << *( b->get< bool >( "bool" ).get() ) << std::endl;
				std::vector< int > const& vi = *( b->get< std::vector< int > >( "int vector" ).get() );
				std::cout << "\tint vector:\t\t" << "size " << vi.size() << std::endl;
				std::vector< std::vector< int > > const& vvi = *( b->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
				std::cout << "\tint vector vector:\t" << "size " << vvi.size() << std::endl;
				std::shared_ptr< const CustomType > s = d->get< CustomType >( "simpletype" );
				std::cout << "simpletype:" << std::endl;
				std::cout << "\tint:\t\t\t" << ( int ) *( s->get< int >( "int" ).get() ) << std::endl;
				std::cout << "\tfloat:\t\t\t" << ( int ) *( s->get< float >( "float" ).get() ) << std::endl;
			}

			std::cout << std::endl;
			std::cout << "--------- current data of i3 ( complextype ) ----------------" << std::endl;
			std::cout << std::endl;

			{
				std::shared_ptr< const CustomType > d = b.inlets[ 3 ].getCurrentData();
				std::cout << "int:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
				std::cout << "string:\t\t\t" << *( d->get< std::string >( "string" ).get() ) << std::endl;
				std::vector< float > const& vf = *( d->get< std::vector< float > >( "float vector" ).get() );
				std::cout << "float vector:\t\t" << "size " <<  vf.size() << std::endl;
				std::shared_ptr< const CustomType > i = d->get< CustomType >( "image" );
				std::shared_ptr< const Image > img = Image::asImage( i );
				std::cout << "image:" << std::endl;
				std::cout << "\twidth:\t\t\t" << img->getWidth() << std::endl;
				std::cout << "\theight:\t\t\t" << img->getHeight() << std::endl;
				std::cout << "\tchannel order:\t\t" << ( std::string ) img->getChannelOrder() << std::endl;
				std::cout << "\tformat:\t\t\t" << ( std::string ) img->getDatatype() << std::endl;
				std::shared_ptr< const CustomType > b = d->get< CustomType >( "basetype" );
				std::cout << "basetype:" << std::endl;
				std::cout << "\tchar:\t\t\t" << ( int ) *( b->get< char >( "char" ).get() ) << std::endl;
				std::cout << "\tuchar:\t\t\t" << ( int ) *( b->get< unsigned char >( "uchar" ).get() ) << std::endl;
				std::cout << "\tint:\t\t\t" << ( int ) *( b->get< int >( "int" ).get() ) << std::endl;
				std::cout << "\tuint:\t\t\t" << ( int ) *( b->get< unsigned int >( "uint" ).get() ) << std::endl;
				std::cout << "\tfloat:\t\t\t" << *( b->get< float >( "float" ).get() ) << std::endl;
				std::cout << "\tdouble:\t\t\t" << *( b->get< double >( "double" ).get() ) << std::endl;
				std::cout << "\tstring:\t\t\t" << *( b->get< std::string >( "string" ).get() ) << std::endl;
				std::cout << "\tbool:\t\t\t" << std::boolalpha << *( b->get< bool >( "bool" ).get() ) << std::endl;
				std::vector< int > const& vi = *( b->get< std::vector< int > >( "int vector" ).get() );
				std::cout << "\tint vector:\t\t" << "size " << vi.size() << std::endl;
				std::vector< std::vector< int > > const& vvi = *( b->get< std::vector< std::vector< int > > >( "int vector vector" ).get() );
				std::cout << "\tint vector vector:\t" << "size " << vvi.size() << std::endl;
				std::shared_ptr< const CustomType > s = d->get< CustomType >( "simpletype" );
				std::cout << "simpletype:" << std::endl;
				std::cout << "\tint:\t\t\t" << ( int ) *( s->get< int >( "int" ).get() ) << std::endl;
				std::cout << "\tfloat:\t\t\t" << ( int ) *( s->get< float >( "float" ).get() ) << std::endl;
			}

			std::cout << std::endl;
			std::cout << "--------- type metainfo of simpletype -----------------------------------" << std::endl;
			std::cout << std::endl;

			{
				app::TypeMetainfo info = b.inlets[ 4 ].getType();
				_2Real::Fields fields; info.getFieldInfo( fields );
				printout( std::cout, fields, 0 );
				for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
					delete *it;
			}

			std::cout << std::endl;
			std::cout << "--------- current data of i4 ( simpletype ) ----------------" << std::endl;
			std::cout << std::endl;

			{
				std::shared_ptr< const CustomType > d = b.inlets[ 4 ].getCurrentData();
				std::cout << "\tint:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
				std::cout << "\tfloat:\t\t\t" << ( int ) *( d->get< float >( "float" ).get() ) << std::endl;
			}

			std::cout << std::endl;
			std::cout << "--------- current data of i5 ( simpletype ) ----------------" << std::endl;
			std::cout << std::endl;

			{
				std::shared_ptr< const CustomType > d = b.inlets[ 5 ].getCurrentData();
				std::cout << "\tint:\t\t\t" << ( int ) *( d->get< int >( "int" ).get() ) << std::endl;
				std::cout << "\tfloat:\t\t\t" << ( int ) *( d->get< float >( "float" ).get() ) << std::endl;
			}

			std::cout << std::endl;
			std::cout << "--------- current data of i6 ( int vector ) ------------------" << std::endl;
			std::cout << std::endl;

			{
				std::shared_ptr< const CustomType > d = b.inlets[ 6 ].getCurrentData();
				std::vector< int > const& vi = *( d->get< std::vector< int > >( "default" ).get() );
				std::cout << "int vector:\t\t" << "size " << vi.size() << std::endl;
			}

			std::cout << std::endl;
			std::cout << "--------- current data of i7 ( int vector vector ) ----------------" << std::endl;
			std::cout << std::endl;

			{
				std::shared_ptr< const CustomType > d = b.inlets[ 7 ].getCurrentData();
				std::vector< std::vector< int > > const& vvi = *( d->get< std::vector< std::vector< int > > >( "default" ).get() );
				std::cout << "int vector:\t\t" << "size " << vvi.size() << std::endl;
			}

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
	}

	return 0;
}