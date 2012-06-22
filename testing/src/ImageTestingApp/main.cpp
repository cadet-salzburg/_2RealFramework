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
#include "_2RealImageT.h"

#include <windows.h>
#include <iostream>

#include "vld.h"

#include "Poco/Mutex.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;
using _2Real::ImageT;
using _2Real::ImageChannelT;
using _2Real::ImageChannelOrder;
using _2Real::OutputData;
using _2Real::Engine;
using _2Real::System;
using _2Real::BundleIdentifier;
using _2Real::BlockIdentifier;
using _2Real::UpdatePolicy;
using _2Real::Exception;

template< typename T >
class Receiver
{

public:

	void receiveData( OutputData &data )
	{
		m_Access.lock();

		m_Received = data.getData< T >();

		m_Access.unlock();
	}

private:

	Poco::FastMutex	m_Access;
	T				m_Received;
	//T				m_Current;

};

#ifndef _DEBUG
	#define shared_library_suffix ".dll"
#else
	#define shared_library_suffix "_d.dll"
#endif

int main( int argc, char *argv[] )
{
	Engine &testEngine = Engine::instance();
	System testSystem( "test system" );

	try
	{
		BundleIdentifier testBundle = testEngine.load( string( "ImageTesting" ).append( shared_library_suffix ) );

		UpdatePolicy fpsTrigger;
		fpsTrigger.triggerByUpdateRate( 100.0f );

		UpdatePolicy newTrigger;
		newTrigger.triggerWhenAllDataNew();

		BlockIdentifier out = testSystem.createBlock( testBundle, "image out" );
		testSystem.setup( out );
		testSystem.setPolicy( out, fpsTrigger );
		testSystem.start( out );

		BlockIdentifier inout = testSystem.createBlock( testBundle, "image in - out" );
		testSystem.setup( inout );
		testSystem.setPolicy( inout, newTrigger );
		testSystem.start( inout );

		BlockIdentifier in = testSystem.createBlock( testBundle, "image in" );
		testSystem.setup( in );
		testSystem.setPolicy( in, newTrigger );
		testSystem.start( in );

		testSystem.link( out, "image outlet", inout, "image inlet" );
		testSystem.link( inout, "image outlet", in, "image inlet" );

		//ImageT< unsigned int > *img1 = new ImageT< unsigned int >( 10, 10, ImageChannelOrder::RGB );

		//ImageT< unsigned int >::iterator it1a = img1->iter();
		//while( it1a.nextLine() )
		//{
		//	while ( it1a.nextPixel() )
		//	{
		//		it1a.r() = 255;
		//		it1a.g() = 0;
		//		it1a.b() = 0;
		//	}
		//}

		//ImageT< unsigned int > img2( *img1 );

		//delete img1;

		//ImageT< unsigned int >::iterator it2a = img2.iter();
		//while( it2a.nextLine() )
		//{
		//	while ( it2a.nextPixel() )
		//	{
		//		cout << it2a.x() << " " << it2a.y() << " : " << it2a.r() << " " << it2a.g() << " " << it2a.b() << endl;
		//	}
		//}

		//ImageT< unsigned int > img3 = img2;

		//ImageT< unsigned int >::iterator it2b = img2.iter();
		//while( it2b.nextLine() )
		//{
		//	while ( it2b.nextPixel() )
		//	{
		//		it2b.r() = 0;
		//		it2b.g() = 255;
		//		it2b.b() = 0;
		//	}
		//}

		//ImageT< unsigned int >::iterator it3a = img3.iter();
		//while( it3a.nextLine() )
		//{
		//	while ( it3a.nextPixel() )
		//	{
		//		cout << it3a.x() << " " << it3a.y() << " : " << it3a.r() << " " << it3a.g() << " " << it3a.b() << endl;
		//	}
		//}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
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
	}

	return 0;
}