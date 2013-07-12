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

ostream& operator<<( ostream &out, _2Real::Fields const& fields )
{
	for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
	{
		_2Real::Field const& f = **it;
		out << f.getName() << " " << f.getTypename() << std::endl;
		if ( !f.getSubFields().empty() )
		{
			out << "---" << endl;
			out << f.getSubFields();
			out << "---" << endl;
		}
	}
	return out;
}

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	testEngine.setBaseDirectory( "." );

	InletHandle i00, i10, i01, i11;
	OutletHandle o0, o1;

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

		BlockHandle testBlock0 = testBundle.createBlockInstance( "TypeTestingBlock" );
		BlockHandle testBlock1 = testBundle.createBlockInstance( "TypeTestingBlock" );

		i00 = testBlock0.getInletHandle( "customInlet0" );
		i01 = testBlock0.getInletHandle( "customInlet1" );
		o0 = testBlock0.getOutletHandle( "customOutlet0" );

		i10 = testBlock1.getInletHandle( "customInlet0" );
		i11 = testBlock1.getInletHandle( "customInlet1" );
		o1 = testBlock1.getOutletHandle( "customOutlet0" );

		testBlock0.setup();
		testBlock0.start();

		o0.link( i10 );

		testBlock1.setup();
		testBlock1.start();
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;
	}

	unsigned int cnt = 0;

	while( 1 )
	{
		std::shared_ptr< const CustomType > o0data = o0.getCurrentData();

		std::shared_ptr< const CustomType > i10data = i10.getCurrentData();
		if ( i10data.get() )
		{
			std::shared_ptr< const CustomType > custom = i10data->get< CustomType >( "test image" );
			std::shared_ptr< const Image > img = Image::asImage( custom );
			std::cout << img->getWidth() << " " << img->getHeight() << std::endl;
		}

		std::shared_ptr< CustomType > testData = i00.makeData();
		testData->set< int >( "test int", ++cnt );
		Image img;
		std::vector< unsigned char > data( 16, 255 );
		img.setImagedata( &data[ 0 ], 2, 2, Image::ChannelOrder::RGBA, Image::Datatype::UINT8 );
		testData->set< CustomType >( "test image", *( img.toCustomType().get() ) );
		i00.receiveData( testData );

		//app::TypeMetainfo info = i00.getType();
		//_2Real::Fields fields; info.getFieldInfo( fields );
		//std::cout << fields << std::endl;
		//for ( _2Real::Fields::const_iterator it = fields.begin(); it != fields.end(); ++it )
		//	delete *it;

		string line;
		char lineEnd = '\n';
		getline( cin, line, lineEnd );
		if ( line == "q" )
		{
			break;
		}
	}

	return 0;
}