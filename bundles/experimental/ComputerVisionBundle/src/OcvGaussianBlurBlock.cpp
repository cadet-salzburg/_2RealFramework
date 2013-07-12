/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
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
#include "OcvGaussianBlurBlock.h"
#include "ImageHelpers.h"
#include "_2RealDatatypes.h"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

#define VERBOSE

unsigned int cnt = 0;

OcvGaussianBlurBlock::OcvGaussianBlurBlock() :
	Block()
  , mInstanceNumber( ++cnt )
	{}

OcvGaussianBlurBlock::~OcvGaussianBlurBlock() {}

void OcvGaussianBlurBlock::setup( BlockHandle &block )
{
	try
	{
		mBlock = block;
#ifdef VERBOSE
		std::cout << "setup: " << mInstanceNumber << std::endl;
#endif
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void OcvGaussianBlurBlock::update()
{
	try
	{
#ifdef VERBOSE
		std::cout << "updating: " << mInstanceNumber << std::endl;
#endif
		InletHandle inA = mBlock.getInletHandle( "InImageA" );
		InletHandle inB = mBlock.getInletHandle( "InImageB" );
		OutletHandle out = mBlock.getOutletHandle( "OutImage" );

		std::shared_ptr< const CustomType > inputA = inA.getReadableRef();
		std::shared_ptr< const CustomType > inputB = inB.getReadableRef();
		std::shared_ptr< CustomType > output = out.getWriteableRef();

		std::shared_ptr< const Image > imgA = Image::asImage( inputA );
		std::shared_ptr< const Image > imgB = Image::asImage( inputB );
		std::shared_ptr< Image > imgOut = Image::asImage( output );

		unsigned int szA = imgA->getWidth() * imgA->getHeight();
		unsigned int szB = imgB->getWidth() * imgB->getHeight();

		unsigned int wOut = std::min< unsigned int >( imgA->getWidth(), imgB->getWidth() );
		unsigned int hOut = std::min< unsigned int >( imgA->getHeight(), imgB->getHeight() );

		imgOut->createImagedata( wOut, hOut, Image::ChannelOrder::RGB, Image::Datatype::FLOAT32 );

		std::cout << "created: " << imgOut->getWidth() << " " << imgOut->getHeight() << std::endl;

		float *f = reinterpret_cast< float * >( imgOut->getPixels() );
		float const *a = reinterpret_cast< float const* >( imgA->getPixels() );
		float const *b = reinterpret_cast< float const* >( imgB->getPixels() );
		for ( unsigned int y=0; y<hOut; ++y )
		{
			for ( unsigned int x=0; x<wOut; ++x )
			{
				const unsigned int index = y * wOut + x;
				//std::cout << index << std::endl;
				f[ index ] = a[ index ] - b[ index ];
			}
		}
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void OcvGaussianBlurBlock::shutdown()
{
	try
	{
#ifdef VERBOSE
		std::cout << "shutdown: " << mInstanceNumber << std::endl;
#endif
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}